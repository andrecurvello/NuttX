/****************************************************************************
 * arch/arm/src/lpc17xx/lpc17_ethernet.c
 *
 *   Copyright (C) 2010 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <spudmonkey@racsa.co.cr>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#if defined(CONFIG_NET) && defined(CONFIG_LPC17_ETHERNET)

#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <debug.h>
#include <wdog.h>
#include <errno.h>

#include <nuttx/irq.h>
#include <nuttx/arch.h>
#include <nuttx/mii.h>

#include <net/uip/uip.h>
#include <net/uip/uip-arp.h>
#include <net/uip/uip-arch.h>

#include "chip.h"
#include "up_arch.h"
#include "lpc17_syscon.h"
#include "lpc17_ethernet.h"
#include "lpc17_internal.h"

#include <arch/board/board.h>

/* Does this chip have and ethernet controller? */

#if LPC17_NETHCONTROLLERS > 0

/****************************************************************************
 * Definitions
 ****************************************************************************/

/* CONFIG_LPC17_NINTERFACES determines the number of physical interfaces
 * that will be supported.
 */

#if !defined(CONFIG_LPC17_NINTERFACES) || CONFIG_LPC17_NINTERFACES > LPC17_NETHCONTROLLERS
# define CONFIG_LPC17_NINTERFACES LPC17_NETHCONTROLLERS
#endif

/* The logic here has a few hooks for support for multiple interfaces, but
 * that capability is not yet in place (and I won't worry about it until I get
 * the first multi-interface LPC17xx).
 */

#if CONFIG_LPC17_NINTERFACES > 1
#  warning "Only a single ethernet controller is supported"
#  undef CONFIG_LPC17_NINTERFACES
#  define CONFIG_LPC17_NINTERFACES 1
#endif

/* TX poll deley = 1 seconds. CLK_TCK is the number of clock ticks per second */

#define LPC17_WDDELAY   (1*CLK_TCK)
#define LPC17_POLLHSEC  (1*2)

/* TX timeout = 1 minute */

#define LPC17_TXTIMEOUT (60*CLK_TCK)

/* This is a helper pointer for accessing the contents of the Ethernet header */

#define BUF ((struct uip_eth_hdr *)priv->lp_dev.d_buf)

/* Select PHY-specific values.  Add more PHYs as needed. */

#ifdef CONFIG_PHY_KS8721
#  define LPC17_PHYNAME    "KS8721"
#  define LPC17_PHYID1     MII_PHYID1_KS8721
#  define LPC17_PHYID2     MII_PHYID2_KS8721
#  define LPC17_HAVE_PHY   1
#else
#  warning "No PHY specified!"
#  undef LPC17_HAVE_PHY
#endif

#define MII_BIG_TIMEOUT    666666

/* These definitions are used to remember the speed/duplex settings */

#define LPC17_SPEED_MASK   0x01
#define LPC17_SPEED_100    0x01
#define LPC17_SPEED_10     0x00

#define LPC17_DUPLEX_MASK  0x02
#define LPC17_DUPLEX_FULL  0x02
#define LPC17_DUPLEX_HALF  0x00

#define LPC17_10BASET_HD   (LPC17_SPEED_10  | LPC17_DUPLEX_HALF)
#define LPC17_10BASET_FD   (LPC17_SPEED_10  | LPC17_DUPLEX_FULL)
#define LPC17_100BASET_HD  (LPC17_SPEED_100 | LPC17_DUPLEX_HALF)
#define LPC17_100BASET_FD  (LPC17_SPEED_100 | LPC17_DUPLEX_FULL)

#ifdef CONFIG_PHY_SPEED100
#  ifdef CONFIG_PHY_FDUPLEX
#    define LPC17_MODE_DEFLT LPC17_100BASET_FD
#  else
#    define LPC17_MODE_DEFLT LPC17_100BASET_HD
#  endif
#else
#  ifdef CONFIG_PHY_FDUPLEX
#    define LPC17_MODE_DEFLT LPC17_10BASET_FD
#  else
#    define LPC17_MODE_DEFLT LPC17_10BASET_HD
#  endif
#endif

/* This is the number of ethernet GPIO pins that must be configured */

#define GPIO_NENET_PINS      10

/* EMAC DMA RAM and descriptor definitions.
 *
 * All of AHB SRAM, Bank 0 is set aside for EMAC Tx and Rx descriptors.
 */

#define LPC17_BANK0_SIZE     0x00004000
#warning "Need to exclude bank0 from the heap"

/* Numbers of descriptors and sizes of descriptor and status regions */

#ifdef CONFIG_ETH_NTXDESC
#  define CONFIG_ETH_NTXDESC 16
#endif
#define LPC17_TXDESC_SIZE    (8*CONFIG_ETH_NTXDESC)
#define LPC17_TXSTAT_SIZE    (4*CONFIG_ETH_NTXDESC)

#ifdef CONFIG_ETH_NRXDESC
#  define CONFIG_ETH_NRXDESC 16
#endif
#define LPC17_RXDESC_SIZE    (8*CONFIG_ETH_NRXDESC)
#define LPC17_RXSTAT_SIZE    (8*CONFIG_ETH_NRXDESC)

/* Descriptor Memory Organization.  Descriptors are packed
 * at the end of AHB SRAM, Bank 0.
 */

#define LPC17_DESC_SIZE      (LPC17_TXDESC_SIZE+LPC17_RXDESC_SIZE+LPC17_TXSTAT_SIZE+LPC17_RXSTAT_SIZE)
#define LPC17_DESC_BASE      (LPC17_SRAM_BANK0+LPC17_BANK0_SIZE-LPC17_DESC_SIZE)

#define LPC17_TXDESC_BASE    LPC17_DESC_BASE
#define LPC17_TXSTAT_BASE    (LPC17_TXDESC_BASE+LPC17_TXDESC_SIZE)
#define LPC17_RXDESC_BASE    (LPC17_TXSTAT_BASE+LPC17_TXSTAT_SIZE)
#define LPC17_RXSTAT_BASE    (LPC17_RXDESC_BASE + LPC17_RXDESC_SIZE)

/* Register debug */

