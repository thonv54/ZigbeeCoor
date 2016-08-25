//

// This callback file is created for your convenience. You may add application
// code to this file. If you regenerate this file over a previous version, the
// previous version will be overwritten and any code you have added will be
// lost.

#include "app/framework/include/af.h"
#include "stack/include/ember-types.h"
#include "app/util/common/form-and-join.h"
#include "Uart_Transmitter.h"
#include "Uart_Receiver.h"
#include "Uart_CmdExec.h"
#include "TaskUtility.h"



typedef enum{
    Startup = 0,
    Idle     = 1,
}RunningStep_enum;

unsigned char RunningStep = Startup;




//---------------------------------------------------------------------------//





// Event control struct declaration
EmberEventControl UartSendEventControl;
EmberEventControl UartGetEventControl;
EmberEventControl StartupFormNwkEventControl;

// Event function forward declaration
void UartSendEventFunction(void);
void UartGetEventFunction(void);
void StartupFormNwkEventFunction(void);

// Event function stub
void UartSendEventFunction(void) { 
    emberEventControlSetDelayMS(UartSendEventControl,100);
    UartSendCommand();
}
void UartGetEventFunction(void) { 
    emberEventControlSetActive(UartGetEventControl);
    UartGetCommand();
}

void StartupFormNwkEventFunction(void){

    EmberNodeType CurrentNodeType;
    EmberNetworkStatus NetworkStatus;
    //Tao mang
    NetworkStatus = emberAfNetworkState();
    emberAfGetNodeType(&CurrentNodeType);
    switch(NetworkStatus){
        case EMBER_NO_NETWORK:
            if(emberFormAndJoinIsScanning() == 0){
                emberAfFindUnusedPanIdAndForm();
            }
            emberEventControlSetActive(StartupFormNwkEventControl);
            
        break;
        case EMBER_JOINED_NETWORK:
            if(CurrentNodeType != EMBER_COORDINATOR){
                emberLeaveNetwork();
                emberEventControlSetActive(StartupFormNwkEventControl);
            }
            else{
                RunningStep = Idle;
                emberEventControlSetInactive(StartupFormNwkEventControl);
                emberEventControlSetActive(UartSendEventControl);
                emberEventControlSetActive(UartGetEventControl);
                
            }
        break;
            default:     
        break;  
    }
}


/** @brief Main Init
 *
 * This function is called from the application's main function. It gives the
 * application a chance to do any initialization required at system startup. Any
 * code that you would normally put into the top of the application's main()
 * routine should be put into this function. This is called before the clusters,
 * plugins, and the network are initialized so some functionality is not yet
 * available.
        Note: No callback in the Application Framework is
 * associated with resource cleanup. If you are implementing your application on
 * a Unix host where resource cleanup is a consideration, we expect that you
 * will use the standard Posix system calls, including the use of atexit() and
 * handlers for signals such as SIGTERM, SIGINT, SIGCHLD, SIGPIPE and so on. If
 * you use the signal() function to register your signal handler, please mind
 * the returned value which may be an Application Framework function. If the
 * return value is non-null, please make sure that you call the returned
 * function from your handler to avoid negating the resource cleanup of the
 * Application Framework itself.
 *
 */
void emberAfMainInitCallback(void)
{
	emberSerialInit(HC_SERIAL, HC_BAUD_RATE, PARITY_NONE, 1);
  emberEventControlSetDelayMS(StartupFormNwkEventControl,1000);
}








