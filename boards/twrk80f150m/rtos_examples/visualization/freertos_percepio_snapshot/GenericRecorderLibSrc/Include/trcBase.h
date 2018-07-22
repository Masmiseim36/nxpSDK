/*******************************************************************************
 * Tracealyzer / FreeRTOS+Trace - Snapshot Trace Recorder Library v3.0.9
 * Percepio AB, www.percepio.com
 *
 * trcBase.h
 *
 * Core functionality of the Tracealyzer recorder library.
 *
 *******************************************************************************
 * Copyright (c) 2016, Percepio AB.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 *
 * Tabs are used for indent in this file (1 tab = 4 spaces)
 ******************************************************************************/
 
#ifndef TRCBASE_H
#define TRCBASE_H

#define TRACE_MINOR_VERSION 4
#define TRACE_STORE_MODE_STOP_WHEN_FULL 1
#define TRACE_STORE_MODE_RING_BUFFER 2
#define TRACE_DATA_ALLOCATION_STATIC 1
#define TRACE_DATA_ALLOCATION_DYNAMIC 2
#define TRACE_DATA_ALLOCATION_CUSTOM 3

#include "trcKernelPort.h"

#if (USE_TRACEALYZER_RECORDER == 1)

#include <stdio.h>
#include <string.h>

#ifndef USE_SEPARATE_USER_EVENT_BUFFER
#define USE_SEPARATE_USER_EVENT_BUFFER 0
#endif

/* Max number of event codes supported */
#define NEventCodes 0x100

/* Keeps track of the recorder's critical sections */
extern volatile int recorder_busy;

/* Our local critical sections for the recorder */
#define trcCRITICAL_SECTION_BEGIN() {TRACE_ENTER_CRITICAL_SECTION(); recorder_busy++;}
#define trcCRITICAL_SECTION_END() {recorder_busy--; TRACE_EXIT_CRITICAL_SECTION();}

#if (SELECTED_PORT == PORT_ARM_CortexM || SELECTED_PORT == PORT_ARM_CortexM_SysTick)
	#define trcSR_ALLOC_CRITICAL_SECTION_ON_CORTEX_M_ONLY TRACE_SR_ALLOC_CRITICAL_SECTION
	#define trcCRITICAL_SECTION_BEGIN_ON_CORTEX_M_ONLY trcCRITICAL_SECTION_BEGIN
	#define trcCRITICAL_SECTION_END_ON_CORTEX_M_ONLY trcCRITICAL_SECTION_END
#else
	#define trcSR_ALLOC_CRITICAL_SECTION_ON_CORTEX_M_ONLY() 
	#define trcCRITICAL_SECTION_BEGIN_ON_CORTEX_M_ONLY() recorder_busy++;
	#define trcCRITICAL_SECTION_END_ON_CORTEX_M_ONLY() recorder_busy--;
#endif

/* Structure to handle the exclude flags for all objects and tasks. We add some extra objects since index 0 is not used for each object class. */
extern uint8_t excludedObjects[(TRACE_KERNEL_OBJECT_COUNT + TRACE_NCLASSES) / 8 + 1];

/* Structure to handle the exclude flags for all event codes */
extern uint8_t excludedEventCodes[NEventCodes / 8 + 1];

/******************************************************************************
 * ObjectHandleStack
 * This data-structure is used to provide a mechanism for 1-byte trace object
 * handles. This way, only 1 byte is necessary instead of 4 bytes (a pointer)
 * when storing a reference to an object. This allows for up to 255 objects of
 * each object class active at any given moment. There can be more "historic"
 * objects, that have been deleted - that number is only limited by the size of
 * the symbol table.
 * Note that handle zero (0) is not used, it is a code for an invalid handle.
 *
 * This data structure keeps track of the FREE handles, not the handles in use.
 * This data structure contains one stack per object class. When a handle is
 * allocated to an object, the next free handle is popped from the stack. When
 * a handle is released (on object delete), it is pushed back on the stack.
 * Note that there is no initialization code that pushed the free handles
 * initially, that is not necessary due to the following optimization:
 *
 * The stack of handles (objectHandles) is initially all zeros. Since zero
 * is not a valid handle, that is a signal of additional handles needed.
 * If a zero is received when popping a new handle, it is replaced by the
 * index of the popped handle instead.
 *
 *****************************************************************************/