#ifndef CONFIG_DEBUG
#  undef  CONFIG_LPC17_ENET_REGDEBUG
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* The lpc17_driver_s encapsulates all state information for a single hardware
 * interface
 */

struct lpc17_driver_s
{
  /* The following fields would only be necessary on chips that support
   * multiple Ethernet controllers.
   */

#if LM3S_NETHCONTROLLERS > 1
  uint32_t lp_base;             /* Ethernet controller base address */
  int      lp_irq;              /* Ethernet controller IRQ */
#endif

  bool     lp_ifup;             /* true:ifup false:ifdown */
  bool     lp_mode;             /* speed/duplex */
#ifdef LPC17_HAVE_PHY
  uint8_t  lp_phyaddr;          /* PHY device address */
#endif
  WDOG_ID  lp_txpoll;           /* TX poll timer */
  WDOG_ID  lp_txtimeout;        /* TX timeout timer */

  /* This holds the information visible to uIP/NuttX */

  struct uip_driver_s lp_dev;  /* Interface understood by uIP */
};

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Array of ethernet driver status structures */

static struct lpc17_driver_s g_ethdrvr[CONFIG_LPC17_NINTERFACES];

/* ENET pins are on P1[0,1,4,6,8,9,10,14,15] + MDC on P1[16] or P2[8] and
 * MDIO on P1[17] or P2[9].  The board.h file will define GPIO_ENET_MDC and
 * PGIO_ENET_MDIO to selec which pin setting to use.
 *
 * On older Rev '-' devices, P1[6] ENET-TX_CLK would also have be to configured.
 */

