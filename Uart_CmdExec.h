
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

#ifndef __UART_CMDEXEC_H__
#define __UART_CMDEXEC_H__

     






//---------------------------------Protocol--------------------------------------//

int8u HC_UartConectionError_Task(int8u *data);
int8u HC_FormNetwork_Task(int8u *data);
int8u HC_GetNetworkInfo_Task(void);
int8u HC_NetworkPjoin_Task(int8u *data);
int8u HC_LeaveNetwork_Task(int8u *data);
int8u HC_PjoinStatus_Task(void);
int8u HC_GetMacAddr_Task(int8u *data);
int8u HC_GetActiveEndpoint_Task(int8u *data);
int8u HC_GetSimpleDesCription_Task(int8u *data);
int8u HC_GetNodeDescription_Task(int8u *data);
int8u HC_GetDeviceAttributeInfo_Task(int8u *data);
int8u HC_DeviceControl_Task(int8u *data);
int8u HC_WriteDeviceAtribute_Task(int8u *data);
int8u HC_ResetDevice_Task(int8u *data);
int8u HC_GetDeviceMCVersion_Task(int8u *data);
int8u HC_BindUnbindDevice_Task(int8u *data);

#endif










