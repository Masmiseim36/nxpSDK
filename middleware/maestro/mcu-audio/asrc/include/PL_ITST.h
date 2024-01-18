/*
 * Copyright 2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PL_ITST_H
#define PL_ITST_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****************************************************************************************/
/*                                                                                      */
/*  General definitions of InsTroSpecTion Module                                                               */
/*                                                                                      */
/****************************************************************************************/

#ifdef PL_ITST_FUNC
extern PL_UINT32
    gITSTFrameNumber; // PL_UINT32 corresponding to 11930 Hours of Audio File : largely enough for testing purpose
extern PL_UINT16 gITSTSubFrameNumber;

void _PL_ITST_CONDCHECK_(const char *func, const char *file, PL_INT32 line, const char *cond_str);
void _PL_ITST_WARNING_(const char *func, const char *file, PL_INT32 line, const char *msg_str);
void _PL_ITST_MONITOR_RES_INIT_(const char *Name,
                                PL_UINT16 ElemOffset1,
                                PL_UINT16 ElemOffset2,
                                PL_UINT16 ElemSize,
                                PL_UINTPTR BaseAddr,
                                PL_UINT16 AllocatedSize,
                                PL_UINT16 Start,
                                PL_UINT16 Count_Type);
void _PL_ITST_MONITOR_RES_CHECK_(void);
void _PL_ITST_MONITOR_RES_CHECK_MAX_(void);
void _PL_ITST_MONITOR_RES_RESET_(void);

// Initialize ITST module
#define PL_ITST_INIT gITSTFrameNumber = 0;
// Increment Frame Number - Set the SubFrame number
#define PL_ITST_FRAME_NUMBER(FrameNumber) \
    gITSTFrameNumber++;                   \
    gITSTSubFrameNumber = FrameNumber;

// Check Conditions and Generate an Error
#define PL_ITST_CONDCHECK(cond)                                       \
    do                                                                \
    {                                                                 \
        if (cond)                                                     \
            (void)0;                                                  \
        else                                                          \
            _PL_ITST_CONDCHECK_(__func__, __FILE__, __LINE__, #cond); \
    } while (0);

// Generate a Warning
#define PL_ITST_WARNING(msg)                                  \
    do                                                        \
    {                                                         \
        _PL_ITST_WARNING_(__func__, __FILE__, __LINE__, msg); \
    } while (0);

// Methods used to monitor resource consumption
enum
{
    START_BEGIN = 0,
    START_END
};
enum
{
    NB_ELEM = 0,
    MAX_ELEM_ID
};
#define NO_OFFSET 0x7FFF
#ifdef PL_ITST_MONITOR
#define PL_ITST_MONITOR_RESOURCE_INIT(Name, ElemOffset1, ElemOffset2, ElemSize, BaseAddr, AllocatedSize, Start,   \
                                      Count_Type)                                                                 \
    do                                                                                                            \
    {                                                                                                             \
        _PL_ITST_MONITOR_RES_INIT_(Name, ElemOffset1, ElemOffset2, ElemSize, (PL_UINTPTR)BaseAddr, AllocatedSize, \
                                   Start, Count_Type);                                                            \
    } while (0);
#define PL_ITST_MONITOR_RESOURCE_CHECK     _PL_ITST_MONITOR_RES_CHECK_()
#define PL_ITST_MONITOR_RESOURCE_CHECK_MAX _PL_ITST_MONITOR_RES_CHECK_MAX_()
#define PL_ITST_MONITOR_RESOURCE_RESET     _PL_ITST_MONITOR_RES_RESET_()

#else // PL_ITST_MONITOR
#define PL_ITST_MONITOR_RESOURCE_INIT(Name, ElemOffset1, ElemOffset2, ElemSize, BaseAddr, AllocatedSize, Start, \
                                      Count_Type)
#define PL_ITST_MONITOR_RESOURCE_CHECK
#define PL_ITST_MONITOR_RESOURCE_CHECK_MAX
#define PL_ITST_MONITOR_RESOURCE_RESET
#endif // PL_ITST_MONITOR

#else  // define PL_ITST_FUNC
// Nothing to do
#define PL_ITST_INIT
#define PL_ITST_FRAME_NUMBER(FrameNumber)
#define PL_ITST_CONDCHECK(cond)
#define PL_ITST_WARNING(msg)
#define PL_ITST_MONITOR_RESOURCE_INIT(Name, ElemOffset1, ElemOffset2, ElemSize, BaseAddr, AllocatedSize, Start, \
                                      Count_Type)
#define PL_ITST_MONITOR_RESOURCE_CHECK
#define PL_ITST_MONITOR_RESOURCE_CHECK_MAX
#define PL_ITST_MONITOR_RESOURCE_RESET
#endif

/****************************************************************************************/
/*                                                                                      */
/*  Standard Enumerated types                                                           */
/*                                                                                      */
/****************************************************************************************/

/****************************************************************************************/
/*                                                                                      */
/*  Standard function interfaces                                                        */
/*                                                                                      */
/****************************************************************************************/

/****************************************************************************************/
/*                                                                                      */
/*  End of file                                                                         */
/*                                                                                      */
/****************************************************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PL_ITST_H */