static const uint16_t g_enetpins[GPIO_NENET_PINS] =
{
  GPIO_ENET_TXD0, GPIO_ENET_TXD1, GPIO_ENET_TXEN,   GPIO_ENET_CRS, GPIO_ENET_RXD0,
  GPIO_ENET_RXD1, GPIO_ENET_RXER, GPIO_ENET_REFCLK, GPIO_ENET_MDC, GPIO_ENET_MDIO
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* Register operations */

#ifdef CONFIG_LPC17_ENET_REGDEBUG
static void lpc17_printreg(uint32_t addr, uint32_t val, bool iswrite);
static void lpc17_checkreg(uint32_t addr, uint32_t val, bool iswrite);
static uint32_t lpc17_getreg(uint32_t addr);
static void lpc17_putreg(uint32_t val, uint32_t addr);
#else
# define lpc17_getreg(addr)     getreg32(addr)
# define lpc17_putreg(val,addr) putreg32(val,addr)
#endif

/* Common TX logic */

static int  lpc17_transmit(FAR struct lpc17_driver_s *priv);
static int  lpc17_uiptxpoll(struct uip_driver_s *dev);

/* Interrupt handling */

static void lpc17_receive(FAR struct lpc17_driver_s *priv);
static void lpc17_txdone(FAR struct lpc17_driver_s *priv);
static int  lpc17_interrupt(int irq, FAR void *context);

/* Watchdog timer expirations */

static void lpc17_polltimer(int argc, uint32_t arg, ...);
static void lpc17_txtimeout(int argc, uint32_t arg, ...);

/* NuttX callback functions */

static int lpc17_ifup(struct uip_driver_s *dev);
static int lpc17_ifdown(struct uip_driver_s *dev);
static int lpc17_txavail(struct uip_driver_s *dev);
#ifdef CONFIG_NET_IGMP
static int lpc17_addmac(struct uip_driver_s *dev, FAR const uint8_t *mac);
static int lpc17_rmmac(struct uip_driver_s *dev, FAR const uint8_t *mac);
#endif

/* Initialization functions */

#ifdef CONFIG_LPC17_ENET_REGDEBUG
static void lpc17_showpins(void);
#else
#  define lpc17_showpins()
#endif

/* PHY initialization functions */

#ifdef LPC17_HAVE_PHY
#  ifdef CONFIG_LPC17_ENET_REGDEBUG
static void lpc17_showmii(uint8_t phyaddr, const char *msg);
#  else
#    define lpc17_showmii(phyaddr,msg)
#  endif

static void lpc17_phywrite(uint8_t phyaddr, uint8_t regaddr,
                           uint16_t phydata);
static uint16_t lpc17_phyread(uint8_t phyaddr, uint8_t regaddr);
static inline int lpc17_phyreset(uint8_t phyaddr);
#  ifdef CONFIG_PHY_AUTONEG
static inline int lpc17_phyautoneg(uint8_t phyaddr);
#  endif
static int lpc17_phymode(uint8_t phyaddr, uint8_t mode);
static inline int lpc17_phyinit(struct lpc17_driver_s *priv);
#else
#  define lpc17_phyinit(priv)
#endif

/* EMAC Initialization functions */

static void lpc17_macmode(uint8_t mode);
static void lpc17_ethreset(struct lpc17_driver_s *priv);

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/*******************************************************************************
 * Name: lpc17_printreg
 *
 * Description:
 *   Print the contents of an LPC17xx register operation
 *
 *******************************************************************************/

#ifdef CONFIG_LPC17_ENET_REGDEBUG
static void lpc17_printreg(uint32_t addr, uint32_t val, bool iswrite)
{
  dbg("%08x%s%08x\n", addr, iswrite ? "<-" : "->", val);
}
#endif

/*******************************************************************************
 * Name: lpc17_checkreg
 *
 * Description:
 *   Get the contents of an LPC17xx register
 *
 *******************************************************************************/

#ifdef CONFIG_LPC17_ENET_REGDEBUG
static void lpc17_checkreg(uint32_t addr, uint32_t val, bool iswrite)
{
  static uint32_t prevaddr = 0;
  static uint32_t preval = 0;
  static uint32_t count = 0;
  static bool     prevwrite = false;

  /* Is this the same value that we read from/wrote to the same register last time?
   * Are we polling the register?  If so, suppress the output.
   */

  if (addr == prevaddr && val == preval && prevwrite == iswrite)
    {
      /* Yes.. Just increment the count */

      count++;
    }
  else
    {
      /* No this is a new address or value or operation. Were there any
       * duplicate accesses before this one?
       */

      if (count > 0)
        {
          /* Yes.. Just one? */

          if (count == 1)
            {
              /* Yes.. Just one */

              lpc17_printreg(prevaddr, preval, prevwrite);
            }
          else
            {
              /* No.. More than one. */

              dbg("[repeats %d more times]\n", count);
            }
        }

      /* Save the new address, value, count, and operation for next time */

      prevaddr  = addr;
      preval    = val;
      count     = 0;
      prevwrite = iswrite;

      /* Show the new regisgter access */

      lpc17_printreg(addr, val, iswrite);
    }
}
#endif

/*******************************************************************************
 * Name: lpc17_getreg
 *
 * Description:
 *   Get the contents of an LPC17xx register
 *
 *******************************************************************************/

#ifdef CONFIG_LPC17_ENET_REGDEBUG
static uint32_t lpc17_getreg(uint32_t addr)
{
  /* Read the value from the register */

  uint32_t val = getreg32(addr);

  /* Check if we need to print this value */

  lpc17_checkreg(addr, val, false);
  return val;
}
#endif

/*******************************************************************************
 * Name: lpc17_putreg
 *
 * Description:
 *   Set the contents of an LPC17xx register to a value
 *
 *******************************************************************************/

#ifdef CONFIG_LPC17_ENET_REGDEBUG
static void lpc17_putreg(uint32_t val, uint32_t addr)
{
  /* Check if we need to print this value */

  lpc17_checkreg(addr, val, true);

  /* Write the value */

  putreg32(val, addr);
}
#endif

/****************************************************************************
 * Function: lpc17_transmit
 *
 * Description:
 *   Start hardware transmission.  Called either from the txdone interrupt
 *   handling or from watchdog based polling.
 *
 * Parameters:
 *   priv  - Reference to the driver state structure
 *
 * Returned Value:
 *   OK on success; a negated errno on failure
 *
 * Assumptions:
 *
 ****************************************************************************/

static int lpc17_transmit(FAR struct lpc17_driver_s *priv)
{
  /* Verify that the hardware is ready to send another packet.  If we get
   * here, then we are committed to sending a packet; Higher level logic
   * must have assured that there is not transmission in progress.
   */

  /* Increment statistics */

  /* Disable Ethernet interrupts */

  /* Send the packet: address=priv->lp_dev.d_buf, length=priv->lp_dev.d_len */

  /* Restore Ethernet interrupts */

  /* Setup the TX timeout watchdog (perhaps restarting the timer) */

  (void)wd_start(priv->lp_txtimeout, LPC17_TXTIMEOUT, lpc17_txtimeout, 1, (uint32_t)priv);
  return OK;
}

/****************************************************************************
 * Function: lpc17_uiptxpoll
 *
 * Description:
 *   The transmitter is available, check if uIP has any outgoing packets ready
 *   to send.  This is a callback from uip_poll().  uip_poll() may be called:
 *
 *   1. When the preceding TX packet send is complete,
 *   2. When the preceding TX packet send timesout and the interface is reset
 *   3. During normal TX polling
 *
 * Parameters:
 *   dev  - Reference to the NuttX driver state structure
 *
 * Returned Value:
 *   OK on success; a negated errno on failure
 *
 * Assumptions:
 *
 ****************************************************************************/

static int lpc17_uiptxpoll(struct uip_driver_s *dev)
{
  FAR struct lpc17_driver_s *priv = (FAR struct lpc17_driver_s *)dev->d_private;

  /* If the polling resulted in data that should be sent out on the network,
   * the field d_len is set to a value > 0.
   */

  if (priv->lp_dev.d_len > 0)
    {
      uip_arp_out(&priv->lp_dev);
      lpc17_transmit(priv);

      /* Check if there is room in the device to hold another packet. If not,
       * return a non-zero value to terminate the poll.
       */
    }

  /* If zero is returned, the polling will continue until all connections have
   * been examined.
   */

  return 0;
}

/****************************************************************************
 * Function: lpc17_receive
 *
 * Description:
 *   An interrupt was received indicating the availability of a new RX packet
 *
 * Parameters:
 *   priv  - Reference to the driver state structure
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

static void lpc17_receive(FAR struct lpc17_driver_s *priv)
{
  do
    {
      /* Check for errors and update statistics */

      /* Check if the packet is a valid size for the uIP buffer configuration */

      /* Copy the data data from the hardware to priv->lp_dev.d_buf.  Set
       * amount of data in priv->lp_dev.d_len
       */

      /* We only accept IP packets of the configured type and ARP packets */

#ifdef CONFIG_NET_IPv6
      if (BUF->type == HTONS(UIP_ETHTYPE_IP6))
#else
      if (BUF->type == HTONS(UIP_ETHTYPE_IP))
#endif
        {
          uip_arp_ipin();
          uip_input(&priv->lp_dev);

          /* If the above function invocation resulted in data that should be
           * sent out on the network, the field  d_len will set to a value > 0.
           */

          if (priv->lp_dev.d_len > 0)
           {
             uip_arp_out(&priv->lp_dev);
             lpc17_transmit(priv);
           }
        }
      else if (BUF->type == htons(UIP_ETHTYPE_ARP))
        {
          uip_arp_arpin(&priv->lp_dev);

          /* If the above function invocation resulted in data that should be
           * sent out on the network, the field  d_len will set to a value > 0.
           */

          if (priv->lp_dev.d_len > 0)
            {
              lpc17_transmit(priv);
            }
        }
    }
  while (1); /* While there are more packets to be processed */
}

/****************************************************************************
 * Function: lpc17_txdone
 *
 * Description:
 *   An interrupt was received indicating that the last TX packet(s) is done
 *
 * Parameters:
 *   priv  - Reference to the driver state structure
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

static void lpc17_txdone(FAR struct lpc17_driver_s *priv)
{
  /* Check for errors and update statistics */

  /* If no further xmits are pending, then cancel the TX timeout */

  wd_cancel(priv->lp_txtimeout);

  /* Then poll uIP for new XMIT data */

  (void)uip_poll(&priv->lp_dev, lpc17_uiptxpoll);
}

