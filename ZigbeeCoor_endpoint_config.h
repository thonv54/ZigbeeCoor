// This file is generated by Ember Desktop.  Please do not edit manually.
//
//







// Enclosing macro to prevent multiple inclusion
#ifndef __AF_ENDPOINT_CONFIG__
#define __AF_ENDPOINT_CONFIG__





// Fixed number of defined endpoints
#define FIXED_ENDPOINT_COUNT (1)




// Generated attributes
#define GENERATED_ATTRIBUTES { \
    { 0x0000, ZCL_INT8U_ATTRIBUTE_TYPE, 1, (ATTRIBUTE_MASK_SINGLETON), { (uint8_t*)0x02 } }, /* 0 / Basic / ZCL version*/\
    { 0x0007, ZCL_ENUM8_ATTRIBUTE_TYPE, 1, (ATTRIBUTE_MASK_SINGLETON), { (uint8_t*)0x00 } }, /* 1 / Basic / power source*/\
    { 0xFFFD, ZCL_INT16U_ATTRIBUTE_TYPE, 2, (0x00), { (uint8_t*)0x0001 } }, /* 2 / Basic / cluster revision*/\
    { 0x0000, ZCL_INT16U_ATTRIBUTE_TYPE, 2, (ATTRIBUTE_MASK_WRITABLE), { (uint8_t*)0x0000 } }, /* 3 / Identify / identify time*/\
    { 0xFFFD, ZCL_INT16U_ATTRIBUTE_TYPE, 2, (0x00), { (uint8_t*)0x0001 } }, /* 4 / Identify / cluster revision*/\
    { 0x0000, ZCL_UTC_TIME_ATTRIBUTE_TYPE, 4, (ATTRIBUTE_MASK_WRITABLE|ATTRIBUTE_MASK_SINGLETON), { NULL } }, /* 5 / Time / time*/\
    { 0x0001, ZCL_BITMAP8_ATTRIBUTE_TYPE, 1, (ATTRIBUTE_MASK_WRITABLE|ATTRIBUTE_MASK_SINGLETON), { (uint8_t*)0x00 } }, /* 6 / Time / time status*/\
    { 0xFFFD, ZCL_INT16U_ATTRIBUTE_TYPE, 2, (0x00), { (uint8_t*)0x0001 } }, /* 7 / Time / cluster revision*/\
  }


// Cluster function static arrays
#define GENERATED_FUNCTION_ARRAYS \
PGM EmberAfGenericClusterFunction emberAfFuncArrayIdentifyClusterServer[] = { (EmberAfGenericClusterFunction)emberAfIdentifyClusterServerInitCallback,(EmberAfGenericClusterFunction)emberAfIdentifyClusterServerAttributeChangedCallback}; \
PGM EmberAfGenericClusterFunction emberAfFuncArrayTimeClusterServer[] = { (EmberAfGenericClusterFunction)emberAfTimeClusterServerInitCallback}; \
PGM EmberAfGenericClusterFunction emberAfFuncArraySimpleMeteringClusterClient[] = { (EmberAfGenericClusterFunction)emberAfSimpleMeteringClusterClientDefaultResponseCallback}; \


