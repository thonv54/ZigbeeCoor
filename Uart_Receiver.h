
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



#ifndef __UART_RECEIVER_H__
#define __UART_RECEIVER_H__




//-----------------------------------------------------------------------//







//---------------------Protocol------------------//

void UartProcess(unsigned char *UartPacketCommand);

void UartGetCommand(void);








//----------------------END-----------------------//






#endif