/****************************************************************************
 * Function: lpc17_interrupt
 *
 * Description:
 *   Hardware interrupt handler
 *
 * Parameters:
 *   irq     - Number of the IRQ that generated the interrupt
 *   context - Interrupt register state save info (architecture-specific)
 *
 * Returned Value:
 *   OK on success
 *
 * Assumptions:
 *
 ****************************************************************************/

static int lpc17_interrupt(int irq, FAR void *context)
{
  register FAR struct lpc17_driver_s *priv = &g_ethdrvr[0];

  /* Disable Ethernet interrupts */

  /* Get and clear interrupt status bits */

  /* Handle interrupts according to status bit settings */

  /* Check if we received an incoming packet, if so, call lpc17_receive() */

  lpc17_receive(priv);

  /* Check is a packet transmission just completed.  If so, call lpc17_txdone */

  lpc17_txdone(priv);

  /* Enable Ethernet interrupts (perhaps excluding the TX done interrupt if 
   * there are no pending transmissions.
   */

  return OK;
}

/****************************************************************************
 * Function: lpc17_txtimeout
 *
 * Description:
 *   Our TX watchdog timed out.  Called from the timer interrupt handler.
 *   The last TX never completed.  Reset the hardware and start again.
 *
 * Parameters:
 *   argc - The number of available arguments
 *   arg  - The first argument
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

static void lpc17_txtimeout(int argc, uint32_t arg, ...)
{
  FAR struct lpc17_driver_s *priv = (FAR struct lpc17_driver_s *)arg;

  /* Increment statistics and dump debug info */

  /* Then reset the hardware */

  /* Then poll uIP for new XMIT data */

  (void)uip_poll(&priv->lp_dev, lpc17_uiptxpoll);
}

/****************************************************************************
 * Function: lpc17_polltimer
 *
 * Description:
 *   Periodic timer handler.  Called from the timer interrupt handler.
 *
 * Parameters:
 *   argc - The number of available arguments
 *   arg  - The first argument
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

static void lpc17_polltimer(int argc, uint32_t arg, ...)
{
  FAR struct lpc17_driver_s *priv = (FAR struct lpc17_driver_s *)arg;

  /* Check if there is room in the send another TX packet.  We cannot perform
   * the TX poll if he are unable to accept another packet for transmission.
   */

  /* If so, update TCP timing states and poll uIP for new XMIT data. Hmmm..
   * might be bug here.  Does this mean if there is a transmit in progress,
   * we will missing TCP time state updates?
   */

  (void)uip_timer(&priv->lp_dev, lpc17_uiptxpoll, LPC17_POLLHSEC);

  /* Setup the watchdog poll timer again */

  (void)wd_start(priv->lp_txpoll, LPC17_WDDELAY, lpc17_polltimer, 1, arg);
}

/****************************************************************************
 * Function: lpc17_ifup
 *
 * Description:
 *   NuttX Callback: Bring up the Ethernet interface when an IP address is
 *   provided 
 *
 * Parameters:
 *   dev  - Reference to the NuttX driver state structure
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

static int lpc17_ifup(struct uip_driver_s *dev)
{
  FAR struct lpc17_driver_s *priv = (FAR struct lpc17_driver_s *)dev->d_private;
  uint32_t regval;
  int ret;

  ndbg("Bringing up: %d.%d.%d.%d\n",
       dev->d_ipaddr & 0xff, (dev->d_ipaddr >> 8) & 0xff,
       (dev->d_ipaddr >> 16) & 0xff, dev->d_ipaddr >> 24 );

  /* Reset the Ethernet controller (again) */

  lpc17_ethreset(priv);

  /* Initialize the PHY and wait for the link to be established */

  ret = lpc17_phyinit(priv);
  if (ret != 0)
    {
      ndbg("lpc17_phyinit failed: %d\n", ret);
      return ret;
    }

  /* Configure the MAC station address */

  regval = (uint32_t)priv->lp_dev.d_mac.ether_addr_octet[1] << 8 |
           (uint32_t)priv->lp_dev.d_mac.ether_addr_octet[0];
  lpc17_putreg(regval, LPC17_ETH_SA0);

  regval = (uint32_t)priv->lp_dev.d_mac.ether_addr_octet[3] << 8 |
           (uint32_t)priv->lp_dev.d_mac.ether_addr_octet[2];
  lpc17_putreg(regval, LPC17_ETH_SA1);

  regval = (uint32_t)priv->lp_dev.d_mac.ether_addr_octet[5] << 8 |
           (uint32_t)priv->lp_dev.d_mac.ether_addr_octet[4];
  lpc17_putreg(regval, LPC17_ETH_SA2);

  /* Initialize Ethernet interface for the PHY setup */

  lpc17_macmode(priv->lp_mode);

  /* Initialize EMAC DMA memory */

  /* Set up RX filter and configure to accept broadcast address and perfect
   * station address matches.
   */

  /* Set and activate a timer process */

  (void)wd_start(priv->lp_txpoll, LPC17_WDDELAY, lpc17_polltimer, 1, (uint32_t)priv);

  /* Enable the Ethernet interrupt */

  priv->lp_ifup = true;
  up_enable_irq(LPC17_IRQ_ETH);
  return OK;
}

/****************************************************************************
 * Function: lpc17_ifdown
 *
 * Description:
 *   NuttX Callback: Stop the interface.
 *
 * Parameters:
 *   dev  - Reference to the NuttX driver state structure
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

static int lpc17_ifdown(struct uip_driver_s *dev)
{
  FAR struct lpc17_driver_s *priv = (FAR struct lpc17_driver_s *)dev->d_private;
  irqstate_t flags;

  /* Disable the Ethernet interrupt */

  flags = irqsave();
  up_disable_irq(LPC17_IRQ_ETH);

  /* Cancel the TX poll timer and TX timeout timers */

  wd_cancel(priv->lp_txpoll);
  wd_cancel(priv->lp_txtimeout);

  /* Reset the device */

  lpc17_ethreset(priv);
  priv->lp_ifup = false;
  irqrestore(flags);
  return OK;
}

