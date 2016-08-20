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

#include "app/framework/include/af.h"
#include "ZigbeeCoor.h"
#include "Uart_CmdEnum.h"
#include "typedefs.h"
#include "macro.h"
#include "Uart_CmdExec.h"
#include "Uart_Transmitter.h"
#include "Uart_Receiver.h"

u8_t UartRxDataStep = 0;
u8_t UartRxPacketLength;
u8_t UartRxPacketCheckXor = 0;
u8_t UartRxCommandData[64] = { 0 };
u8_t UartRxCurrentLength = 0;


void UartProcess(u8_t *UartPacketCommand) {
    u8_t seqNumber = UartPacketCommand[0];
    u8_t UartPacketID = UartPacketCommand[1];

    u8_t ErrorCode = UartCmdNormal;
    switch (UartPacketID) {
    case HC_UartConectionError:
        ErrorCode = HC_UartConectionError_Task(UartPacketCommand);
        break;
    case HC_FormNetwork:
        ErrorCode = HC_FormNetwork_Task(UartPacketCommand);
        break;
    case HC_GetNetworkInfo:
        ErrorCode = HC_GetNetworkInfo_Task();
        break;
    case HC_NetworkPjoin:
        ErrorCode = HC_NetworkPjoin_Task(UartPacketCommand);
        break;
    case HC_PjoinStatus:
        ErrorCode = HC_PjoinStatus_Task();
        break;
    case HC_ZclClusterCmdRequest:
        ErrorCode = HC_ZclClusterCmdRequest_Task(UartPacketCommand);
        break;
    case HC_ZclGlobalCmdRequest:
        ErrorCode = HC_ZclGlobalCmdRequest_Task(UartPacketCommand);
        break;
    case HC_ZdoCmdRequest:
        ErrorCode = HC_ZdoCmdRequest_Task(UartPacketCommand);
        break;
    case HC_ResetDevice:
        ErrorCode = HC_ResetDevice_Task(UartPacketCommand);
        break;
    case HC_GetDeviceMCVersion:
        ErrorCode = HC_GetDeviceMCVersion_Task(UartPacketCommand);
        break;
    default:
        ErrorCode = CmdIdNotAvaiable;
        break;
    }
    if (ErrorCode != UartCmdNormal) {
#ifdef Debug
        emberSerialPrintf(APP_SERIAL,"		Uart Error Code: %u \n\r", ErrorCode);
        emberSerialPrintf(APP_SERIAL,"		Uart Error seqNumber: %u \n\r", seqNumber);
#endif //Debug
    }
}

void UartGetCommand(void) {
    int16u NumberOfByteReCeiver;
    u8_t ReadSerialData;
    u8_t ReadStatus;

    NumberOfByteReCeiver = emberSerialReadAvailable(HC_SERIAL);
    if (NumberOfByteReCeiver > 0) {
        ReadStatus = emberSerialReadByte(HC_SERIAL, &ReadSerialData);

        if (ReadStatus == EMBER_SUCCESS) {

            switch (UartRxDataStep) {
            case 0:
                if (ReadSerialData == 0x4C) {
                    UartRxDataStep = 1;
                }
                else {
                    UartRxDataStep = 0;
                }
                break;
            case 1:
                if (ReadSerialData == 0x4D) {
                    UartRxDataStep = 2;
                }
                else {
                    UartRxDataStep = 0;
                }
                break;
            case 2:
                UartRxPacketLength = ReadSerialData;
                if (UartRxPacketLength > 40) {
                    UartRxDataStep = 0;
                }
                else {
                    UartRxDataStep = 3;
                }
                break;
            case 3:
                if (UartRxCurrentLength < UartRxPacketLength - 1) {
                    UartRxCommandData[UartRxCurrentLength] = ReadSerialData;
                    UartRxCurrentLength++;
                }
                else {
                    u8_t PacketCheckXor;
                    u8_t CheckXor = 0;
                    PacketCheckXor = ReadSerialData;
                    {
                        for (u8_t j = 1; j < UartRxPacketLength - 1; j++) {
                            CheckXor = CheckXor ^ UartRxCommandData[j];
                        }
                    }
                    if (PacketCheckXor == CheckXor) {
                        UartProcess(UartRxCommandData);
                    }
                    else {
                        u8_t ErrorCode = XorError;
                        u8_t seqNumber = UartRxCommandData[0];
#ifdef Debug
                        emberSerialPrintf(APP_SERIAL,"		Uart Error Code: %u \n\r", ErrorCode);
                        emberSerialPrintf(APP_SERIAL,"		Uart Error seqNumber: %u \n\r", seqNumber);
#endif // Debug
                    }
                    UartRxCurrentLength = 0;
                    UartRxDataStep = 0;
                    
                }
                break;
            default:
                break;

            }
        }
    }
}