typedef struct
{
	/* For each object class, the index of the next handle to allocate */
	uint16_t indexOfNextAvailableHandle[ TRACE_NCLASSES ];

	/* The lowest index of this class (constant) */
	uint16_t lowestIndexOfClass[ TRACE_NCLASSES ];

	/* The highest index of this class (constant) */
	uint16_t highestIndexOfClass[ TRACE_NCLASSES ];

	/* The highest use count for this class (for statistics) */
	uint16_t handleCountWaterMarksOfClass[ TRACE_NCLASSES ];

	/* The free object handles - a set of stacks within this array */
	objectHandleType objectHandles[ TRACE_KERNEL_OBJECT_COUNT ];

} objectHandleStackType;

extern objectHandleStackType objectHandleStacks;

/******************************************************************************
 * Object Property Table
 * The Object Table contains name and other properties of the objects (tasks,
 * queues, mutexes, etc). The below data structures defines the properties of
 * each object class and are used to cast the byte buffer into a cleaner format.
 *
 * The values in the object table are continuously overwritten and always
 * represent the current state. If a property is changed during runtime, the OLD
 * value should be stored in the trace buffer, not the new value (since the new
 * value is found in the Object Property Table).
 * For close events this mechanism is the old names are stored in the symbol
 * table), for "priority set" (the old priority is stored in the event data)
 * and for "isActive", where the value decides if the task switch event type
 * should be "new" or "resume".
 ******************************************************************************/

typedef struct
{
	/* = NCLASSES */
	uint32_t NumberOfObjectClasses;

	uint32_t ObjectPropertyTableSizeInBytes;

	/* This is used to calculate the index in the dynamic object table
	(handle - 1 - nofStaticObjects = index)*/
#if (USE_16BIT_OBJECT_HANDLES == 1)
	objectHandleType NumberOfObjectsPerClass[2*((TRACE_NCLASSES+1)/2)];
#else
	objectHandleType NumberOfObjectsPerClass[4*((TRACE_NCLASSES+3)/4)];
#endif

	/* Allocation size rounded up to the closest multiple of 4 */
	uint8_t NameLengthPerClass[ 4*((TRACE_NCLASSES+3)/4) ];

	uint8_t TotalPropertyBytesPerClass[ 4*((TRACE_NCLASSES+3)/4) ];

	/* Allocation size rounded up to the closest multiple of 2 */
	uint16_t StartIndexOfClass[ 2*((TRACE_NCLASSES+1)/2) ];

	/* The actual handles issued, should be Initiated to all zeros */
	uint8_t objbytes[ 4*((TRACE_OBJECT_TABLE_SIZE+3)/4) ];
} ObjectPropertyTableType;

/* Symbol table data structure */
typedef struct
{
	/* = SYMBOL_HISTORY_TABLE_SIZE_IN_BYTES */
	uint32_t symTableSize;

	/* Entry 0 is reserved. Any reference to entry 0 implies NULL*/
	uint32_t nextFreeSymbolIndex;

	/* Size rounded up to closest multiple of 4, to avoid alignment issues*/
	uint8_t symbytes[4*((SYMBOL_TABLE_SIZE+3)/4)];

	/* Used for lookups - Up to 64 linked lists within the symbol table
	connecting all entries with the same 6 bit checksum.
	This field holds the current list heads. Should be initiated to zeros */
	uint16_t latestEntryOfChecksum[64];
} symbolTableType;


/*******************************************************************************
 * The data structures of the different events, all 4 bytes long
 ******************************************************************************/

