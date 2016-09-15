/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Include file for application
 *
 * Author: Thonv
 *
 * Last Changed By:  $Author: thonv $
 * Revision:         $Revision: 1.0.0.0 $
 * Last Changed:     $Date: 2016-05-20 10:20:00 (Fri, 20 May 2016) $
 *
 ******************************************************************************/

#ifndef __BOARD_H__
#define __BOARD_H__

#include "bspconfig.h"
#include "fl216flash_config.h"

// Define board features

 #define DISABLE_PTI

// #define DISABLE_DCDC

// Enable COM port to use retarget serial configuration
//#define COM_RETARGET_SERIAL
// Enable HW flow control
//#define COM_USART0_HW_FC

// Define onboard external flash locations
#define EXTERNAL_FLASH_MOSI_PORT    FL216_PORT_MOSI
#define EXTERNAL_FLASH_MOSI_PIN     FL216_PIN_MOSI
#define EXTERNAL_FLASH_MISO_PORT    FL216_PORT_MISO
#define EXTERNAL_FLASH_MISO_PIN     FL216_PIN_MISO
#define EXTERNAL_FLASH_CLK_PORT     FL216_PORT_SCLK
#define EXTERNAL_FLASH_CLK_PIN      FL216_PIN_SCLK
#define EXTERNAL_FLASH_CS_PORT      FL216_PORT_CS
#define EXTERNAL_FLASH_CS_PIN       FL216_PIN_CS

// Define nHOST_INT to PD10 -> EXP7 on SLWSTK6000A
#define NHOST_INT_PORT gpioPortD
#define NHOST_INT_PIN  10
// Define nWAKE to PD11 -> EXP9 on SLWSTK6000A
#define NWAKE_PORT     gpioPortD
#define NWAKE_PIN      11

#define halInternalConfigPa() do {                                 \
  RADIO_PAInit_t paInit = RADIO_PA_2P4_INIT;                       \
  RADIO_PA_Init(&paInit);                                          \
} while(0)

// Include common definitions
#include "board/wstk-common.h"

#endif //__BOARD_H__
/** @} END Board Specific Functions */

/** @} END addtogroup */