// Clusters defitions
#define GENERATED_CLUSTERS { \
    { 0x0000, (EmberAfAttributeMetadata*)&(generatedAttributes[0]), 3, 2, (CLUSTER_MASK_SERVER), NULL,  },    \
    { 0x0003, (EmberAfAttributeMetadata*)&(generatedAttributes[3]), 0, 0, (CLUSTER_MASK_CLIENT), NULL,  },    \
    { 0x0003, (EmberAfAttributeMetadata*)&(generatedAttributes[3]), 2, 4, (CLUSTER_MASK_SERVER| CLUSTER_MASK_INIT_FUNCTION| CLUSTER_MASK_ATTRIBUTE_CHANGED_FUNCTION), emberAfFuncArrayIdentifyClusterServer, },    \
    { 0x0004, (EmberAfAttributeMetadata*)&(generatedAttributes[5]), 0, 0, (CLUSTER_MASK_CLIENT), NULL,  },    \
    { 0x0005, (EmberAfAttributeMetadata*)&(generatedAttributes[5]), 0, 0, (CLUSTER_MASK_CLIENT), NULL,  },    \
    { 0x0006, (EmberAfAttributeMetadata*)&(generatedAttributes[5]), 0, 0, (CLUSTER_MASK_CLIENT), NULL,  },    \
    { 0x0008, (EmberAfAttributeMetadata*)&(generatedAttributes[5]), 0, 0, (CLUSTER_MASK_CLIENT), NULL,  },    \
    { 0x000A, (EmberAfAttributeMetadata*)&(generatedAttributes[5]), 3, 2, (CLUSTER_MASK_SERVER| CLUSTER_MASK_INIT_FUNCTION), emberAfFuncArrayTimeClusterServer, },    \
    { 0x001A, (EmberAfAttributeMetadata*)&(generatedAttributes[8]), 0, 0, (CLUSTER_MASK_CLIENT), NULL,  },    \
    { 0x0100, (EmberAfAttributeMetadata*)&(generatedAttributes[8]), 0, 0, (CLUSTER_MASK_CLIENT), NULL,  },    \
    { 0x0101, (EmberAfAttributeMetadata*)&(generatedAttributes[8]), 0, 0, (CLUSTER_MASK_CLIENT), NULL,  },    \
    { 0x0300, (EmberAfAttributeMetadata*)&(generatedAttributes[8]), 0, 0, (CLUSTER_MASK_CLIENT), NULL,  },    \
    { 0x0500, (EmberAfAttributeMetadata*)&(generatedAttributes[8]), 0, 0, (CLUSTER_MASK_CLIENT), NULL,  },    \
    { 0x0702, (EmberAfAttributeMetadata*)&(generatedAttributes[8]), 0, 0, (CLUSTER_MASK_CLIENT| CLUSTER_MASK_DEFAULT_RESPONSE_FUNCTION), emberAfFuncArraySimpleMeteringClusterClient, },    \
    { 0x0B01, (EmberAfAttributeMetadata*)&(generatedAttributes[8]), 0, 0, (CLUSTER_MASK_CLIENT), NULL,  },    \
    { 0x0B03, (EmberAfAttributeMetadata*)&(generatedAttributes[8]), 0, 0, (CLUSTER_MASK_CLIENT), NULL,  },    \
  }


// Endpoint types
#define GENERATED_ENDPOINT_TYPES {        \
    { (EmberAfCluster*)&(generatedClusters[0]), 16, 8 }, \
  }


// Networks
#define EM_AF_GENERATED_NETWORK_TYPES { \
  EM_AF_NETWORK_TYPE_ZIGBEE_PRO, /* Primary */ \
}
#define EM_AF_GENERATED_ZIGBEE_PRO_NETWORKS { \
  { \
    /* Primary */ \
    ZA_COORDINATOR, \
    EMBER_AF_SECURITY_PROFILE_HA, \
  }, \
}
#define EM_AF_GENERATED_NETWORK_STRINGS  \
  "Primary (pro)", \


// Cluster manufacturer codes
#define GENERATED_CLUSTER_MANUFACTURER_CODES {      \
{0x00, 0x00} \
  }
#define GENERATED_CLUSTER_MANUFACTURER_CODE_COUNT (0)

// Attribute manufacturer codes
#define GENERATED_ATTRIBUTE_MANUFACTURER_CODES {      \
{0x00, 0x00} \
  }
#define GENERATED_ATTRIBUTE_MANUFACTURER_CODE_COUNT (0)


// Largest attribute size is needed for various buffers
#define ATTRIBUTE_LARGEST (4)
// Total size of singleton attributes
#define ATTRIBUTE_SINGLETONS_SIZE (7)