/****************************************************************************
 * Function: lpc17_txavail
 *
 * Description:
 *   Driver callback invoked when new TX data is available.  This is a 
 *   stimulus perform an out-of-cycle poll and, thereby, reduce the TX
 *   latency.
 *
 * Parameters:
 *   dev  - Reference to the NuttX driver state structure
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *   Called in normal user mode
 *
 ****************************************************************************/

static int lpc17_txavail(struct uip_driver_s *dev)
{
  FAR struct lpc17_driver_s *priv = (FAR struct lpc17_driver_s *)dev->d_private;
  irqstate_t flags;

  flags = irqsave();

  /* Ignore the notification if the interface is not yet up */

  if (priv->lp_ifup)
    {

      /* Check if there is room in the hardware to hold another outgoing packet. */

      /* If so, then poll uIP for new XMIT data */

      (void)uip_poll(&priv->lp_dev, lpc17_uiptxpoll);
    }

  irqrestore(flags);
  return OK;
}

/****************************************************************************
 * Function: lpc17_addmac
 *
 * Description:
 *   NuttX Callback: Add the specified MAC address to the hardware multicast
 *   address filtering
 *
 * Parameters:
 *   dev  - Reference to the NuttX driver state structure
 *   mac  - The MAC address to be added 
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

#ifdef CONFIG_NET_IGMP
static int lpc17_addmac(struct uip_driver_s *dev, FAR const uint8_t *mac)
{
  FAR struct lpc17_driver_s *priv = (FAR struct lpc17_driver_s *)dev->d_private;

  /* Add the MAC address to the hardware multicast routing table */

  return OK;
}
#endif

/****************************************************************************
 * Function: lpc17_rmmac
 *
 * Description:
 *   NuttX Callback: Remove the specified MAC address from the hardware multicast
 *   address filtering
 *
 * Parameters:
 *   dev  - Reference to the NuttX driver state structure
 *   mac  - The MAC address to be removed 
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

#ifdef CONFIG_NET_IGMP
static int lpc17_rmmac(struct uip_driver_s *dev, FAR const uint8_t *mac)
{
  FAR struct lpc17_driver_s *priv = (FAR struct lpc17_driver_s *)dev->d_private;

  /* Add the MAC address to the hardware multicast routing table */

  return OK;
}
#endif

/*******************************************************************************
 * Name: lpc17_showpins
 *
 * Description:
 *   Dump GPIO registers
 *
 * Parameters:
 *   None 
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 *******************************************************************************/

#ifdef CONFIG_LPC17_ENET_REGDEBUG
static void lpc17_showpins(void)
{
  lpc17_dumpgpio(GPIO_PORT0|GPIO_PIN0, "P0[1-15]");
  lpc17_dumpgpio(GPIO_PORT0|GPIO_PIN16, "P0[16-31]");
}
#endif

/*******************************************************************************
 * Name: lpc17_showmii
 *
 * Description:
 *   Dump PHY MII registers
 *
 * Parameters:
 *   phyaddr - The device address where the PHY was discovered
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 *******************************************************************************/

#if defined(CONFIG_LPC17_ENET_REGDEBUG) && defined(LPC17_HAVE_PHY)
static void lpc17_showmii(uint8_t phyaddr, const char *msg)
{
  dbg("PHY " LPC17_PHYNAME ": %s\n", msg);
  dbg("  MCR:       %04x\n", lpc17_getreg(MII_MCR));
  dbg("  MSR:       %04x\n", lpc17_getreg(MII_MSR));
  dbg("  ADVERTISE: %04x\n", lpc17_getreg(MII_ADVERTISE));
  dbg("  LPA:       %04x\n", lpc17_getreg(MII_LPA));
  dbg("  EXPANSION: %04x\n", lpc17_getreg(MII_EXPANSION));
#ifdef CONFIG_PHY_KS8721
  dbg("  10BTCR:    %04x\n", lpc17_getreg(MII_KS8721_10BTCR));
#endif
}
#endif

/****************************************************************************
 * Function: lpc17_phywrite
 *
 * Description:
 *   Write a value to an MII PHY register
 *
 * Parameters:
 *   phyaddr - The device address where the PHY was discovered
 *   regaddr - The address of the PHY register to be written
 *   phydata - The data to write to the PHY register 
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

#ifdef LPC17_HAVE_PHY
static void lpc17_phywrite(uint8_t phyaddr, uint8_t regaddr, uint16_t phydata)
{
  uint32_t regval;

  /* Set PHY address and PHY register address */

  regval = ((uint32_t)phyaddr << ETH_MADR_PHYADDR_SHIFT) |
           ((uint32_t)regaddr << ETH_MADR_REGADDR_SHIFT);
  lpc17_putreg(regval, LPC17_ETH_MADR);

  /* Set up to write */

  lpc17_putreg(ETH_MCMD_WRITE, LPC17_ETH_MCMD);

  /* Write the register data to the PHY */

  lpc17_putreg((uint32_t)phydata, LPC17_ETH_MWTD);

  /* Wait for the PHY command to complete */

  while ((lpc17_getreg(LPC17_ETH_MIND) & ETH_MIND_BUSY) != 0);
}
#endif

/****************************************************************************
 * Function: lpc17_phyread
 *
 * Description:
 *   Read a value from an MII PHY register
 *
 * Parameters:
 *   phyaddr - The device address where the PHY was discovered
 *   regaddr - The address of the PHY register to be written
 *
 * Returned Value:
 *   Data read from the PHY register
 *
 * Assumptions:
 *
 ****************************************************************************/

#ifdef LPC17_HAVE_PHY
static uint16_t lpc17_phyread(uint8_t phyaddr, uint8_t regaddr)
{
  uint32_t regval;

  lpc17_putreg(0, LPC17_ETH_MCMD);

  /* Set PHY address and PHY register address */

  regval = ((uint32_t)phyaddr << ETH_MADR_PHYADDR_SHIFT) |
           ((uint32_t)regaddr << ETH_MADR_REGADDR_SHIFT);
  lpc17_putreg(regval, LPC17_ETH_MADR);

  /* Set up to read */

  lpc17_putreg(ETH_MCMD_READ, LPC17_ETH_MCMD);

  /* Wait for the PHY command to complete */

  while ((lpc17_getreg(LPC17_ETH_MIND) & (ETH_MIND_BUSY|ETH_MIND_NVALID)) != 0);
  lpc17_putreg(0, LPC17_ETH_MCMD);

  /* Return the PHY register data */

  return (uint16_t)(lpc17_getreg(LPC17_ETH_MRDD) & ETH_MRDD_MASK);
}
#endif

