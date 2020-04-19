/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

Module Name:

    PosCx.h

Abstract:

    Header file that defines structures, prototypes and definitions required
    by PoS class extension client drivers.

--*/

//
// NOTE: This header is generated by stubwork.  Please make any 
//       modifications to the corresponding template files 
//       (.x or .y) and use stubwork to regenerate the header
//

#ifndef _POSCX_H_
#define _POSCX_H_

#ifndef WDF_EXTERN_C
  #ifdef __cplusplus
    #define WDF_EXTERN_C       extern "C"
    #define WDF_EXTERN_C_START extern "C" {
    #define WDF_EXTERN_C_END   }
  #else
    #define WDF_EXTERN_C
    #define WDF_EXTERN_C_START
    #define WDF_EXTERN_C_END
  #endif
#endif

WDF_EXTERN_C_START



typedef VOID (*PPOSCX_EXPORTED_INTERFACES)(VOID);
extern PPOSCX_EXPORTED_INTERFACES PoscxFunctions [];
typedef struct _WDF_DRIVER_GLOBALS POSCX_DRIVER_GLOBALS, *PPOSCX_DRIVER_GLOBALS;
typedef WDFOBJECT POSOBJECT, *PPOSOBJECT;

#include "PoscxFuncEnum.h"

typedef enum _POS_CX_EVENT_DEST {
    POS_CX_EVENT_DEST_INVALID = 0,
    POS_CX_EVENT_DEST_DEVICE_OWNER = 1,
    POS_CX_EVENT_DEST_ALL = 2,
    POS_CX_EVENT_DEST__MAX,
} POS_CX_EVENT_DEST;

typedef enum _POS_CX_EVENT_PRIORITY {
    POS_CX_EVENT_PRIORITY_INVALID = 0,
    POS_CX_EVENT_PRIORITY_DATA = 1,
    POS_CX_EVENT_PRIORITY_CONTROL = 2,
    POS_CX_EVENT_PRIORITY__MAX,
} POS_CX_EVENT_PRIORITY;

typedef enum _POS_CX_EVENT_ATTRIBUTES {
    POS_CX_EVENT_ATTR_DATA = ((POS_CX_EVENT_DEST_DEVICE_OWNER << 8) | POS_CX_EVENT_PRIORITY_DATA),
    POS_CX_EVENT_ATTR_CONTROL = ((POS_CX_EVENT_DEST_DEVICE_OWNER << 8) | POS_CX_EVENT_PRIORITY_CONTROL),
    POS_CX_EVENT_ATTR_BCAST_CONTROL = ((POS_CX_EVENT_DEST_ALL << 8)          | POS_CX_EVENT_PRIORITY_CONTROL),
} POS_CX_EVENT_ATTRIBUTES;



//
// CLX to Client Interface ----------------------------------------------------
//

//
//      EVT_POS_CX_DEVICE_OWNERSHIP_CHANGE callback is called when the device ownership has changed.
//      It is used by the client driver to set the device to its default state.
//
typedef
_Function_class_(EVT_POS_CX_DEVICE_OWNERSHIP_CHANGE)
_IRQL_requires_same_
VOID
EVT_POS_CX_DEVICE_OWNERSHIP_CHANGE(
    _In_
    WDFDEVICE device,
    _In_opt_
    WDFFILEOBJECT oldOwnerFileObj,
    _In_opt_
    WDFFILEOBJECT newOwnerFileObj
    );

typedef EVT_POS_CX_DEVICE_OWNERSHIP_CHANGE *PFN_POS_CX_DEVICE_OWNERSHIP_CHANGE;

//
// EVT_POS_CX_DEVICE_REMOTE_CLAIM callback is called whenever the device transitions from
// unclaimed to claimed.  This adds support for remote devices that handle their own claim
// semantics.
//
typedef
_Function_class_(EVT_POS_CX_DEVICE_REMOTE_CLAIM)
_IRQL_requires_same_
_Check_return_
NTSTATUS
EVT_POS_CX_DEVICE_REMOTE_CLAIM(
    _In_
    WDFDEVICE device,
    ULONG deviceInterfaceTag
    );

typedef EVT_POS_CX_DEVICE_REMOTE_CLAIM *PFN_POS_CX_DEVICE_REMOTE_CLAIM;

//
// EVT_POS_CX_DEVICE_REMOTE_RELEASE callback is called whenever the device is released and left
// with no owner.  This + previous callback adds support for remote devices that can handle their
// own claim semantics. 
//
typedef
_Function_class_(EVT_POS_CX_DEVICE_REMOTE_RELEASE)
_IRQL_requires_same_
_Check_return_
NTSTATUS
EVT_POS_CX_DEVICE_REMOTE_RELEASE(
    _In_
    WDFDEVICE device,
    ULONG deviceInterfaceTag
    );

