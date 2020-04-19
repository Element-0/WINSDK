
/*++

Copyright (c) Microsoft Corporation. All rights reserved.

Module Name:

    evntprov.h

Abstract:

    This defines the unified provider side user mode API.

Revision History:

--*/

#ifndef _EVNTPROV_H_
#define _EVNTPROV_H_

#pragma once

#include <winapifamily.h>

#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable:4820) // padding added
#endif

#ifndef _EVNT_SOURCE_
#ifdef _NTDLLBUILD_
#define _EVNT_SOURCE_
#endif // _NTDLLBUILD_
#endif // _EVNT_SOURCE_

#ifndef EVNTAPI
#ifndef MIDL_PASS
#ifdef _EVNT_SOURCE_
#define EVNTAPI __stdcall
#else
#define EVNTAPI DECLSPEC_IMPORT __stdcall
#endif // _EVNT_SOURCE_
#endif // MIDL_PASS
#endif // EVNTAPI

#ifndef EVNTPROV_PFORCEINLINE
  #if defined(PFORCEINLINE)
    #define EVNTPROV_PFORCEINLINE PFORCEINLINE
  #elif defined(FORCEINLINE)
    #define EVNTPROV_PFORCEINLINE FORCEINLINE
  #else
    #define EVNTPROV_PFORCEINLINE __forceinline
  #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define EVENT_MIN_LEVEL                      (0) /*
    Lowest value for an event level is 0. */
#define EVENT_MAX_LEVEL                      (0xff) /*
    Highest value for an event level is 255. */

/*
EVENT_ACTIVITY_CTRL values for the ControlCode parameter of
EventActivityIdControl.
*/
#define EVENT_ACTIVITY_CTRL_GET_ID           (1) /*
    EventActivityIdControl will return the current thread's activity ID. */
#define EVENT_ACTIVITY_CTRL_SET_ID           (2) /*
    EventActivityIdControl will set the current thread's activity ID. */
#define EVENT_ACTIVITY_CTRL_CREATE_ID        (3) /*
    EventActivityIdControl will generate and return a new activity ID. Note
    that the returned activity ID is not a GUID. The EventActivityIdControl
    function uses a faster generation algorithm than UuidCreate. The returned
    ID is guaranteed to be different from any valid GUID and different from any
    other activity ID generated by EventActivityIdControl on the same machine
    during the same boot session. */
#define EVENT_ACTIVITY_CTRL_GET_SET_ID       (4) /*
    EventActivityIdControl will set the current thread's activity ID and
    return the previous activity ID. */
#define EVENT_ACTIVITY_CTRL_CREATE_SET_ID    (5) /*
    EventActivityIdControl will generate a new activity ID, set the current
    thread's activity ID to the new value, and return the previous activity
    ID. */

#define MAX_EVENT_DATA_DESCRIPTORS           (128) /*
    The maximum number of EVENT_DATA_DESCRIPTORs that can be used in an event.
    Used with EventWrite, EventWriteTransfer, EventWriteEx. */
#define MAX_EVENT_FILTER_DATA_SIZE           (1024) /*
    The maximum data size for many of the filter types.
    Used with EVENT_FILTER_DESCRIPTOR. */
#define MAX_EVENT_FILTER_PAYLOAD_SIZE        (4096) /*
    The maximum data size for an event payload filter.
    Used with EVENT_FILTER_DESCRIPTOR of type EVENT_FILTER_TYPE_PAYLOAD. */
#define MAX_EVENT_FILTER_EVENT_NAME_SIZE     (4096) /*
    The maximum data size for a name-based filter.
    Used with EVENT_FILTER_DESCRIPTOR for name-based filters. */

#define MAX_EVENT_FILTERS_COUNT              (13) /*
    The maximum number of filters that can be provided in a call to
    EnableTraceEx2.
    Used with ENABLE_TRACE_PARAMETERS. */

#define MAX_EVENT_FILTER_PID_COUNT           (8) /*
    The maximum number of process IDs in a PID filter.
    Used with EVENT_FILTER_DESCRIPTOR of type EVENT_FILTER_TYPE_PID. */

#define MAX_EVENT_FILTER_EVENT_ID_COUNT      (64) /*
    The maximum number of event IDs in an event ID or stackwalk filter.
    Used with EVENT_FILTER_DESCRIPTOR of type EVENT_FILTER_TYPE_EVENT_ID or
    EVENT_FILTER_TYPE_STACKWALK. */

