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


#ifndef __UART_TRANSMITTER_H__
#define __UART_TRANSMITTER_H__
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "af.h"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef enum {
    Zbs_UartError       	      =	  0x00,
    Zbs_NwkInfo                 =	  0x01,
    
    Zbs_PjoinStatus	            =	  0x05,
    Zbs_ZclClusterCmdResponse   =   0x06,
    Zbs_ZclGlobalCmdResponse    =   0x07,
    Zbs_ZdoCmdResponse          =   0x08,
    Zbs_DeviceMCVersion	        =	  0x0F,

}Zbs_CommandID_enum;

//----------0x00-ZigbeeErrorCode----------//
typedef enum {
    UartCmdNormal     = 0,
    XorError          = 1,
    CmdIdNotAvaiable  = 2,
    CmdDataError      = 3,
    ZigBeeBusy        = 4,
}ZigbeeErrorCode_enum;

typedef enum{
    PermitJoin  =   1,
    NotPermitJoin=  0,
}PjoinState_enum;


typedef enum {
    CmdReady,
    CmdBusy,
}uartSendCommandStatus_enum;


typedef struct{
    int8u Length;
    int8u TxSeqNumber;
    int8u CmdId;
    int8u ErrorCode;
    int8u seqNumber;
    int8u CheckXor;
    int8u TimeOut;
}Zbs_UartErrorDataStr;

typedef struct{
    int8u Length;
    int8u TxSeqNumber;
    int8u CmdId;
    int8u ExtendedPanId[8];
    int8u PanId[2];
    int8u Channel;
    int8u CheckXor;
    int8u TimeOut;
}Zbs_NwkInfoDataStr;


typedef struct{
    int8u Length;
    int8u TxSeqNumber;
    int8u CmdId;
    int8u State;
    int8u CheckXor;
    int8u TimeOut;
}Zbs_PjoinStatusDataStr;


typedef struct{
    int8u Length;
    int8u TxSeqNumber;
    int8u CmdId;
    int8u NwkAddr[2];
    int8u Endpoint;
    int8u ClusterId[2];
    int8u CmdPayloadLength;
    int8u* CmdPayload;
    int8u CheckXor;
    int8u TimeOut;
}Zbs_ZclClusterCmdResponseStr;

typedef struct{
    int8u Length;
    int8u TxSeqNumber;
    int8u CmdId;
    int8u NwkAddr[2];
    int8u Endpoint;
    int8u ClusterId[2];
    int8u GeneralCmd;
    int8u CmdPayloadLength;
    int8u* CmdPayload;
    int8u CheckXor;
    int8u TimeOut;
}Zbs_ZclGlobalCmdResponseStr;


typedef struct{
    int8u Length;
    int8u TxSeqNumber;
    int8u CmdId;
    int8u NwkAddr[2];
    int8u ZdoCmd[2];
    int8u CmdPayloadLength;
    int8u* CmdPayload;
    int8u CheckXor;
    int8u TimeOut;
}Zbs_ZdoCmdResponseStr;


//---------------------Define------------------//
#define NORMAL_TIMEOUT	30				      //30ms
#define NormalTimeOut 	0xFF            // timeout off UART message send is NormalTimeOut*MinimumTimeOut
#define MinimumTimeOut  1000			      //MinimumTimeOut for lag value 1s


/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
/**
 * @func   GetLastTxUartCmd
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
int16u GetLastTxUartCmd(void);
/**
 * @func   UartSendCommand
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void UartSendCommand(void);
/**
 * @func   UartSendUartError
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void UartSendUartError(int8u ErrorCode, int8u seqNumber);
/**
 * @func   UartSendNwkInfo
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void UartSendNwkInfo(int8u ExtendedPanId[8], int16u PanId, int8u Channel);

/**
 * @func   UartSendPjoinStatus
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void UartSendPjoinStatus(int8u State);


/**
 * @func   UartSendZdoCmdResponse
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void UartSendZdoCmdResponse(int16u NwkAddr,
                            int16u ZdoCmd,
                            int8u CmdPayloadLength,
                            int8u *CmdPayload);

/**
 * @func   UartSendZclGlobalCmdResponse
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
void UartSendZclGlobalCmdResponse(int16u NwkAddr,
                                   int8u Endpoint,
                                   int16u ClusterId,
                                   int8u GeneralCmd,
                                   int8u CmdPayloadLength,
                                   int8u *CmdPayload);

/**
 * @func   UartSendZclClusterCmdResponse
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
void UartSendZclClusterCmdResponse(int16u NwkAddr,
                                   int8u Endpoint,
                                   int16u ClusterId,
                                   int8u CmdPayloadLength,
                                   int8u *CmdPayload);

#endif






