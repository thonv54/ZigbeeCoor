
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
int8u HC_PjoinStatus_Task(void);
int8u HC_ZclClusterCmdRequest_Task(int8u *data);
int8u HC_ZclGlobalCmdRequest_Task(int8u *data);
int8u HC_ZdoCmdRequest_Task(int8u *data);
int8u HC_ResetDevice_Task(int8u *data);
int8u HC_GetDeviceMCVersion_Task(int8u *data);


#endif