/*
EVENT_FILTER_TYPE values for the Type field of EVENT_FILTER_DESCRIPTOR.
*/
#define EVENT_FILTER_TYPE_NONE               (0x00000000)
#define EVENT_FILTER_TYPE_SCHEMATIZED        (0x80000000) // Provider-side.
#define EVENT_FILTER_TYPE_SYSTEM_FLAGS       (0x80000001) // Internal use only.
#define EVENT_FILTER_TYPE_TRACEHANDLE        (0x80000002) // Initiate rundown.
#define EVENT_FILTER_TYPE_PID                (0x80000004) // Process ID.
#define EVENT_FILTER_TYPE_EXECUTABLE_NAME    (0x80000008) // EXE file name.
#define EVENT_FILTER_TYPE_PACKAGE_ID         (0x80000010) // Package ID.
#define EVENT_FILTER_TYPE_PACKAGE_APP_ID     (0x80000020) // Package Relative App Id (PRAID).
#define EVENT_FILTER_TYPE_PAYLOAD            (0x80000100) // TDH payload filter.
#define EVENT_FILTER_TYPE_EVENT_ID           (0x80000200) // Event IDs.
#define EVENT_FILTER_TYPE_EVENT_NAME         (0x80000400) // Event name (TraceLogging only).
#define EVENT_FILTER_TYPE_STACKWALK          (0x80001000) // Event IDs for stack.
#define EVENT_FILTER_TYPE_STACKWALK_NAME     (0x80002000) // Event name for stack (TraceLogging only).
#define EVENT_FILTER_TYPE_STACKWALK_LEVEL_KW (0x80004000) // Filter stack collection by level and keyword.

/*
EVENT_DATA_DESCRIPTOR_TYPE values for the Type field of EVENT_DATA_DESCRIPTOR.
Note that the Type field is ignored by default. ETW will only check the Type
field if a provider has opted-in via a call to EventSetInformation.
*/
#define EVENT_DATA_DESCRIPTOR_TYPE_NONE               (0) /*
   Event data (payload). */
#define EVENT_DATA_DESCRIPTOR_TYPE_EVENT_METADATA     (1) /*
   TraceLogging event decoding information. */
#define EVENT_DATA_DESCRIPTOR_TYPE_PROVIDER_METADATA  (2) /*
    Manually-attached provider traits. For use with operating systems that do
    not support attaching provider traits via EventSetInformation. This data
    will be ignored if provider traits have been configured via
    EventSetInformation. */
#define EVENT_DATA_DESCRIPTOR_TYPE_TIMESTAMP_OVERRIDE (3) /*
    64-bit event timestamp override. For use when relogging. Note that logging
    events out of timestamp order may lead to event ordering issues during
    trace processing. */

/*
EVENT_WRITE_FLAG values for the Flag parameter of EventWriteEx and EtwWriteEx.
*/
#define EVENT_WRITE_FLAG_NO_FAULTING 0x00000001 /*
    EVENT_WRITE_FLAG_NO_FAULTING is used in kernel-mode calls to EtwWriteEx.
    Certain ETW functionality requires paging. ETW will automatically disable
    such functionality when it is called at high IRQL (dispatch or above).
    To call ETW at low IRQL (below dispatch) from a context in which page
    faults are not tolerated, use EtwWriteEx and include this flag. */
#define EVENT_WRITE_FLAG_INPRIVATE   0x00000002 /*
    EVENT_WRITE_FLAG_INPRIVATE indicates that an event should be dropped on any
    logger the uses EVENT_ENABLE_PROPERTY_EXCLUDE_INPRIVATE. */

/*
A REGHANDLE represents an event provider session.
A REGHANDLE is opened with EventRegister and closed with EventUnregister.
*/
typedef ULONGLONG REGHANDLE, *PREGHANDLE;

#pragma warning(push)
#pragma warning(disable:4201) // Nonstandard extension: unnamed struct/union

/*
EVENT_DATA_DESCRIPTOR is used with EventWrite to provide user data items.
*/
typedef struct _EVENT_DATA_DESCRIPTOR {

    ULONGLONG       Ptr;   // Pointer to data. Set to (ULONGLONG)(ULONG_PTR)pData.
    ULONG           Size;  // Size of data in bytes.
    union {
        ULONG       Reserved; // Set to 0 if not using a special data item type.
        struct {
            UCHAR   Type;     // Default is EVENT_DATA_DESCRIPTOR_TYPE_NONE, meaning normal payload data.
            UCHAR   Reserved1;
            USHORT  Reserved2;
        } DUMMYSTRUCTNAME;
    } DUMMYUNIONNAME;

} EVENT_DATA_DESCRIPTOR, *PEVENT_DATA_DESCRIPTOR;

#pragma warning(pop) // 4201


#ifndef EVENT_DESCRIPTOR_DEF
#define EVENT_DESCRIPTOR_DEF