typedef EVT_POS_CX_DEVICE_REMOTE_RELEASE *PFN_POS_CX_DEVICE_REMOTE_RELEASE;

//
// EVT_POS_CX_DEVICE_REMOTE_RETAIN callback is called whenever poscx attempts to hold onto a claim
// on a device.
//
typedef
_Function_class_(EVT_POS_CX_DEVICE_REMOTE_RETAIN)
_IRQL_requires_same_
_Check_return_
NTSTATUS
EVT_POS_CX_DEVICE_REMOTE_RETAIN(
    _In_
    WDFDEVICE device,
    ULONG deviceInterfaceTag
    );

typedef EVT_POS_CX_DEVICE_REMOTE_RETAIN *PFN_POS_CX_DEVICE_REMOTE_RETAIN;

//
// The POS library attributes.
// This structure is passed during POS library initialization.
// It is used by the caller to setup callback functions, ...
//
typedef struct _POS_CX_ATTRIBUTES {
    //
    // Function to device ownership has changed
    //
    PFN_POS_CX_DEVICE_OWNERSHIP_CHANGE EvtDeviceOwnershipChange;
    PFN_POS_CX_DEVICE_REMOTE_CLAIM EvtDeviceRemoteClaim;
    PFN_POS_CX_DEVICE_REMOTE_RELEASE EvtDeviceRemoteRelease;
    PFN_POS_CX_DEVICE_REMOTE_RETAIN EvtDeviceRemoteRetain;
} POS_CX_ATTRIBUTES;



//
//
//  Routine Description:
//      POS_CX_ATTRIBUTES_INIT() is called to initialize POS_CX_ATTRIBUTES, before calling poscxInit().
//      The caller may setup callback routines, after calling POS_CX_ATTRIBUTES_INIT().
//
//  Arguments:
//
//      poscxAttrPtr - Address of a caller allocated POS_CX_ATTRIBUTES to be initilized.
//
//  Return Value:
//
//      
//
VOID
FORCEINLINE
POS_CX_ATTRIBUTES_INIT(
    _Out_ POS_CX_ATTRIBUTES *PosCxAttrPtr
    )
{
    RtlZeroMemory(PosCxAttrPtr, sizeof(*PosCxAttrPtr));
}

//
// POSCX Function: PosCxClaimDevice
//
typedef
WDFAPI
NTSTATUS
(*PFN_POSCXCLAIMDEVICE)(
    _In_
    PPOSCX_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFDEVICE device,
    _In_
    WDFREQUEST request
    );

NTSTATUS
FORCEINLINE
PosCxClaimDevice(
    _In_
    WDFDEVICE device,
    _In_
    WDFREQUEST request
    )
{
    return ((PFN_POSCXCLAIMDEVICE) PoscxFunctions[PosCxClaimDeviceTableIndex])(PoscxDriverGlobals, device, request);
}

//
// POSCX Function: PosCxInit
//
typedef
WDFAPI
NTSTATUS
(*PFN_POSCXINIT)(
    _In_
    PPOSCX_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFDEVICE device,
    _In_
    POS_CX_ATTRIBUTES* posCxAttrPtr
    );

NTSTATUS
FORCEINLINE
PosCxInit(
    _In_
    WDFDEVICE device,
    _In_
    POS_CX_ATTRIBUTES* posCxAttrPtr
    )
{
    return ((PFN_POSCXINIT) PoscxFunctions[PosCxInitTableIndex])(PoscxDriverGlobals, device, posCxAttrPtr);
}

//
// POSCX Function: PosCxRetainDevice
//
typedef
WDFAPI
NTSTATUS
(*PFN_POSCXRETAINDEVICE)(
    _In_
    PPOSCX_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFDEVICE device,
    _In_
    WDFREQUEST request
    );

NTSTATUS
FORCEINLINE
PosCxRetainDevice(
    _In_
    WDFDEVICE device,
    _In_
    WDFREQUEST request
    )
{
    return ((PFN_POSCXRETAINDEVICE) PoscxFunctions[PosCxRetainDeviceTableIndex])(PoscxDriverGlobals, device, request);
}

//
// POSCX Function: PosCxRemoteRequestRelease
//
typedef
WDFAPI
NTSTATUS
(*PFN_POSCXREMOTEREQUESTRELEASE)(
    _In_
    PPOSCX_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFDEVICE device,
    _In_
    ULONG deviceInterfaceTag
    );