// Total size of attribute storage
#define ATTRIBUTE_MAX_SIZE 8

// Array of endpoints that are supported
#define FIXED_ENDPOINT_ARRAY { 1 }

// Array of profile ids
#define FIXED_PROFILE_IDS { 260 }

// Array of profile ids
#define FIXED_DEVICE_IDS { 80 }

// Array of profile ids
#define FIXED_DEVICE_VERSIONS { 0 }

// Array of endpoint types supported on each endpoint
#define FIXED_ENDPOINT_TYPES { 0 }

// Array of networks supported on each endpoint
#define FIXED_NETWORKS { 0 }


// Code used to configure the cluster event mechanism
#define EMBER_AF_GENERATED_EVENT_CODE \
  EmberEventControl emberAfIdentifyClusterServerTickCallbackControl1; \
  EmberEventControl emberAfTimeClusterServerTickCallbackControl1; \
  extern EmberEventControl emberAfPluginConcentratorUpdateEventControl; \
  extern void emberAfPluginConcentratorUpdateEventHandler(void); \
  extern EmberEventControl emberAfPluginFormAndJoinCleanupEventControl; \
  extern void emberAfPluginFormAndJoinCleanupEventHandler(void); \
  extern EmberEventControl emberAfPluginIdentifyFeedbackProvideFeedbackEventControl; \
  extern void emberAfPluginIdentifyFeedbackProvideFeedbackEventHandler(void); \
  extern EmberEventControl emberAfPluginNetworkFindTickEventControl; \
  extern void emberAfPluginNetworkFindTickEventHandler(void); \
  extern EmberEventControl UartSendEventControl; \
  extern void UartSendEventFunction(void); \
  extern EmberEventControl UartGetEventControl; \
  extern void UartGetEventFunction(void); \
  extern EmberEventControl NwkFormEventControl; \
  extern void NwkFormEventFunction(void); \
  extern EmberEventControl BindDeviceEventControl; \
  extern void BindDeviceEventFunction(void); \
  extern EmberEventControl UnbindDeviceEventControl; \
  extern void UnbindDeviceEventFunction(void); \
  static void clusterTickWrapper(EmberEventControl *control, EmberAfTickFunction callback, uint8_t endpoint) \
  { \
    emberAfPushEndpointNetworkIndex(endpoint); \
    emberEventControlSetInactive(*control); \
    (*callback)(endpoint); \
    emberAfPopNetworkIndex(); \
  } \
  void emberAfIdentifyClusterServerTickCallbackWrapperFunction1(void) { clusterTickWrapper(&emberAfIdentifyClusterServerTickCallbackControl1, emberAfIdentifyClusterServerTickCallback, 1); } \
  void emberAfTimeClusterServerTickCallbackWrapperFunction1(void) { clusterTickWrapper(&emberAfTimeClusterServerTickCallbackControl1, emberAfTimeClusterServerTickCallback, 1); } \


// EmberEventData structs used to populate the EmberEventData table
#define EMBER_AF_GENERATED_EVENTS   \
  { &emberAfIdentifyClusterServerTickCallbackControl1, emberAfIdentifyClusterServerTickCallbackWrapperFunction1 }, \
  { &emberAfTimeClusterServerTickCallbackControl1, emberAfTimeClusterServerTickCallbackWrapperFunction1 }, \
  { &emberAfPluginConcentratorUpdateEventControl, emberAfPluginConcentratorUpdateEventHandler }, \
  { &emberAfPluginFormAndJoinCleanupEventControl, emberAfPluginFormAndJoinCleanupEventHandler }, \
  { &emberAfPluginIdentifyFeedbackProvideFeedbackEventControl, emberAfPluginIdentifyFeedbackProvideFeedbackEventHandler }, \
  { &emberAfPluginNetworkFindTickEventControl, emberAfPluginNetworkFindTickEventHandler }, \
  { &UartSendEventControl, UartSendEventFunction }, \
  { &UartGetEventControl, UartGetEventFunction }, \
  { &NwkFormEventControl, NwkFormEventFunction }, \
  { &BindDeviceEventControl, BindDeviceEventFunction }, \
  { &UnbindDeviceEventControl, UnbindDeviceEventFunction }, \