/*
EVENT_DESCRIPTOR describes and categorizes an event.
Note that for TraceLogging events, the Id and Version fields are not
meaningful and should be ignored.
*/
typedef struct _EVENT_DESCRIPTOR {

    USHORT Id; /*
        For manifest-based events, the Provider.Guid + Event.Id + Event.Version
        should uniquely identify an event. Once a manifest with a particular
        event Id+Version has been made public, the definition of that event
        (the types, ordering, and semantics of the fields) should never be
        changed. If an event needs to be changed, it must be given a new
        identity (usually by incrementing the Version), and the original event
        must remain in the manifest (so that older versions of the event can
        still be decoded with the new manifest). To change an event (e.g. to
        add/remove a field or to change a field type): duplicate the event in
        the manifest, then increment the event Version and make changes in the
        new copy.
        For manifest-free events (i.e. TraceLogging), Event.Id and
        Event.Version are not useful and should be ignored. Use Event name,
        level, keyword, and opcode for event filtering and identification. */
    UCHAR Version; /*
        For manifest-based events, the Provider.Guid + Event.Id + Event.Version
        should uniquely identify an event. The Id+Version constitute a 24-bit
        identifier. Generally, events with the same Id are semantically
        related, and the Version is incremented as the event is refined over
        time. */
    UCHAR Channel; /*
        The meaning of the Channel field depends on the event consumer.
        This field is most commonly used with events that will be consumed by
        the Windows Event Log. Note that Event Log does not listen to all ETW
        events, so setting a channel is not enough to make the event appear in
        the Event Log. For an ETW event to be routed to Event Log, the
        following must be configured:
        - The provider and its channels must be defined in a manifest.
        - The manifest must be compiled with the mc.exe tool, and the resulting
          BIN files must be included into the resources of an EXE or DLL.
        - The EXE or DLL containing the BIN data must be installed on the
          machine where the provider will run.
        - The manifest must be registered (using wevtutil.exe) on the machine
          where the provider will run. The manifest registration process must
          record the location of the EXE or DLL with the BIN data.
        - The channel must be enabled in Event Log configuration.
        - The provider must log an event with the channel and keyword set
          correctly as defined in the manifest. (Note that the mc.exe code
          generator will automatically define an implicit keyword for each
          channel, and will automatically add the channel's implicit keyword to
          each event that references a channel.) */
    UCHAR Level; /*
        The event level defines the event's severity or importance and is a
        primary means for filtering events. Microsoft-defined levels (in
        evntrace.h and  winmeta.h) are 1 (critical/fatal), 2 (error),
        3 (warning), 4 (information), and 5 (verbose). Levels 6-9 are reserved.
        Level 0 means the event is always-on (will not be filtered by level).
        For a provider, a lower level means the event is more important. An
        event with level 0 will always pass any level-based filtering.
        For a consumer, a lower level means the session's filter is more
        restrictive. However, setting a session's level to 0 disables level
        filtering (i.e. session level 0 is the same as session level 255). */
    UCHAR Opcode; /*
        The event opcode is used to mark events with special semantics that
        may be used by event decoders to organize and correlate events.
        Globally-recognized opcode values are defined in winmeta.h. A provider
        can define its own opcodes. Most events use opcode 0 (information).
        The opcodes 1 (start) and 2 (stop) are used to indicate the beginning
        and end of an activity as follows:
        - Generate a new activity Id (UuidCreate or EventActivityIdControl).
        - Write an event with opcode = start, activity ID = (the generated
          activity ID), and related activity ID = (the parent activity if any).
        - Write any number of informational events with opcode = info, activity
          ID = (the generated activity ID).
        - Write a stop event with opcode = stop, activity ID = (the generated
          activity ID).
        Each thread has an implicit activity ID (in thread-local storage) that
        will be applied to any event that does not explicitly specify an
        activity ID. The implicit activity ID can be accessed using
        EventActivityIdControl. It is intended that the thread-local activity
        will be used to implement scope-based activities: on entry to a scope
        (i.e. at the start of a function), a user will record the existing
        value of the implicit activity ID, generate and set a new value, and
        write a start event; on exit from the scope, the user will write a stop
        event and restore the previous activity ID. Note that there is no enforcement
        of this pattern, and an application must be aware that other code may
        potentially overwrite the activity ID without restoring it. In
        addition, the implicit activity ID does not work well with cross-thread
        activities. For these reasons, it may be more appropriate to use
        explicit activity IDs (explicitly pass a GUID to EventWriteTransfer)
        instead of relying on the implicity activity ID. */
    USHORT      Task; /*
        The event task code can be used for any purpose as defined by the
        provider. The task code 0 is the default, used to indicate that no
        special task code has been assigned to the event. The ETW manifest
        supports assigning localizable strings for each task code. The task
        code might be used to group events into categories, or to simply
        associate a task name with each event. */
    ULONGLONG   Keyword; /*
        The event keyword defines membership in various categories and is an
        important means for filtering events. The event's keyword is a set of
        64 bits indicating the categories to which an event belongs. The
        provider manifest may provide definitions for up to 48 keyword values,
        each value defining the meaning of a single keyword bit (the upper 16
        bits are reserved by Microsoft for special purposes). For example, if
        the provider manifest defines keyword 0x0010 as "Networking", and
        defines keyword 0x0020 as "Threading", an event with keyword 0x0030
        would be in both "Networking" and "Threading" categories, while an
        event with keyword 0x0001 would be in neither category. An event with
        keyword 0 is treated as uncategorized.
        Event consumers can use keyword masks to determine which events should
        be included in the log. A session can define a KeywordAny mask and
        a KeywordAll mask. An event will pass the session's keyword filtering
        if the following expression is true:
            event.Keyword == 0 || (
            (event.Keyword & session.KeywordAny) != 0 &&
            (event.Keyword & session.KeywordAll) == session.KeywordAll).
        In other words, uncategorized events (events with no keywords set)
        always pass keyword filtering, and categorized events pass if they
        match any keywords in KeywordAny and match all keywords in KeywordAll.
        */

} EVENT_DESCRIPTOR, *PEVENT_DESCRIPTOR;