/****************************************************************************
 * Function: lpc17_phyreset
 *
 * Description:
 *   Reset the PHY
 *
 * Parameters:
 *   phyaddr - The device address where the PHY was discovered
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

#ifdef LPC17_HAVE_PHY
static inline int lpc17_phyreset(uint8_t phyaddr)
{
  int32_t timeout;
  uint16_t phyreg;

  /* Reset the PHY.  Needs a minimal 50uS delay after reset. */

  lpc17_phywrite(phyaddr, MII_MCR, MII_MCR_RESET);

  /* Wait for a minimum of 50uS no matter what */

  up_udelay(50);

  /* The MCR reset bit is self-clearing.  Wait for it to be clear
   * indicating that the reset is complete.
   */

  for (timeout = MII_BIG_TIMEOUT; timeout > 0; timeout--)
    {
      phyreg = lpc17_phyread(phyaddr, MII_MCR);
      if ((phyreg & MII_MCR_RESET) == 0)
        {
          return OK;
        }
    }

  ndbg("Reset failed. MCR: %04x\n", phyreg);
  return -ETIMEDOUT;
}
#endif

/****************************************************************************
 * Function: lpc17_phyautoneg
 *
 * Description:
 *   Enable auto-negotiation.
 *
 * Parameters:
 *   phyaddr - The device address where the PHY was discovered
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *   The adverisement regiser has already been configured.
 *
 ****************************************************************************/

#if defined(LPC17_HAVE_PHY) && defined(CONFIG_PHY_AUTONEG)
static inline int lpc17_phyautoneg(uint8_t phyaddr)
{
  int32_t timeout;
  uint16_t phyreg;

  /* Start auto-negotiation */

  lpc17_phywrite(phyaddr, MII_MCR, MII_MCR_ANENABLE | MII_MCR_ANRESTART);

  /* Wait for autonegotiation to complete */

  for (timeout = MII_BIG_TIMEOUT; timeout > 0; timeout--)
    {
      /* Check if auto-negotiation has completed */

      phyreg = lpc17_phyread(phyaddr, MII_MSR);
      if ((phyreg & (MII_MSR_LINKSTATUS | MII_MSR_ANEGCOMPLETE)) == 
          (MII_MSR_LINKSTATUS | MII_MSR_ANEGCOMPLETE))
        {
          /* Yes.. return success */

          return OK;
        }
    }

  ndbg("Auto-negotiation failed. MSR: %04x\n", phyreg);
  return -ETIMEDOUT;
}
#endif

/****************************************************************************
 * Function: lpc17_phymode
 *
 * Description:
 *   Set the PHY to operate at a selected speed/duplex mode.
 *
 * Parameters:
 *   phyaddr - The device address where the PHY was discovered
 *   mode - speed/duplex mode
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

#ifdef LPC17_HAVE_PHY
static int lpc17_phymode(uint8_t phyaddr, uint8_t mode)
{
  int32_t timeout;
  uint16_t phyreg;

  /* Disable auto-negotiation and set fixed Speed and Duplex settings */

  phyreg = 0;
  if ((mode & LPC17_SPEED_MASK) ==  LPC17_SPEED_100)
    {
      phyreg = MII_MCR_SPEED100;
    }

  if ((mode & LPC17_DUPLEX_MASK) == LPC17_DUPLEX_FULL)
    {
      phyreg |= MII_MCR_FULLDPLX;
    }

  lpc17_phywrite(phyaddr, MII_MCR, phyreg);

  /* Then wait for the link to be established */

  for (timeout = MII_BIG_TIMEOUT; timeout > 0; timeout--)
    {
      phyreg = lpc17_phyread(phyaddr, MII_MSR);
      if (phyreg & MII_MSR_LINKSTATUS)
        {
          /* Yes.. return success */

          return OK;
        }
    }

  ndbg("Link failed. MSR: %04x\n", phyreg);
  return -ETIMEDOUT;
}
#endif

/****************************************************************************
 * Function: lpc17_phyinit
 *
 * Description:
 *   Initialize the PHY
 *
 * Parameters:
 *   priv - Pointer to EMAC device driver structure 
 *
 * Returned Value:
 *   None directory.
 *   As a side-effect, it will initialize priv->lp_phyaddr and
 *   priv->lp_phymode.
 *
 * Assumptions:
 *
 ****************************************************************************/

