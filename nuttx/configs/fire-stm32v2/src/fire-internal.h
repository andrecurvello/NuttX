/************************************************************************************
 * configs/fire-stm32v2/src/fire-internal.h
 * arch/arm/src/board/fire-internal.n
 *
 *   Copyright (C) 2009 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
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
 ************************************************************************************/

#ifndef __CONFIGS_FIRE_STM32V2_SRC_FIRE_INTERNAL_H
#define __CONFIGS_FIRE_STM32V2_SRC_FIRE_INTERNAL_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <nuttx/config.h>
#include <nuttx/compiler.h>
#include <stdint.h>

/************************************************************************************
 * Definitions
 ************************************************************************************/

/* How many SPI modules does this chip support? Most support 2 SPI modules (others
 * may support more -- in such case, the following must be expanded).
 */

#if STM32_NSPI < 1
#  undef CONFIG_STM32_SPI1
#  undef CONFIG_STM32_SPI2
#elif STM32_NSPI < 2
#  undef CONFIG_STM32_SPI2
#endif

/* There is only CAN1 on the M3 Wildfire board */

#if defined(CONFIG_STM32_CAN2)
#  warning "The M3 Wildfire only supports CAN1"
#endif

/* M3 Wildfire GPIOs ****************************************************************/
/* Camera
 *
 * --- ------ -------------- -------------------------------------------------------------------
 * PIN NAME   SIGNAL         NOTES
 * --- ------ -------------- -------------------------------------------------------------------
 *
 * 23  PA0    PA0-C-VSYNC    Camera (P9)
 * 67  PA8    PA8-C-XCLK     Camera (P9)
 * 91  PB5    PB5-C-WRST     Camera (P9)
 * 95  PB8    PB8-C-DO_0     Camera (P9)
 * 96  PB9    PB9-C-DO_1     Camera (P9)
 * 47  PB10   PB10-C-DO_2    Camera (P9)
 * 48  PB11   PB11-C-DO_3    Camera (P9)
 * 51  PB12   PB12-C-DO_4    Camera (P9)
 * 52  PB13   PB13-C-DO_5    Camera (P9)
 * 53  PB14   PB14-C-DO_6    Camera (P9)
 * 54  PB15   PB15-C-DO_7    Camera (P9)
 * 63  PC6    PC6-C-SIO_C    Camera (P9)
 * 64  PC7    PC7-C-SIO_D    Camera (P9)
 * 84  PD3    PD3-C-WEN      Camera (P9)
 * 87  PD6    PD6-C-OE       Camera (P9)
 * 59  PD12   C-LED_EN       Camera (P9)
 * 97  PE0    PE0-C-RRST     Camera (P9)
 * 1   PE2    PE2-C-RCLK     Camera (P9)
 */

/* Bell
 *
 * --- ------ -------------- -------------------------------------------------------------------
 * PIN NAME   SIGNAL         NOTES
 * --- ------ -------------- -------------------------------------------------------------------
 *
 * 3   PE4    PE4-BEEP       LS1 Bell
 */

/* 2.4" TFT + Touchscreen
 *
 * --- ------ -------------- -------------------------------------------------------------------
 * PIN NAME   SIGNAL         NOTES
 * --- ------ -------------- -------------------------------------------------------------------
 *
 * 30  PA5    PA5-SPI1-SCK   2.4" TFT + Touchscreen, 10Mbit ENC28J60, SPI 2M FLASH
 * 31  PA6    PA6-SPI1-MISO  2.4" TFT + Touchscreen, 10Mbit ENC28J60, SPI 2M FLASH
 * 32  PA7    PA7-SPI1-MOSI  2.4" TFT + Touchscreen, 10Mbit ENC28J60, SPI 2M FLASH
 * 92  PB6    PB6-I2C1-SCL   2.4" TFT + Touchscreen, AT24C02
 * 93  PB7    PB7-I2C1-SDA   2.4" TFT + Touchscreen, AT24C02
 * 7   PC13   PD13_LCD_LIGHT 2.4" TFT + Touchscreen
 * 81  PD0    PD0-FSMC_D2    2.4" TFT + Touchscreen
 * 82  PD1    PD1-FSMC_D3    2.4" TFT + Touchscreen
 * 85  PD4    PD4-FSMC_NOE   2.4" TFT + Touchscreen
 * 86  PD5    PD5-FSMC_NWE   2.4" TFT + Touchscreen
 * 88  PD7    PD7-FSMC_NE1   2.4" TFT + Touchscreen
 * 55  PD8    PD8-FSMC_D13   2.4" TFT + Touchscreen
 * 56  PD9    PD9-FSMC_D14   2.4" TFT + Touchscreen
 * 57  PD10   PD10-FSMC_D15  2.4" TFT + Touchscreen
 * 58  PD11   PD11-FSMC_A16  2.4" TFT + Touchscreen
 * 60  PD13   PD13-LCD/LIGHT 2.4" TFT + Touchscreen
 * 61  PD14   PD14-FSMC_D0   2.4" TFT + Touchscreen
 * 62  PD15   PD15-FSMC_D1   2.4" TFT + Touchscreen
 * 98  PE1    PE1-FSMC_NBL1  2.4" TFT + Touchscreen
 * 38  PE7    PE7-FSMC_D4    2.4" TFT + Touchscreen
 * 39  PE8    PE8-FSMC_D5    2.4" TFT + Touchscreen
 * 40  PE9    PE9-FSMC_D6    2.4" TFT + Touchscreen
 * 41  PE10   PE10-FSMC_D7   2.4" TFT + Touchscreen
 * 42  PE11   PE11-FSMC_D8   2.4" TFT + Touchscreen
 * 43  PE12   PE12-FSMC_D9   2.4" TFT + Touchscreen
 * 44  PE13   PE13-FSMC_D10  2.4" TFT + Touchscreen
 * 45  PE14   PE14-FSMC_D11  2.4" TFT + Touchscreen
 * 46  PE15   PE15-FSMC_D12  2.4" TFT + Touchscreen
 */