typedef const EVENT_DESCRIPTOR *PCEVENT_DESCRIPTOR;

#endif

/*
EVENT_FILTER_DESCRIPTOR describes a filter data item for EnableTraceEx2.
*/
typedef struct _EVENT_FILTER_DESCRIPTOR {

    ULONGLONG   Ptr;  // Pointer to filter data. Set to (ULONGLONG)(ULONG_PTR)pData.
    ULONG       Size; // Size of filter data in bytes.
    ULONG       Type; // EVENT_FILTER_TYPE value.

} EVENT_FILTER_DESCRIPTOR, *PEVENT_FILTER_DESCRIPTOR;

/*
Defines the header data that must precede the filter data.
The filter data layout is defined in the instrumentation manifest.
*/
typedef struct _EVENT_FILTER_HEADER {

    USHORT     Id;
    UCHAR      Version;
    UCHAR      Reserved[5];
    ULONGLONG  InstanceId;
    ULONG      Size;
    ULONG      NextOffset;

} EVENT_FILTER_HEADER, *PEVENT_FILTER_HEADER;

/*
EVENT_FILTER_EVENT_ID is used to pass EventId filter for
stack walk filters.
*/
typedef struct _EVENT_FILTER_EVENT_ID {
    BOOLEAN FilterIn;
    UCHAR Reserved;
    USHORT Count;
    USHORT Events[ANYSIZE_ARRAY];
} EVENT_FILTER_EVENT_ID, *PEVENT_FILTER_EVENT_ID;

/*
EVENT_FILTER_EVENT_NAME is used to pass the EventName filter.

This filter will only be applied to events that are otherwise enabled
on the logging session (via level/keyword in the enable call).

The Level, MatchAnyKeyword, and MatchAllKeyword fields in the filter
determine which of those otherwise enabled events the filter is applied to.

FilterIn determines whether the events matching the provided names will be
filtered in or filtered out.  When used for the EVENT_FILTER_TYPE_STACKWALK_NAME
filter type, the filtered in events will have stacks collected for them.

The Names field should be a series of NameCount nul-terminated utf-8
event names.
*/
typedef struct _EVENT_FILTER_EVENT_NAME {
    ULONGLONG MatchAnyKeyword;
    ULONGLONG MatchAllKeyword;
    UCHAR Level;
    BOOLEAN FilterIn;
    USHORT NameCount;
    UCHAR Names[ANYSIZE_ARRAY]; // utf-8
} EVENT_FILTER_EVENT_NAME, *PEVENT_FILTER_EVENT_NAME;

/*
EVENT_FILTER_LEVEL_KW is used to pass the StackWalk Level-Keyword filter.

This filter will only be applied to events that are otherwise enabled
on the logging session (via level/keyword in the enable call).

The Level, MatchAnyKeyword, and MatchAllKeyword fields in the filter
determine which of these events should have the filter applied to it.

FilterIn determines whether the events matching the provided names will be
filtered in or filtered out.  Those filtered in will have Stacks collected.
*/
typedef struct _EVENT_FILTER_LEVEL_KW {
    ULONGLONG MatchAnyKeyword;
    ULONGLONG MatchAllKeyword;
    UCHAR Level;
    BOOLEAN FilterIn;
} EVENT_FILTER_LEVEL_KW, *PEVENT_FILTER_LEVEL_KW;

#ifndef MIDL_PASS

/*
EVENT_INFO_CLASS values for the InformationClass parameter of
EventSetInformation.
*/
typedef enum _EVENT_INFO_CLASS {
    EventProviderBinaryTrackInfo, /*
        Requests that the ETW runtime add the full path to the binary that
        registered the provider into each trace. The full path is important if
        if the binary contains the mc.exe-generated decoding resources but is
        not globally registered. Decoding tools can use the path to locate the
        binary and extract the decoding resources. */
    EventProviderSetReserved1, /*
        Not used. */
    EventProviderSetTraits, /*
        Provides the ETW runtime with additional information about the
        provider, potentially including the provider name and a group GUID.
        Refer the the MSDN Provider Traits topic for more information about the
        format of the data to be used with this control code.
        Setting this trait also configures the ETW runtime to respect the
        Type field of EVENT_DATA_DESCRIPTOR (by default the Type field is
        ignored). */
    EventProviderUseDescriptorType, /*
        Configures whether the ETW runtime should respect the Type field of the
        EVENT_DATA_DESCRIPTOR. The data for this control code is a BOOLEAN
        (1 byte, value FALSE or TRUE). */
    MaxEventInfo
} EVENT_INFO_CLASS;