#ifdef LPC17_HAVE_PHY
static inline int lpc17_phyinit(struct lpc17_driver_s *priv)
{
  unsigned int phyaddr;
  uint16_t phyreg;
  uint32_t regval;
  int ret;

  /* MII configuration: host clocked divided per board.h, no suppress
   * preambl,e no scan increment.
   */

  lpc17_putreg(ETH_MCFG_CLKSEL_DIV, LPC17_ETH_MCFG);
  lpc17_putreg(0, LPC17_ETH_MCMD);

  /* Enter RMII mode and select 100 MBPS support */

  lpc17_putreg(ETH_CMD_RMII, LPC17_ETH_CMD);
  lpc17_putreg(ETH_SUPP_SPEED, LPC17_ETH_SUPP);

  /* Find PHY Address.  Because controller has a pull-up and the
   * PHY have pull-down resistors on RXD lines some times the PHY
   * latches different at different addresses.
   */

  for (phyaddr = 1; phyaddr < 32; phyaddr++)
    {
       /* Check if we can see the selected device ID at this
        * PHY address.
        */

       phyreg = (unsigned int)lpc17_phyread(phyaddr, MII_PHYID1);
       if (phyreg == LPC17_PHYID1)
        {
          phyreg = lpc17_phyread(phyaddr, MII_PHYID2);
          if (phyreg  == LPC17_PHYID2)
            {
              break;
            }
        }
    }
  nvdbg("phyaddr: %d\n", phyaddr);

  /* Check if the PHY device address was found */

  if (phyaddr > 31)
    {
      /* Failed to find PHY at any location */

      return -ENODEV;
    }

  /* Save the discovered PHY device address */

  priv->lp_phyaddr = phyaddr;

  /* Reset the PHY */

  ret = lpc17_phyreset(phyaddr);
  if (ret < 0)
    {
      return ret;
    }
  lpc17_showmii(phyaddr, "After reset");

  /* Check for preamble suppression support */

  phyreg = lpc17_phyread(phyaddr, MII_MSR);
  if ((phyreg & MII_MSR_MFRAMESUPPRESS) != 0)
    {
      /* The PHY supports preamble suppression */

      regval  = lpc17_getreg(LPC17_ETH_MCFG);
      regval |= ETH_MCFG_SUPPRE;
      lpc17_putreg(regval, LPC17_ETH_MCFG);
    }

  /* Are we configured to do auto-negotiation? */

#ifdef CONFIG_PHY_AUTONEG
  /* Setup the Auto-negotiation advertisement: 100 or 10, and HD or FD */

  lpc17_phywrite(phyaddr, MII_ADVERTISE, 
                 (MII_ADVERTISE_100BASETXFULL | MII_ADVERTISE_100BASETXHALF |
                  MII_ADVERTISE_10BASETXFULL  | MII_ADVERTISE_10BASETXHALF  |
                  MII_ADVERTISE_CSMA));

  /* Then perform the auto-negotiation */

  ret = lpc17_phyautoneg(phyaddr);
  if (ret < 0)
    {
      return ret;
    }
#else
  /* Set up the fixed PHY configuration */

  ret = lpc17_phymode(phyaddr, LPC17_MODE_DEFLT);
  if (ret < 0)
    {
      return ret;
    }
#endif

  /* The link is established */

  lpc17_showmii(phyaddr, "After link established");

  /* Check configuration */

#ifdef CONFIG_PHY_KS8721
  phyreg = lpc17_phyread(phyaddr, MII_KS8721_10BTCR);

  switch (phyreg & KS8721_10BTCR_MODE_MASK)
    {
      case KS8721_10BTCR_MODE_10BTHD:  /* 10BASE-T half duplex */
        priv->lp_mode = LPC17_10BASET_HD;
        lpc17_putreg(0, LPC17_ETH_SUPP);
        break;
      case KS8721_10BTCR_MODE_100BTHD: /* 100BASE-T half duplex */
        priv->lp_mode = LPC17_100BASET_HD;
        break;
      case KS8721_10BTCR_MODE_10BTFD: /* 10BASE-T full duplex */
        priv->lp_mode = LPC17_10BASET_FD;
        lpc17_putreg(0, LPC17_ETH_SUPP);
        break;
      case KS8721_10BTCR_MODE_100BTFD: /* 100BASE-T full duplex */
        priv->lp_mode = LPC17_100BASET_FD;
        break;
      default:
        dbg("Unrecognized mode: %04x\n", phyreg);
        return -ENODEV;
    }
#else
#  warning "PHY Unknown: speed and duplex are bogus"
#endif

  ndbg("%dBase-T %s duplex\n",
       priv->lp_mode & LPC17_SPEED_MASK ==  LPC17_SPEED_100 ? 100 : 10,
       priv->lp_mode & LPC17_DUPLEX_MASK == LPC17_DUPLEX_FULL ?"full" : "half");

  /* Disable auto-configuration.  Set the fixed speed/duplex mode.
   * (probably more than little redundant).
   */

  ret = lpc17_phymode(phyaddr, LPC17_MODE_DEFLT);
  lpc17_showmii(phyaddr, "After final configuration");
  return ret;
}
#endif

/****************************************************************************
 * Function: lpc17_macmode
 *
 * Description:
 *   Set the MAC to operate at a selected speed/duplex mode.
 *
 * Parameters:
 *   mode - speed/duplex mode
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

#ifdef LPC17_HAVE_PHY
static void lpc17_macmode(uint8_t mode)
{
  uint32_t regval;

  /* Set up for full or half duplex operation */

  if ((mode & LPC17_DUPLEX_MASK) == LPC17_DUPLEX_FULL)
    {
      /* Set the back-to-back inter-packet gap */
 
      lpc17_putreg(21, LPC17_ETH_IPGT);

      /* Set MAC to operate in full duplex mode with CRC and Pad enabled */

      regval = lpc17_getreg(LPC17_ETH_MAC2);
      regval |= (ETH_MAC2_FD | ETH_MAC2_CRCEN | ETH_MAC2_PADCRCEN);
      lpc17_putreg(regval, LPC17_ETH_MAC2);

      /* Select full duplex operation for ethernet controller */

      regval = lpc17_getreg(LPC17_ETH_CMD);
      regval |= (ETH_CMD_FD | ETH_CMD_RMII | ETH_CMD_PRFRAME);
      lpc17_putreg(regval, LPC17_ETH_CMD);
    }
  else
    {
      /* Set the back-to-back inter-packet gap */
 
      lpc17_putreg(18, LPC17_ETH_IPGT);

      /* Set MAC to operate in half duplex mode with CRC and Pad enabled */

      regval = lpc17_getreg(LPC17_ETH_MAC2);
      regval &= ~ETH_MAC2_FD;
      regval |= (ETH_MAC2_CRCEN | ETH_MAC2_PADCRCEN);
      lpc17_putreg(regval, LPC17_ETH_MAC2);

      /* Select half duplex operation for ethernet controller */

      regval = lpc17_getreg(LPC17_ETH_CMD);
      regval &= ~ETH_CMD_FD;
      regval |= (ETH_CMD_RMII | ETH_CMD_PRFRAME);
      lpc17_putreg(regval, LPC17_ETH_CMD);
    }

  /* This is currently done in lpc17_phyinit().  That doesn't
   * seem like the right place. It should be done here.
   */

#if 0
  regval = lpc17_getreg(LPC17_ETH_SUPP);
  if ((mode & LPC17_SPEED_MASK) == LPC17_SPEED_100)
    {
      regval |= ETH_SUPP_SPEED;
    }
  else
    {
      regval &= ~ETH_SUPP_SPEED;
    }
  lpc17_putreg(regval, LPC17_ETH_SUPP);