#define EMBER_AF_GENERATED_EVENT_STRINGS   \
  "Identify Cluster Server EP 1",  \
  "Time Cluster Server EP 1",  \
  "Concentrator Support Plugin Update",  \
  "Form and Join Library Plugin Cleanup",  \
  "Identify Feedback Plugin ProvideFeedback",  \
  "Network Find Plugin Tick",  \
  "UartSend Custom",  \
  "UartGetCommand Custom",  \
  "Nwk Custom",  \

// The length of the event context table used to track and retrieve cluster events
#define EMBER_AF_EVENT_CONTEXT_LENGTH 2

// EmberAfEventContext structs used to populate the EmberAfEventContext table
#define EMBER_AF_GENERATED_EVENT_CONTEXT { 0x1, 0x3, false, EMBER_AF_LONG_POLL, EMBER_AF_OK_TO_SLEEP, &emberAfIdentifyClusterServerTickCallbackControl1}, \
{ 0x1, 0xa, false, EMBER_AF_LONG_POLL, EMBER_AF_OK_TO_SLEEP, &emberAfTimeClusterServerTickCallbackControl1}


#define EMBER_AF_GENERATED_PLUGIN_INIT_FUNCTION_DECLARATIONS \
  void emberAfPluginConcentratorInitCallback(void); \
  void emberAfPluginCountersInitCallback(void); \
  void emberAfPluginNetworkFindInitCallback(void); \
  void emberAfPluginScanDispatchInitCallback(void); \


#define EMBER_AF_GENERATED_PLUGIN_INIT_FUNCTION_CALLS \
  emberAfPluginConcentratorInitCallback(); \
  emberAfPluginCountersInitCallback(); \
  emberAfPluginNetworkFindInitCallback(); \
  emberAfPluginScanDispatchInitCallback(); \


#define EMBER_AF_GENERATED_PLUGIN_NCP_INIT_FUNCTION_DECLARATIONS \
  void emberAfPluginAddressTableNcpInitCallback(bool memoryAllocation); \
  void emberAfPluginConcentratorNcpInitCallback(bool memoryAllocation); \


#define EMBER_AF_GENERATED_PLUGIN_NCP_INIT_FUNCTION_CALLS \
  emberAfPluginAddressTableNcpInitCallback(memoryAllocation); \
  emberAfPluginConcentratorNcpInitCallback(memoryAllocation); \


#define EMBER_AF_GENERATED_PLUGIN_STACK_STATUS_FUNCTION_DECLARATIONS \
  void emberAfPluginConcentratorStackStatusCallback(EmberStatus status); \
  void emberAfPluginNetworkFindStackStatusCallback(EmberStatus status); \


#define EMBER_AF_GENERATED_PLUGIN_STACK_STATUS_FUNCTION_CALLS \
  emberAfPluginConcentratorStackStatusCallback(status); \
  emberAfPluginNetworkFindStackStatusCallback(status); \


#define EMBER_AF_GENERATED_PLUGIN_MESSAGE_SENT_FUNCTION_DECLARATIONS \
  void emberAfPluginConcentratorMessageSentCallback(EmberOutgoingMessageType type, \
                    uint16_t indexOrDestination, \
                    EmberApsFrame *apsFrame, \
                    EmberStatus status, \
                    uint16_t messageLength, \
                    uint8_t *messageContents); \


#define EMBER_AF_GENERATED_PLUGIN_MESSAGE_SENT_FUNCTION_CALLS \
  emberAfPluginConcentratorMessageSentCallback(type, \
                    indexOrDestination, \
                    apsFrame, \
                    status, \
                    messageLength, \
                    messageContents); \