#if !defined(_ETW_KM_) || defined(_EVNT_SOURCE_)

/*
Optional callback function that users provide to EventRegister.
(Refer to MSDN documentation for EnableCallback.)

The ETW runtime will invoke this function to send notifications to a provider.
These notifications include the following values of the IsEnabled parameter:

- EVENT_CONTROL_CODE_DISABLE_PROVIDER (0): No sessions are listening for events
  from this provider.
- EVENT_CONTROL_CODE_ENABLE_PROVIDER (1): At least one session is listening for
  events from this provider. This notification may be sent multiple times if
  multiple different providers are listening, if a provider changes its level
  or keyword masks, or if a provider updates its filter data.
- EVENT_CONTROL_CODE_CAPTURE_STATE (2): A session is requesting that this
  provider log its state.

Note that for the ENABLE_PROVIDER and CAPTURE_STATE notifications, the ETW
runtime will adjust the Level, MatchAnyKeyword, and MatchAllKeyword parameters
to the least-restrictive values set by any subscribed session. The callback is
not responsible for maintaining these values on a per-session basis.

The provider can use these notifications to perform efficient event filtering.
While such filtering is optional (the ETW runtime will perform the same
filtering), filtering within the provider is usually much more efficient than
relying on ETW's internal filtering. Filtering within the provider usually
means checking a few global variables and can be done before preparing the
data to be sent to ETW. ETW's internal filtering always occurs after the data
has been prepared, and it requires looking up the provider's handle in a table.

The provider can use the CAPTURE_STATE notification to log non-event
information (such as performance counters or system status) on-demand.
If the provider has no such state, it can ignore any CAPTURE_STATE events.

Assuming that the callback saves the values of IsEnabled, Level,
MatchAnyKeyword, and MatchAllKeyword in global variables each time a
DISABLE_PROVIDER or ENABLE_PROVIDER notification occurs, an event would be
filtered as follows:

if (g_IsEnabled != 0 &&       // Provider is enabled
    event.Level <= g_Level && // Event passes level filter
    (event.Keyword == 0 || (  // Event is uncategorized or passes keyword filter
     (event.Keyword & g_MatchAnyKeyword) != 0 &&
     (event.Keyword & g_MatchAllKeyword) == g_MatchAllKeyword)))
{
    PrepareEventData(...);
    EventWrite(...);
}

Note that the code generated by mc.exe includes an efficient callback routine
and optimized event filtering system that implements the behavior described
above. The mc.exe-generated callback will be registered if you use the
mc.exe-generated EventRegister[ProviderName] macro to register the provider.
Similarly, when using TraceLoggingProvider.h, TraceLoggingRegister will
install its own callback routine so that it can efficiently filter each
TraceLoggingWrite event.

If filtering performance is not critical, but you still want to avoid
preparing data unnecessarily, you can eliminate the need for a callback by
using the EventEnabled or EventProviderEnabled routines. These routines still
require ETW to perform a table lookup, but they can help you avoid the overhead
of preparing your data without needing to register a callback. In that case,
the above code would be replaced with something like this:

if (EventProviderEnabled(regHandle, event.Level, event.Keyword))
{
    PrepareEventData(...);
    EventWrite(...);
}
*/
typedef
VOID
(NTAPI *PENABLECALLBACK) (
    _In_ LPCGUID SourceId,
    _In_ ULONG IsEnabled,
    _In_ UCHAR Level,
    _In_ ULONGLONG MatchAnyKeyword,
    _In_ ULONGLONG MatchAllKeyword,
    _In_opt_ PEVENT_FILTER_DESCRIPTOR FilterData,
    _Inout_opt_ PVOID CallbackContext
    );

#ifndef _APISET_EVENTING

#pragma region Application Family or OneCore Family or GameCore Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)

//
// Registration APIs
//

#if (WINVER >= _WIN32_WINNT_VISTA)

/*
Registers a provider with the ETW runtime and obtains a handle to use with
calls to EventWrite.

Note that the returned RegHandle must be closed by calling EventUnregister.
This is especially important if the provider is registered by DLL. If the DLL
unloads without closing the handle, the process may crash when the ETW runtime
attempts to invoke the provider's callback after the has unloaded.
*/
ULONG
EVNTAPI
EventRegister(
    _In_ LPCGUID ProviderId,
    _In_opt_ PENABLECALLBACK EnableCallback,
    _In_opt_ PVOID CallbackContext,
    _Out_ PREGHANDLE RegHandle
    );
