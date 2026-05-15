# MCMGR Porting guide to version v5.0.0

This porting guide will describe steps to port Multicore Manager (MCMGR) to version v5.0.0.

## Overview

The implementation of MCMGR before version v5.0.0. assumed that only two cores were  
present on device.

That caused some functions were missing parameter specifying on what core to call  
that function and it was always assumed that it is other core then itself internally.

With introduction of device with more than two core this limitation needed to be  
addressed and change in API was introduced in this release.

## Changes in Directory Structure

All include files were moved from `src/` directory to `inc/` directory.

New platform specific include directory added `inc/platform/<platform_name>/mcmgr_platform.h`.
This contains platform specific macros like `MCMGR_CORECOUNT (2)`.

Please update your project include paths accordingly.

## Changes in the API

Main change in public API is adding the core number parameter to functions described bellow.

Before, for example just calling `MCMGR_GetStartupData()` function assumed that it  
requires data from other core.

But with more than two cores Application using this function needs to decide from what  
core number it wants to receive startup data.

And same way for `MCMGR_TriggerEvent` function.

### Event Callback function change

Change in Event Callback function will now receive core number from core which event  
was sent.

This information then can be used by application to capture events from specific cores.

```diff
- typedef void (*mcmgr_event_callback_t)(uint16_t data, void *context);
+ typedef void (*mcmgr_event_callback_t)(mcmgr_core_t coreNum, uint16_t data, void *context);
```

### MCMGR_GetStartupData() change

```diff
- mcmgr_status_t MCMGR_GetStartupData(uint32_t *startupData);
+ mcmgr_status_t MCMGR_GetStartupData(mcmgr_core_t coreNum, uint32_t *startupData);
```

### MCMGR_TriggerEvent() change

```diff
- mcmgr_status_t MCMGR_TriggerEvent(mcmgr_event_type_t type, uint16_t eventData);
+ mcmgr_status_t MCMGR_TriggerEvent(mcmgr_core_t coreNum, mcmgr_event_type_t type, uint16_t eventData);
```

### MCMGR_TriggerEventForce() change

```diff
- mcmgr_status_t MCMGR_TriggerEventForce(mcmgr_event_type_t type, uint16_t eventData);
+ mcmgr_status_t MCMGR_TriggerEventForce(mcmgr_core_t coreNum, mcmgr_event_type_t type, uint16_t eventData);
```

## How to port your Application

This chapter will describe necessary steps to port your application using MCMGR.

### Event Handler change

Before this change event handler only received event data from one core.

```c
void RPMsgRemoteReadyEventHandler(uint16_t eventData, void *context)
{
    uint16_t *data = (uint16_t *)context;

    *data = eventData;
}

void main()
{
    uint16_t RPMsgRemoteReadyEventData = {0};

    MCMGR_RegisterEvent(kMCMGR_RemoteApplicationEvent, RPMsgRemoteReadyEventHandler,
                        RPMsgRemoteReadyEventData);
}
```

After this change event handler can receive event data from multiple cores and application  
is responsible to register callback data `RPMsgRemoteReadyEventData` array with length of  
cores in the system.

For this `MCMGR_CORECOUNT` platform specific macro can be used.

The type of context data is Applications specific and make sure to do correct casting  
in application event handler.

> **_NOTE:_**  In case the Application is running on system with only two cores  
> then application can still use `MCMGR_RegisterEvent` and Event handler as before.
> This is because the event handler really cannot be trigger by any other core if there  
> are only two.

```c
static void RPMsgRemoteReadyEventHandler(mcmgr_core_t coreNum, uint16_t eventData, void *context)
{
    uint16_t *data = &((uint16_t *)context)[coreNum];

    *data = eventData;
}

int main(void)
{
    volatile uint16_t RPMsgRemoteReadyEventData[MCMGR_CORECOUNT] = {0};

    MCMGR_RegisterEvent(kMCMGR_RemoteApplicationEvent, RPMsgRemoteReadyEventHandler,
                       (void *)RPMsgRemoteReadyEventData);
}
```

### GetStartupData and TriggerEvent functions

For these functions application needs to provide new parameter of core number.

```diff
- MCMGR_TriggerEvent(kMCMGR_RemoteApplicationEvent, APP_RPMSG_READY_EVENT_DATA);
+ MCMGR_TriggerEvent(kMCMGR_Core0, kMCMGR_RemoteApplicationEvent, APP_RPMSG_READY_EVENT_DATA);
```

Example of getting startup data from secondary core `kMCMGR_Core1` will specify it  
wants to request data from `kMCMGR_Core0`.

```diff
uint32_t startupData;

do
{
-    mcmgrStatus = MCMGR_GetStartupData(&startupData);
+    mcmgrStatus = MCMGR_GetStartupData(kMCMGR_Core0, &startupData);
} while (mcmgrStatus != kStatus_MCMGR_Success);
```