typedef struct
{
	uint8_t type;
	uint8_t objHandle;
	uint16_t dts;	/* differential timestamp - time since last event */
} TSEvent, TREvent;

typedef struct
{
	uint8_t type;
	uint8_t dummy;
	uint16_t dts;	/* differential timestamp - time since last event */
} LPEvent;

typedef struct
{
	uint8_t type;
	uint8_t objHandle;
	uint16_t dts;	/* differential timestamp - time since last event */
} KernelCall;

typedef struct
{
	uint8_t type;
	uint8_t objHandle;
	uint8_t param;
	uint8_t dts;	/* differential timestamp - time since last event */
} KernelCallWithParamAndHandle;

typedef struct
{
	uint8_t type;
	uint8_t dts;	/* differential timestamp - time since last event */
	uint16_t param;
} KernelCallWithParam16;

typedef struct
{
	uint8_t type;
	uint8_t objHandle;	/* the handle of the closed object */
	uint16_t symbolIndex;		 /* the name of the closed object */
} ObjCloseNameEvent;

typedef struct
{
	uint8_t type;
	uint8_t arg1;
	uint8_t arg2;
	uint8_t arg3;
} ObjClosePropEvent;

typedef struct
{
	uint8_t type;
	uint8_t unused1;
	uint8_t unused2;
	uint8_t dts;
} TaskInstanceStatusEvent;

typedef struct
{
	uint8_t type;
	uint8_t dts;
	uint16_t payload;		 /* the name of the user event */
} UserEvent;

typedef struct
{
	uint8_t type;

	/* 8 bits extra for storing DTS, if it does not fit in ordinary event
	(this one is always MSB if used) */
	uint8_t xts_8;

	/* 16 bits extra for storing DTS, if it does not fit in ordinary event. */
	uint16_t xts_16;
} XTSEvent;

typedef struct
{
	uint8_t type;

	uint8_t xps_8;
	uint16_t xps_16;
} XPSEvent;

typedef struct{
	uint8_t type;
	uint8_t dts;
	uint16_t size;
} MemEventSize;

typedef struct{
	uint8_t type;
	uint8_t addr_high;
	uint16_t addr_low;
} MemEventAddr;

/*******************************************************************************
 * The separate user event buffer structure. Can be enabled in trcConfig.h.
 ******************************************************************************/

#if (USE_SEPARATE_USER_EVENT_BUFFER == 1)
typedef struct
{
	traceLabel name;
	traceLabel defaultFormat;
} ChannelFormatPair;

typedef struct
{
	uint16_t bufferID;
	uint16_t version;
	uint32_t wraparoundCounter;
	uint32_t numberOfSlots;
	uint32_t nextSlotToWrite;
	uint8_t numberOfChannels;
	uint8_t padding1;
	uint8_t padding2;
	uint8_t padding3;
	ChannelFormatPair channels[CHANNEL_FORMAT_PAIRS+1];
	uint8_t channelBuffer[(USER_EVENT_BUFFER_SIZE + 3) & 0xFFFFFFFC]; /* 1 byte per slot, with padding for 4 byte alignment */
	uint8_t dataBuffer[USER_EVENT_BUFFER_SIZE * 4]; /* 4 bytes per slot */

} UserEventBuffer;
#endif

/*******************************************************************************
 * The main data structure, read by Tracealyzer from the RAM dump
 ******************************************************************************/