#endif

#if (WINVER >= _WIN32_WINNT_VISTA)
/*
Closes the RegHandle that was opened by EventRegister.
*/
ULONG
EVNTAPI
EventUnregister(
    _In_ REGHANDLE RegHandle
    );
#endif

#if (WINVER >= _WIN32_WINNT_WIN8)
/*
Used to provide the ETW runtime with additional information about your provider
or to configure how the ETW runtime handles your provider's events.
Refer to the documentation of the EVENT_INFO_CLASS for more information.
*/
ULONG
EVNTAPI
EventSetInformation(
    _In_ REGHANDLE RegHandle,
    _In_ EVENT_INFO_CLASS InformationClass,
    _In_reads_bytes_(InformationLength) PVOID EventInformation,
    _In_ ULONG InformationLength
    );
#endif

#if (WINVER >= _WIN32_WINNT_VISTA)
/*
Used to check whether any sessions are enabled to receive an event with the
Level and Keyword specified in the given EventDescriptor.

Note that this function is not as efficient as the checks built into most
ETW frameworks (e.g. the checks implemented by mc.exe's generated code or by
TraceLoggingProvider.h). This function must look up the RegHandle state, while
the ETW frameworks can usually perform a direct check against global variables.
If using an ETW framework, use the framework's test APIs in preference to this
function.

This function can be used to avoid performing expensive preparation before
calling EventWrite, i.e. to avoid spending CPU cycles looking up data and
packing it into EVENT_DATA_DESCRIPTORs.

// This is a good idea if PrepareEventData(...) is expensive:
if (EventEnabled(regHandle, &myEventDesc))
{
    PrepareEventData(...);
    EventWrite(regHandle, &myEventDesc, ...);
}

It is not necessary or efficient to use this function simply to avoid a call to
EventWrite. The EventWrite function will automatically perform an EventEnabled
check as its first operation.

// This is unnecessary complexity and does not improve performance:
if (EventEnabled(regHandle, &myEventDesc))
{
    EventWrite(regHandle, &myEventDesc, ...);
}
*/
BOOLEAN
EVNTAPI
EventEnabled(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR EventDescriptor
    );
#endif

#if (WINVER >= _WIN32_WINNT_VISTA)
/*
Used to check whether any sessions are enabled to receive an event with the
given Level and Keyword.

Note that this function is not as efficient as the checks built into most
ETW frameworks (e.g. the checks implemented by mc.exe's generated code or by
TraceLoggingProvider.h). This function must look up the RegHandle state, while
the ETW frameworks can usually perform a direct check against global variables.
If using an ETW framework, use the framework's test APIs in preference to this
function.

This function can be used to avoid performing expensive preparation before
calling EventWrite, i.e. to avoid spending CPU cycles looking up data and
packing it into EVENT_DATA_DESCRIPTORs.

// This is a good idea if PrepareEventData(...) is expensive:
if (EventProviderEnabled(regHandle, myEventDesc.Level, myEventDesc.Keyword))
{
    PrepareEventData(...);
    EventWrite(regHandle, &myEventDesc, ...);
}

It is not necessary or efficient to use this function simply to avoid a call to
EventWrite. The EventWrite function will automatically perform an EventEnabled
check as its first operation.

// This is unnecessary complexity and does not improve performance:
if (EventProviderEnabled(regHandle, myEventDesc.Level, myEventDesc.Keyword))
{
    EventWrite(regHandle, &myEventDesc, ...);
}
*/
BOOLEAN
EVNTAPI
EventProviderEnabled(
    _In_ REGHANDLE RegHandle,
    _In_ UCHAR Level,
    _In_ ULONGLONG Keyword
    );
#endif

//
// Writing (Publishing/Logging) APIs
//

#if (WINVER >= _WIN32_WINNT_VISTA)
/*
Logs an ETW event.

Note that this function will use the implicit thread activity ID (from
thread-local storage) as the event's activity ID.

UserData may be NULL if UserDataCount is 0.

EventWrite(regHandle, &eventDesc, cData, pData) is equivalent to
EventWriteTransfer(regHandle, &eventDesc, NULL, NULL, cData, pData).
*/
ULONG
EVNTAPI
EventWrite(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR EventDescriptor,
    _In_range_(0, MAX_EVENT_DATA_DESCRIPTORS) ULONG UserDataCount,
    _In_reads_opt_(UserDataCount) PEVENT_DATA_DESCRIPTOR UserData
    );
#endif