#endif
}
#endif

/****************************************************************************
 * Function: lpc17_ethreset
 *
 * Description:
 *   Configure and reset the Ethernet module, leaving it in a disabled state.
 *
 * Parameters:
 *   priv   - Reference to the driver state structure
 *
 * Returned Value:
 *   OK on success; a negated errno on failure
 *
 * Assumptions:
 *
 ****************************************************************************/

static void lpc17_ethreset(struct lpc17_driver_s *priv)
{
  irqstate_t flags;

  /* Reset the MAC */

  flags = irqsave();

  /* Put the MAC into the reset state */

  lpc17_putreg((ETH_MAC1_TXRST    | ETH_MAC1_MCSTXRST |ETH_MAC1_RXRST |
                ETH_MAC1_MCSRXRST | ETH_MAC1_SIMRST   | ETH_MAC1_SOFTRST),
               LPC17_ETH_MAC1);

  /* Disable RX/RX, clear modes, reset all control registers */

  lpc17_putreg((ETH_CMD_REGRST | ETH_CMD_TXRST | ETH_CMD_RXRST),
               LPC17_ETH_CMD);

  /* Take the MAC out of the reset state */

  up_udelay(50);
  lpc17_putreg(0, LPC17_ETH_MAC1);

  /* The RMII bit must be set on initialization (I'm not sure
   * this needs to be done here but... oh well.
   */

  lpc17_putreg(ETH_CMD_RMII, LPC17_ETH_CMD);

  /* Set other misc configuration-related registers to default values */

  lpc17_putreg(0, LPC17_ETH_MAC2);
  lpc17_putreg(0, LPC17_ETH_SUPP);
  lpc17_putreg(0, LPC17_ETH_TEST);

  lpc17_putreg(18, LPC17_ETH_IPGR);
  lpc17_putreg(((15 << ETH_CLRT_RMAX_SHIFT) | (55 << ETH_CLRT_COLWIN_SHIFT)),
               LPC17_ETH_CLRT);
  lpc17_putreg(0x0600, LPC17_ETH_MAXF);

  /* Disable all Ethernet controller interrupts */

  lpc17_putreg(0, LPC17_ETH_INTEN);

  /* Clear any pending interrupts (shouldn't be any) */

  lpc17_putreg(0xffffffff, LPC17_ETH_INTCLR);
  irqrestore(flags);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Function: lpc17_ethinitialize
 *
 * Description:
 *   Initialize one Ethernet controller and driver structure.
 *
 * Parameters:
 *   intf - Selects the interface to be initialized.
 *
 * Returned Value:
 *   OK on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

#if LPC17_NETHCONTROLLERS > 1
int lpc17_ethinitialize(int intf)
#else
static inline int lpc17_ethinitialize(int intf)
#endif
{
  struct lpc17_driver_s *priv;
  uint32_t regval;
  int ret;
  int i;

  DEBUGASSERT(inf < LPC17_NETHCONTROLLERS);
  priv = &g_ethdrvr[intf];

  /* Turn on the ethernet MAC clock */

  regval  = lpc17_getreg(LPC17_SYSCON_PCONP);
  regval |= SYSCON_PCONP_PCENET;
  lpc17_putreg(regval, LPC17_SYSCON_PCONP);

  /* Configure all GPIO pins needed by ENET */

  for (i = 0; i < GPIO_NENET_PINS; i++)
    {
      (void)lpc17_configgpio(g_enetpins[i]);
    }
  lpc17_showpins();

  /* Initialize the driver structure */

  memset(priv, 0, sizeof(struct lpc17_driver_s));
  priv->lp_dev.d_ifup    = lpc17_ifup;    /* I/F down callback */
  priv->lp_dev.d_ifdown  = lpc17_ifdown;  /* I/F up (new IP address) callback */
  priv->lp_dev.d_txavail = lpc17_txavail; /* New TX data callback */
#ifdef CONFIG_NET_IGMP
  priv->lp_dev.d_addmac  = lpc17_addmac;  /* Add multicast MAC address */
  priv->lp_dev.d_rmmac   = lpc17_rmmac;   /* Remove multicast MAC address */
#endif
  priv->lp_dev.d_private = (void*)priv;   /* Used to recover private state from dev */

#if LM3S_NETHCONTROLLERS > 1
# error "A mechanism to associate base address an IRQ with an interface is needed"
  priv->lp_base          = ??;            /* Ethernet controller base address */
  priv->lp_irq           = ??;            /* Ethernet controller IRQ number */
#endif

  /* Create a watchdog for timing polling for and timing of transmisstions */

  priv->lp_txpoll        = wd_create();   /* Create periodic poll timer */
  priv->lp_txtimeout     = wd_create();   /* Create TX timeout timer */

  /* Reset the Ethernet controller and leave in the ifdown statue.  The
   * Ethernet controller will be properly re-initialized each time
   * lpc17_ifup() is called.
   */

  lpc17_ifdown(&priv->lp_dev);

  /* Attach the IRQ to the driver */

#if LM3S_NETHCONTROLLERS > 1
  ret = irq_attach(priv->irq, lpc17_interrupt);
#else
  ret = irq_attach(LPC17_IRQ_ETH, lpc17_interrupt);
#endif
  if (ret != 0)
    {
      /* We could not attach the ISR to the the interrupt */

      return -EAGAIN;
    }

  /* Register the device with the OS so that socket IOCTLs can be performed */

  (void)netdev_register(&priv->lp_dev);
  return OK;
}

/****************************************************************************
 * Name: up_netinitialize
 *
 * Description:
 *   Initialize the first network interface.  If there are more than one
 *   interface in the chip, then board-specific logic will have to provide
 *   this function to determine which, if any, Ethernet controllers should
 *   be initialized.
 *
 ****************************************************************************/

#if LPC17_NETHCONTROLLERS == 1
void up_netinitialize(void)
{
  (void)lpc17_ethinitialize(0);
}
#endif
#endif /* LPC17_NETHCONTROLLERS > 0 */
#endif /* CONFIG_NET && CONFIG_LPC17_ETHERNET */