typedef struct
{
	uint8_t startmarker0;
	uint8_t startmarker1;
	uint8_t startmarker2;
	uint8_t startmarker3;
	uint8_t startmarker4;
	uint8_t startmarker5;
	uint8_t startmarker6;
	uint8_t startmarker7;
	uint8_t startmarker8;
	uint8_t startmarker9;
	uint8_t startmarker10;
	uint8_t startmarker11;

	/* Used to determine Kernel and Endianess */
	uint16_t version;

	/* Currently 3 */
	uint8_t minor_version;

	/* This should be 0 if lower IRQ priority values implies higher priority
	levels, such as on ARM Cortex M. If the opposite scheme is used, i.e.,
	if higher IRQ priority values means higher priority, this should be 1. */
	uint8_t irq_priority_order;

	/* sizeof(RecorderDataType) - just for control */
	uint32_t filesize;

	/* Current number of events recorded */
	uint32_t numEvents;

	/* The buffer size, in number of event records */
	uint32_t maxEvents;

	/* The event buffer index, where to write the next event */
	uint32_t nextFreeIndex;

	/* 1 if the buffer is full, 0 otherwise */
	uint32_t bufferIsFull;

	/* The frequency of the clock/timer/counter used as time base */
	uint32_t frequency;

	/* The absolute timestamp of the last stored event, in the native
	timebase, modulo frequency! */
	uint32_t absTimeLastEvent;

	/* The number of seconds in total - lasts for 136 years */
	uint32_t absTimeLastEventSecond;

	/* 1 if the recorder has been started, 0 if not yet started or stopped.
	This is a 32 bit variable due to alignment issues. */
	uint32_t recorderActive;

	/* Not used, remains for compatibility and future use */
	uint8_t notused[28];

	/* The amount of heap memory remaining at the last malloc or free event */
	uint32_t heapMemUsage;

	/* 0xF0F0F0F0 - for control only */
	int32_t debugMarker0;

	/* Set to value of USE_16BIT_OBJECT_HANDLES */
	uint32_t isUsing16bitHandles;

	/* The Object Property Table holds information about currently active
	tasks, queues, and other recorded objects. This is updated on each
	create call and includes object name and other properties. */
	ObjectPropertyTableType ObjectPropertyTable;

	/* 0xF1F1F1F1 - for control only */
	int32_t debugMarker1;

	/* The Symbol Table stores strings for User Events and is also used to
	store names of deleted objects, which still may be in the trace but no
	longer are available. */
	symbolTableType SymbolTable;

	/* For inclusion of float support, and for endian detection of floats.
	The value should be (float)1 or (uint32_t)0 */
#if (INCLUDE_FLOAT_SUPPORT == 1)
	float exampleFloatEncoding;
#else
	uint32_t exampleFloatEncoding;
#endif
	/* This is non-zero if an internal error occurred in the recorder, e.g., if
	one of the Nxxx constants was too small. The systemInfo string will then
	contain an error message that is displayed when attempting to view the
	trace file. */
	uint32_t internalErrorOccured;

	/* 0xF2F2F2F2 - for control only */
	int32_t debugMarker2;

	/* Error messages from the recorder. */
	char systemInfo[80];

	/* 0xF3F3F3F3 - for control only */
	int32_t debugMarker3;

	/* The event data, in 4-byte records */
	uint8_t eventData[ EVENT_BUFFER_SIZE * 4 ];

#if (USE_SEPARATE_USER_EVENT_BUFFER == 1)
	UserEventBuffer userEventBuffer;
#endif

	/* This should always be 0 */
	uint32_t endOfSecondaryBlocks;

	uint8_t endmarker0;
	uint8_t endmarker1;
	uint8_t endmarker2;
	uint8_t endmarker3;
	uint8_t endmarker4;
	uint8_t endmarker5;
	uint8_t endmarker6;
	uint8_t endmarker7;
	uint8_t endmarker8;
	uint8_t endmarker9;
	uint8_t endmarker10;
	uint8_t endmarker11;
} RecorderDataType;

extern RecorderDataType* RecorderDataPtr;

/* Internal functions */

uint16_t prvTraceGetDTS(uint16_t param_maxDTS);

void prvTraceGetChecksum(const char *pname, uint8_t* pcrc, uint8_t* plength);

traceLabel prvTraceCreateSymbolTableEntry(const char* name,
										 uint8_t crc6,
										 uint8_t len,
										 traceLabel channel);

traceLabel prvTraceLookupSymbolTableEntry(const char* name,
										 uint8_t crc6,
										 uint8_t len,
										 traceLabel channel);