#if (WINVER >= _WIN32_WINNT_VISTA)
/*
Logs an ETW event, optionally specifying activity IDs.

If the ActivityId parameter is NULL, this function will use the implicit thread
activity ID (from thread-local storage) as the event's activity ID.

If the RelatedActivityId parameter is NULL, the event will not include a
related activity ID.

UserData may be NULL if UserDataCount is 0.

EventWriteTransfer(regHandle, &eventDesc, pAI, pRAI, cData, pData) is equivalent to
EventWriteEx(regHandle, &eventDesc, 0, 0, pAI, pRAI, cData, pData).
*/
ULONG
EVNTAPI
EventWriteTransfer(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR EventDescriptor,
    _In_opt_ LPCGUID ActivityId,
    _In_opt_ LPCGUID RelatedActivityId,
    _In_range_(0, MAX_EVENT_DATA_DESCRIPTORS) ULONG UserDataCount,
    _In_reads_opt_(UserDataCount) PEVENT_DATA_DESCRIPTOR UserData
    );
#endif

#if (WINVER >= _WIN32_WINNT_WIN7)
/*
Logs an ETW event, optionally specifying activity IDs, filter, and flags.

The Filter parameter specifies sessions to which the event should not be
written. This should be set to 0 unless the provider is doing provider-size
filtering based on the FilterData parameter of the provider callback.

The Flags parameter adds special behaviors to the event. Use EVENT_WRITE_FLAG
values, or 0 for no special behaviors.

If the ActivityId parameter is NULL, this function will use the implicit thread
activity ID (from thread-local storage) as the event's activity ID.

If the RelatedActivityId parameter is NULL, the event will not include a
related activity ID.

UserData may be NULL if UserDataCount is 0.

EventWriteTransfer(regHandle, &eventDesc, pAI, pRAI, cData, pData) is equivalent to
EventWriteEx(regHandle, &eventDesc, 0, 0, pAI, pRAI, cData, pData).
*/
ULONG
EVNTAPI
EventWriteEx(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR EventDescriptor,
    _In_ ULONG64 Filter,
    _In_ ULONG Flags,
    _In_opt_ LPCGUID ActivityId,
    _In_opt_ LPCGUID RelatedActivityId,
    _In_range_(0, MAX_EVENT_DATA_DESCRIPTORS) ULONG UserDataCount,
    _In_reads_opt_(UserDataCount) PEVENT_DATA_DESCRIPTOR UserData
    );
#endif

#if (WINVER >= _WIN32_WINNT_VISTA)
/*
Writes a simple event containing a string.
*/
ULONG
EVNTAPI
EventWriteString(
    _In_ REGHANDLE RegHandle,
    _In_ UCHAR Level,
    _In_ ULONGLONG Keyword,
    _In_ PCWSTR String
    );
#endif

#if (WINVER >= _WIN32_WINNT_VISTA)
/*
Performs operations on activity IDs.

Use EVENT_ACTIVITY_CTRL values for the ControlCode parameter.

Depending on the ControlCode, this function does one or more of the following:

- Generate a new activity ID.
- Get the implicit thread activity ID.
- Set the implicit thread activity ID.

Generating an activity ID using this API is faster than generating a GUID with
UuidCreate. The IDs generated by this API are not GUIDs -- they are only
guaranteed to be unique on the local machine and only during the current boot
session.
*/
ULONG
EVNTAPI
EventActivityIdControl(
    _In_ ULONG ControlCode,
    _Inout_ LPGUID ActivityId
    );
#endif

#endif /* WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES) */
#pragma endregion

#endif // _APISET_EVENTING

#endif // _ETW_KM_

#pragma region Application Family or OneCore Family or GameCore Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)
/*
Initializes an EVENT_DATA_DESCRIPTOR with the given values.
*/
EVNTPROV_PFORCEINLINE
VOID
EventDataDescCreate(
    _Out_ PEVENT_DATA_DESCRIPTOR EventDataDescriptor,
    _In_reads_bytes_(DataSize) const VOID* DataPtr,
    _In_ ULONG DataSize
    )
{
    EventDataDescriptor->Ptr = (ULONGLONG)(ULONG_PTR)DataPtr;
    EventDataDescriptor->Size = DataSize;
    EventDataDescriptor->Reserved = 0; // Reserved MUST be initialized.
    return;
}

#endif /* WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES) */
#pragma endregion

#pragma region Desktop Family or OneCore Family or GameCore Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)

/*
Initializes an EVENT_DESCRIPTOR with the given values.
*/
EVNTPROV_PFORCEINLINE
VOID
EventDescCreate(
    _Out_ PEVENT_DESCRIPTOR EventDescriptor,
    _In_ USHORT Id,
    _In_ UCHAR Version,
    _In_ UCHAR Channel,
    _In_ UCHAR Level,
    _In_ USHORT Task,
    _In_ UCHAR Opcode,
    _In_ ULONGLONG Keyword
    )
{
    EventDescriptor->Id = Id;
    EventDescriptor->Version = Version;
    EventDescriptor->Channel = Channel;
    EventDescriptor->Level = Level;
    EventDescriptor->Task = Task;
    EventDescriptor->Opcode = Opcode;
    EventDescriptor->Keyword = Keyword;
    return;
}