#define EMBER_AF_GENERATED_PLUGIN_TICK_FUNCTION_DECLARATIONS \
  void emberAfPluginHeartbeatTickCallback(void); \
  void emberAfPluginIdleSleepTickCallback(void); \
  void emberAfPluginScanDispatchTickCallback(void); \


#define EMBER_AF_GENERATED_PLUGIN_TICK_FUNCTION_CALLS \
  emberAfPluginHeartbeatTickCallback(); \
  emberAfPluginIdleSleepTickCallback(); \
  emberAfPluginScanDispatchTickCallback(); \

// Generated data for the command discovery
#define GENERATED_COMMANDS { \
    { 0x0003, 0x00, COMMAND_MASK_OUTGOING_CLIENT | COMMAND_MASK_INCOMING_SERVER }, /* Identify / Identify */ \
    { 0x0003, 0x00, COMMAND_MASK_OUTGOING_SERVER }, /* Identify / IdentifyQueryResponse */ \
    { 0x0003, 0x01, COMMAND_MASK_OUTGOING_CLIENT | COMMAND_MASK_INCOMING_SERVER }, /* Identify / IdentifyQuery */ \
    { 0x0004, 0x00, COMMAND_MASK_OUTGOING_CLIENT }, /* Groups / AddGroup */ \
    { 0x0004, 0x01, COMMAND_MASK_OUTGOING_CLIENT }, /* Groups / ViewGroup */ \
    { 0x0004, 0x02, COMMAND_MASK_OUTGOING_CLIENT }, /* Groups / GetGroupMembership */ \
    { 0x0004, 0x03, COMMAND_MASK_OUTGOING_CLIENT }, /* Groups / RemoveGroup */ \
    { 0x0004, 0x04, COMMAND_MASK_OUTGOING_CLIENT }, /* Groups / RemoveAllGroups */ \
    { 0x0004, 0x05, COMMAND_MASK_OUTGOING_CLIENT }, /* Groups / AddGroupIfIdentifying */ \
    { 0x0005, 0x00, COMMAND_MASK_OUTGOING_CLIENT }, /* Scenes / AddScene */ \
    { 0x0005, 0x01, COMMAND_MASK_OUTGOING_CLIENT }, /* Scenes / ViewScene */ \
    { 0x0005, 0x02, COMMAND_MASK_OUTGOING_CLIENT }, /* Scenes / RemoveScene */ \
    { 0x0005, 0x03, COMMAND_MASK_OUTGOING_CLIENT }, /* Scenes / RemoveAllScenes */ \
    { 0x0005, 0x04, COMMAND_MASK_OUTGOING_CLIENT }, /* Scenes / StoreScene */ \
    { 0x0005, 0x05, COMMAND_MASK_OUTGOING_CLIENT }, /* Scenes / RecallScene */ \
    { 0x0005, 0x06, COMMAND_MASK_OUTGOING_CLIENT }, /* Scenes / GetSceneMembership */ \
    { 0x0006, 0x00, COMMAND_MASK_OUTGOING_CLIENT }, /* On/off / Off */ \
    { 0x0006, 0x01, COMMAND_MASK_OUTGOING_CLIENT }, /* On/off / On */ \
    { 0x0006, 0x02, COMMAND_MASK_OUTGOING_CLIENT }, /* On/off / Toggle */ \
    { 0x0008, 0x00, COMMAND_MASK_OUTGOING_CLIENT }, /* Level Control / MoveToLevel */ \
    { 0x0008, 0x01, COMMAND_MASK_OUTGOING_CLIENT }, /* Level Control / Move */ \
    { 0x0008, 0x02, COMMAND_MASK_OUTGOING_CLIENT }, /* Level Control / Step */ \
    { 0x0008, 0x03, COMMAND_MASK_OUTGOING_CLIENT }, /* Level Control / Stop */ \
    { 0x0008, 0x04, COMMAND_MASK_OUTGOING_CLIENT }, /* Level Control / MoveToLevelWithOnOff */ \
    { 0x0008, 0x05, COMMAND_MASK_OUTGOING_CLIENT }, /* Level Control / MoveWithOnOff */ \
    { 0x0008, 0x06, COMMAND_MASK_OUTGOING_CLIENT }, /* Level Control / StepWithOnOff */ \
    { 0x0008, 0x07, COMMAND_MASK_OUTGOING_CLIENT }, /* Level Control / StopWithOnOff */ \
    { 0x001A, 0x00, COMMAND_MASK_OUTGOING_CLIENT }, /* Power Profile / PowerProfileRequest */ \
    { 0x001A, 0x01, COMMAND_MASK_OUTGOING_CLIENT }, /* Power Profile / PowerProfileStateRequest */ \
    { 0x001A, 0x02, COMMAND_MASK_OUTGOING_CLIENT }, /* Power Profile / GetPowerProfilePriceResponse */ \
    { 0x001A, 0x03, COMMAND_MASK_OUTGOING_CLIENT }, /* Power Profile / GetOverallSchedulePriceResponse */ \
    { 0x001A, 0x04, COMMAND_MASK_OUTGOING_CLIENT }, /* Power Profile / EnergyPhasesScheduleNotification */ \
    { 0x001A, 0x05, COMMAND_MASK_OUTGOING_CLIENT }, /* Power Profile / EnergyPhasesScheduleResponse */ \
    { 0x001A, 0x06, COMMAND_MASK_OUTGOING_CLIENT }, /* Power Profile / PowerProfileScheduleConstraintsRequest */ \
    { 0x001A, 0x07, COMMAND_MASK_OUTGOING_CLIENT }, /* Power Profile / EnergyPhasesScheduleStateRequest */ \
    { 0x001A, 0x08, COMMAND_MASK_OUTGOING_CLIENT }, /* Power Profile / GetPowerProfilePriceExtendedResponse */ \
    { 0x0101, 0x00, COMMAND_MASK_OUTGOING_CLIENT }, /* Door Lock / LockDoor */ \
    { 0x0101, 0x01, COMMAND_MASK_OUTGOING_CLIENT }, /* Door Lock / UnlockDoor */ \
    { 0x0300, 0x07, COMMAND_MASK_OUTGOING_CLIENT }, /* Color Control / MoveToColor */ \
    { 0x0300, 0x08, COMMAND_MASK_OUTGOING_CLIENT }, /* Color Control / MoveColor */ \
    { 0x0300, 0x09, COMMAND_MASK_OUTGOING_CLIENT }, /* Color Control / StepColor */ \
    { 0x0500, 0x00, COMMAND_MASK_OUTGOING_CLIENT }, /* IAS Zone / ZoneEnrollResponse */ \
    { 0x0702, 0x00, COMMAND_MASK_INCOMING_CLIENT }, /* Simple Metering / GetProfileResponse */ \
    { 0x0702, 0x01, COMMAND_MASK_INCOMING_CLIENT }, /* Simple Metering / RequestMirror */ \
    { 0x0702, 0x02, COMMAND_MASK_INCOMING_CLIENT }, /* Simple Metering / RemoveMirror */ \
    { 0x0702, 0x03, COMMAND_MASK_INCOMING_CLIENT }, /* Simple Metering / RequestFastPollModeResponse */ \
    { 0x0702, 0x0C, COMMAND_MASK_INCOMING_CLIENT }, /* Simple Metering / SupplyStatusResponse */ \
    { 0x0B03, 0x00, COMMAND_MASK_OUTGOING_CLIENT }, /* Appliance Statistics / LogRequest */ \
    { 0x0B03, 0x01, COMMAND_MASK_OUTGOING_CLIENT }, /* Appliance Statistics / LogQueueRequest */ \
  }
#define EMBER_AF_GENERATED_COMMAND_COUNT (49)
#endif // __AF_ENDPOINT_CONFIG__