traceLabel prvTraceOpenSymbol(const char* name, traceLabel userEventChannel);

void prvTraceUpdateCounters(void);

void prvCheckDataToBeOverwrittenForMultiEntryEvents(uint8_t nEntries);

objectHandleType xTraceGetObjectHandle(traceObjectClass objectclass);

void vTraceFreeObjectHandle(traceObjectClass objectclass,
							objectHandleType handle);

void vTraceSetObjectName(traceObjectClass objectclass,
							objectHandleType handle,
							const char* name);

void* xTraceNextFreeEventBufferSlot(void);

#if (USE_16BIT_OBJECT_HANDLES == 1)
unsigned char prvTraceGet8BitHandle(objectHandleType handle);
#else
#define prvTraceGet8BitHandle(x) ((unsigned char)x)
#endif


uint16_t uiIndexOfObject(objectHandleType objecthandle,
						 uint8_t objectclass);

/*******************************************************************************
 * vTraceError
 *
 * Called by various parts in the recorder. Stops the recorder and stores a
 * pointer to an error message, which is printed by the monitor task.
 ******************************************************************************/
void vTraceError(const char* msg);

/*******************************************************************************
 * prvTraceInitTraceData
 *
 * Allocates and initializes the recorder data structure, based on the constants
 * in trcConfig.h. This allows for allocating the data on the heap, instead of
 * using a static declaration.
 ******************************************************************************/
void prvTraceInitTraceData(void);

/* Internal macros */

#define TRACE_PROPERTY_NAME_GET(objectclass, objecthandle) \
(const char*)(& RecorderDataPtr->ObjectPropertyTable.objbytes \
[uiIndexOfObject(objecthandle, objectclass)])

#define TRACE_PROPERTY_OBJECT_STATE(objectclass, handle) \
RecorderDataPtr->ObjectPropertyTable.objbytes[uiIndexOfObject(handle, objectclass) \
+ RecorderDataPtr->ObjectPropertyTable.NameLengthPerClass[objectclass]]

#define TRACE_PROPERTY_ACTOR_PRIORITY(objectclass, handle) \
RecorderDataPtr->ObjectPropertyTable.objbytes[uiIndexOfObject(handle, objectclass) \
+ RecorderDataPtr->ObjectPropertyTable.NameLengthPerClass[objectclass] + 1]

#define TRACE_SET_FLAG_ISEXCLUDED(flags, bitIndex) flags[(bitIndex) >> 3] |= (1 << ((bitIndex) & 7))
#define TRACE_CLEAR_FLAG_ISEXCLUDED(flags, bitIndex) flags[(bitIndex) >> 3] &= ~(1 << ((bitIndex) & 7))
#define TRACE_GET_FLAG_ISEXCLUDED(flags, bitIndex) (flags[(bitIndex) >> 3] & (1 << ((bitIndex) & 7)))

#define TRACE_SET_EVENT_CODE_FLAG_ISEXCLUDED(eventCode) TRACE_SET_FLAG_ISEXCLUDED(excludedEventCodes, eventCode)
#define TRACE_CLEAR_EVENT_CODE_FLAG_ISEXCLUDED(eventCode) TRACE_CLEAR_FLAG_ISEXCLUDED(excludedEventCodes, eventCode)
#define TRACE_GET_EVENT_CODE_FLAG_ISEXCLUDED(eventCode) TRACE_GET_FLAG_ISEXCLUDED(excludedEventCodes, eventCode)

/* DEBUG ASSERTS */
#if defined USE_TRACE_ASSERT && USE_TRACE_ASSERT != 0
#define TRACE_ASSERT(eval, msg, defRetVal) \
if (!(eval)) \
{ \
	vTraceError("TRACE_ASSERT: " msg); \
	return defRetVal; \
}
#else
#define TRACE_ASSERT(eval, msg, defRetVal)
#endif

#endif

#endif