#define GPIO_LCD_BACKLIGHT (GPIO_OUTPUT|GPIO_CNF_OUTPP|GPIO_MODE_50MHz|\
                             GPIO_OUTPUT_CLEAR|GPIO_PORTD|GPIO_PIN13)

/* LEDs
 *
 * --- ------ -------------- -------------------------------------------------------------------
 * PIN NAME   SIGNAL         NOTES
 * --- ------ -------------- -------------------------------------------------------------------
 *
 * 18  PC3    PC3-LED1       LED1, Active low (pulled high)
 * 33  PC4    PC4-LED2       LED2, Active low (pulled high)
 * 34  PC5    PC5-LED3       LED3, Active low (pulled high)
 */

#define GPIO_LED1       (GPIO_OUTPUT|GPIO_CNF_OUTPP|GPIO_MODE_50MHz|\
                         GPIO_OUTPUT_CLEAR|GPIO_PORTC|GPIO_PIN3)
#define GPIO_LED2       (GPIO_OUTPUT|GPIO_CNF_OUTPP|GPIO_MODE_50MHz|\
                         GPIO_OUTPUT_CLEAR|GPIO_PORTC|GPIO_PIN4)
#define GPIO_LED3       (GPIO_OUTPUT|GPIO_CNF_OUTPP|GPIO_MODE_50MHz|\
                         GPIO_OUTPUT_CLEAR|GPIO_PORTC|GPIO_PIN5)

/* Buttons
 *
 * --- ------ -------------- -------------------------------------------------------------------
 * PIN NAME   SIGNAL         NOTES
 * --- ------ -------------- -------------------------------------------------------------------
 *
 * 35  PB0    PB0-KEY1       KEY1, Low when closed (pulled high if open)
 * 36  PB1    PB1-KEY2       KEY2, Low when closed (pulled high if open)
 */

#define MIN_IRQBUTTON   BUTTON_KEY1
#define MAX_IRQBUTTON   BUTTON_KEY2
#define NUM_IRQBUTTONS  (MAX_IRQBUTTON - MIN_IRQBUTTON + 1)

#define GPIO_BTN_KEY1   (GPIO_INPUT|GPIO_CNF_INFLOAT|GPIO_MODE_INPUT|\
                         GPIO_PORTB|GPIO_PIN0)
#define GPIO_BTN_KEY2   (GPIO_INPUT|GPIO_CNF_INFLOAT|GPIO_MODE_INPUT|\
                         GPIO_PORTB|GPIO_PIN1)

/************************************************************************************
 * Public Types
 ************************************************************************************/

/************************************************************************************
 * Public data
 ************************************************************************************/

#ifndef __ASSEMBLY__

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: stm32_spiinitialize
 *
 * Description:
 *   Called to configure SPI chip select GPIO pins for the M3 Wildfire board.
 *
 ************************************************************************************/

void weak_function stm32_spiinitialize(void);

/************************************************************************************
 * Name: stm32_usbinitialize
 *
 * Description:
 *   Called to setup USB-related GPIO pins for the M3 Wildfire board.
 *
 ************************************************************************************/

void weak_function stm32_usbinitialize(void);

/************************************************************************************
 * Name: stm32_selectlcd
 *
 * Description:
 *   Initialize to the LCD
 *
 ************************************************************************************/

#ifdef CONFIG_STM32_FSMC
void stm32_selectlcd(void);
#endif

#endif /* __ASSEMBLY__ */
#endif /* __CONFIGS_FIRE_STM32V2_SRC_FIRE_INTERNAL_H */