/*
Initializes an EVENT_DESCRIPTOR. Sets all values to 0.
*/
EVNTPROV_PFORCEINLINE
VOID
EventDescZero(
    _Out_ PEVENT_DESCRIPTOR EventDescriptor
    )
{
    memset(EventDescriptor, 0, sizeof(EVENT_DESCRIPTOR));
    return;
}

//
// Macros to extract info from an Event Descriptor
//

EVNTPROV_PFORCEINLINE
USHORT
EventDescGetId(
    _In_ PCEVENT_DESCRIPTOR EventDescriptor
    )
{
    return (EventDescriptor->Id);
}

EVNTPROV_PFORCEINLINE
UCHAR
EventDescGetVersion(
    _In_ PCEVENT_DESCRIPTOR EventDescriptor
    )
{
    return (EventDescriptor->Version);
}

EVNTPROV_PFORCEINLINE
USHORT
EventDescGetTask(
    _In_ PCEVENT_DESCRIPTOR EventDescriptor
    )
{
    return (EventDescriptor->Task);
}

EVNTPROV_PFORCEINLINE
UCHAR
EventDescGetOpcode(
    _In_ PCEVENT_DESCRIPTOR EventDescriptor
    )
{
    return (EventDescriptor->Opcode);
}

EVNTPROV_PFORCEINLINE
UCHAR
EventDescGetChannel(
    _In_ PCEVENT_DESCRIPTOR EventDescriptor
    )
{
    return (EventDescriptor->Channel);
}

EVNTPROV_PFORCEINLINE
UCHAR
EventDescGetLevel(
    _In_ PCEVENT_DESCRIPTOR EventDescriptor
    )
{
    return (EventDescriptor->Level);
}

EVNTPROV_PFORCEINLINE
ULONGLONG
EventDescGetKeyword(
    _In_ PCEVENT_DESCRIPTOR EventDescriptor
    )
{
    return (EventDescriptor->Keyword);
}

//
// Macros to set info into an Event Descriptor
//

EVNTPROV_PFORCEINLINE
PEVENT_DESCRIPTOR
EventDescSetId(
    _In_ PEVENT_DESCRIPTOR EventDescriptor,
    _In_ USHORT Id
    )
{
    EventDescriptor->Id         = Id;
    return (EventDescriptor);
}

EVNTPROV_PFORCEINLINE
PEVENT_DESCRIPTOR
EventDescSetVersion(
    _In_ PEVENT_DESCRIPTOR EventDescriptor,
    _In_ UCHAR Version
    )
{
    EventDescriptor->Version    = Version;
    return (EventDescriptor);
}

EVNTPROV_PFORCEINLINE
PEVENT_DESCRIPTOR
EventDescSetTask(
    _In_ PEVENT_DESCRIPTOR EventDescriptor,
    _In_ USHORT Task
    )
{
    EventDescriptor->Task       = Task;
    return (EventDescriptor);
}

EVNTPROV_PFORCEINLINE
PEVENT_DESCRIPTOR
EventDescSetOpcode(
    _In_ PEVENT_DESCRIPTOR EventDescriptor,
    _In_ UCHAR Opcode
    )
{
    EventDescriptor->Opcode     = Opcode;
    return (EventDescriptor);
}

EVNTPROV_PFORCEINLINE
PEVENT_DESCRIPTOR
EventDescSetLevel(
    _In_ PEVENT_DESCRIPTOR EventDescriptor,
    _In_ UCHAR  Level
    )
{
    EventDescriptor->Level      = Level;
    return (EventDescriptor);
}

EVNTPROV_PFORCEINLINE
PEVENT_DESCRIPTOR
EventDescSetChannel(
    _In_ PEVENT_DESCRIPTOR EventDescriptor,
    _In_ UCHAR Channel
    )
{
    EventDescriptor->Channel    = Channel;
    return (EventDescriptor);
}

EVNTPROV_PFORCEINLINE
PEVENT_DESCRIPTOR
EventDescSetKeyword(
    _In_ PEVENT_DESCRIPTOR EventDescriptor,
    _In_ ULONGLONG Keyword
    )
{
    EventDescriptor->Keyword    = Keyword;
    return (EventDescriptor);
}


EVNTPROV_PFORCEINLINE
PEVENT_DESCRIPTOR
EventDescOrKeyword(
    _In_ PEVENT_DESCRIPTOR EventDescriptor,
    _In_ ULONGLONG Keyword
    )
{
    EventDescriptor->Keyword    |= Keyword;
    return (EventDescriptor);
}

#endif /* WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES) */
#pragma endregion



#endif // MIDL_PASS

#ifdef __cplusplus
}
#endif

#if _MSC_VER >= 1200
#pragma warning(pop)
#endif

#endif