NTSTATUS
FORCEINLINE
PosCxRemoteRequestRelease(
    _In_
    WDFDEVICE device,
    _In_
    ULONG deviceInterfaceTag
    )
{
    return ((PFN_POSCXREMOTEREQUESTRELEASE) PoscxFunctions[PosCxRemoteRequestReleaseTableIndex])(PoscxDriverGlobals, device, deviceInterfaceTag);
}

//
// POSCX Function: PosCxReleaseDevice
//
typedef
WDFAPI
NTSTATUS
(*PFN_POSCXRELEASEDEVICE)(
    _In_
    PPOSCX_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFDEVICE device,
    _In_
    WDFFILEOBJECT fileObject
    );

NTSTATUS
FORCEINLINE
PosCxReleaseDevice(
    _In_
    WDFDEVICE device,
    _In_
    WDFFILEOBJECT fileObject
    )
{
    return ((PFN_POSCXRELEASEDEVICE) PoscxFunctions[PosCxReleaseDeviceTableIndex])(PoscxDriverGlobals, device, fileObject);
}

//
// POSCX Function: PosCxIsDeviceOwner
//
typedef
WDFAPI
BOOLEAN
(*PFN_POSCXISDEVICEOWNER)(
    _In_
    PPOSCX_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFDEVICE device,
    _In_
    WDFFILEOBJECT fileObject
    );

BOOLEAN
FORCEINLINE
PosCxIsDeviceOwner(
    _In_
    WDFDEVICE device,
    _In_
    WDFFILEOBJECT fileObject
    )
{
    return ((PFN_POSCXISDEVICEOWNER) PoscxFunctions[PosCxIsDeviceOwnerTableIndex])(PoscxDriverGlobals, device, fileObject);
}

//
// POSCX Function: PosCxGetDeviceInterfaceTag
//
typedef
WDFAPI
ULONG
(*PFN_POSCXGETDEVICEINTERFACETAG)(
    _In_
    PPOSCX_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFFILEOBJECT fileObject
    );

ULONG
FORCEINLINE
PosCxGetDeviceInterfaceTag(
    _In_
    WDFFILEOBJECT fileObject
    )
{
    return ((PFN_POSCXGETDEVICEINTERFACETAG) PoscxFunctions[PosCxGetDeviceInterfaceTagTableIndex])(PoscxDriverGlobals, fileObject);
}

//
// POSCX Function: PosCxPutPendingEvent
//
typedef
WDFAPI
NTSTATUS
(*PFN_POSCXPUTPENDINGEVENT)(
    _In_
    PPOSCX_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFDEVICE device,
    _In_
    ULONG deviceInterfaceTag,
    _In_
    ULONG eventType,
    _In_
    size_t rawEventDataSize,
    _In_reads_opt_(rawEventDataSize)
    PVOID rawEventDataPtr,
    _In_
    POS_CX_EVENT_ATTRIBUTES eventAttr
    );

NTSTATUS
FORCEINLINE
PosCxPutPendingEvent(
    _In_
    WDFDEVICE device,
    _In_
    ULONG deviceInterfaceTag,
    _In_
    ULONG eventType,
    _In_
    size_t rawEventDataSize,
    _In_reads_opt_(rawEventDataSize)
    PVOID rawEventDataPtr,
    _In_
    POS_CX_EVENT_ATTRIBUTES eventAttr
    )
{
    return ((PFN_POSCXPUTPENDINGEVENT) PoscxFunctions[PosCxPutPendingEventTableIndex])(PoscxDriverGlobals, device, deviceInterfaceTag, eventType, rawEventDataSize, rawEventDataPtr, eventAttr);
}

//
// POSCX Function: PosCxPutPendingEventMemory
//
typedef
WDFAPI
NTSTATUS
(*PFN_POSCXPUTPENDINGEVENTMEMORY)(
    _In_
    PPOSCX_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFDEVICE device,
    _In_
    ULONG deviceInterfaceTag,
    _In_
    WDFMEMORY eventMemory,
    _In_
    POS_CX_EVENT_ATTRIBUTES eventAttr
    );

NTSTATUS
FORCEINLINE
PosCxPutPendingEventMemory(
    _In_
    WDFDEVICE device,
    _In_
    ULONG deviceInterfaceTag,
    _In_
    WDFMEMORY eventMemory,
    _In_
    POS_CX_EVENT_ATTRIBUTES eventAttr
    )
{
    return ((PFN_POSCXPUTPENDINGEVENTMEMORY) PoscxFunctions[PosCxPutPendingEventMemoryTableIndex])(PoscxDriverGlobals, device, deviceInterfaceTag, eventMemory, eventAttr);
}

//
// POSCX Function: PosCxGetPendingEvent
//
typedef
WDFAPI
NTSTATUS
(*PFN_POSCXGETPENDINGEVENT)(
    _In_
    PPOSCX_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFDEVICE device,
    _In_
    WDFREQUEST request
    );

NTSTATUS
FORCEINLINE
PosCxGetPendingEvent(
    _In_
    WDFDEVICE device,
    _In_
    WDFREQUEST request
    )
{
    return ((PFN_POSCXGETPENDINGEVENT) PoscxFunctions[PosCxGetPendingEventTableIndex])(PoscxDriverGlobals, device, request);
}

//
// POSCX Function: PosCxCleanupEvents
//
typedef
WDFAPI
VOID
(*PFN_POSCXCLEANUPEVENTS)(
    _In_
    PPOSCX_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFDEVICE device,
    _In_
    WDFFILEOBJECT fileObject
    );

VOID
FORCEINLINE
PosCxCleanupEvents(
    _In_
    WDFDEVICE device,
    _In_
    WDFFILEOBJECT fileObject
    )
{
    ((PFN_POSCXCLEANUPEVENTS) PoscxFunctions[PosCxCleanupEventsTableIndex])(PoscxDriverGlobals, device, fileObject);
}

//
// POSCX Function: PosCxCleanPendingRequests
//
typedef
WDFAPI
VOID
(*PFN_POSCXCLEANPENDINGREQUESTS)(
    _In_
    PPOSCX_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFDEVICE device,
    _In_opt_
    WDFFILEOBJECT callerFileObj,
    _In_
    NTSTATUS completionStatus
    );

VOID
FORCEINLINE
PosCxCleanPendingRequests(
    _In_
    WDFDEVICE device,
    _In_opt_
    WDFFILEOBJECT callerFileObj,
    _In_
    NTSTATUS completionStatus
    )
{
    ((PFN_POSCXCLEANPENDINGREQUESTS) PoscxFunctions[PosCxCleanPendingRequestsTableIndex])(PoscxDriverGlobals, device, callerFileObj, completionStatus);
}

//
// POSCX Function: PosCxIsPosApp
//
typedef
WDFAPI
BOOLEAN
(*PFN_POSCXISPOSAPP)(
    _In_
    PPOSCX_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFDEVICE device,
    _In_
    WDFFILEOBJECT fileObject
    );

BOOLEAN
FORCEINLINE
PosCxIsPosApp(
    _In_
    WDFDEVICE device,
    _In_
    WDFFILEOBJECT fileObject
    )
{
    return ((PFN_POSCXISPOSAPP) PoscxFunctions[PosCxIsPosAppTableIndex])(PoscxDriverGlobals, device, fileObject);
}

//
// POSCX Function: PosCxMarkPosApp
//
typedef
WDFAPI
NTSTATUS
(*PFN_POSCXMARKPOSAPP)(
    _In_
    PPOSCX_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFDEVICE device,
    _In_
    WDFFILEOBJECT fileObject,
    _In_
    BOOLEAN isPosApp
    );

NTSTATUS
FORCEINLINE
PosCxMarkPosApp(
    _In_
    WDFDEVICE device,
    _In_
    WDFFILEOBJECT fileObject,
    _In_
    BOOLEAN isPosApp
    )
{
    return ((PFN_POSCXMARKPOSAPP) PoscxFunctions[PosCxMarkPosAppTableIndex])(PoscxDriverGlobals, device, fileObject, isPosApp);
}

//
// POSCX Function: PosCxOpen
//
typedef
WDFAPI
NTSTATUS
(*PFN_POSCXOPEN)(
    _In_
    PPOSCX_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFDEVICE device,
    _In_
    WDFFILEOBJECT fileObject,
    _In_
    ULONG deviceInterfaceTag
    );

NTSTATUS
FORCEINLINE
PosCxOpen(
    _In_
    WDFDEVICE device,
    _In_
    WDFFILEOBJECT fileObject,
    _In_
    ULONG deviceInterfaceTag
    )
{
    return ((PFN_POSCXOPEN) PoscxFunctions[PosCxOpenTableIndex])(PoscxDriverGlobals, device, fileObject, deviceInterfaceTag);
}

//
// POSCX Function: PosCxClose
//
typedef
WDFAPI
NTSTATUS
(*PFN_POSCXCLOSE)(
    _In_
    PPOSCX_DRIVER_GLOBALS DriverGlobals,
    _In_
    WDFDEVICE device,
    _In_
    WDFFILEOBJECT fileObject
    );

NTSTATUS
FORCEINLINE
PosCxClose(
    _In_
    WDFDEVICE device,
    _In_
    WDFFILEOBJECT fileObject
    )
{
    return ((PFN_POSCXCLOSE) PoscxFunctions[PosCxCloseTableIndex])(PoscxDriverGlobals, device, fileObject);
}



WDF_EXTERN_C_END

#endif // _POSCX_H_

