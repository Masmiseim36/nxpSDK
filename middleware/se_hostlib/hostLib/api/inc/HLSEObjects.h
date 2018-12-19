/**
* @file HLSEObjects.h
* @author NXP Semiconductors
* @version 1.0
* @par License
* Copyright 2017 NXP
*
* This software is owned or controlled by NXP and may only be used
* strictly in accordance with the applicable license terms.  By expressly
* accepting such terms or by downloading, installing, activating and/or
* otherwise using the software, you are agreeing that you have read, and
* that you agree to comply with and are bound by, such license terms.  If
* you do not agree to be bound by the applicable license terms, then you
* may not retain, install, activate or otherwise use the software.
*
* @par Description
* Host Lib wrapper API: Objects Operations
*
* @par HISTORY
*
*/
#ifndef _HLSE_OBJECTS_H_
#define _HLSE_OBJECTS_H_

#include "HLSETypes.h"


// HLSE stands for: Host Library Secure Element


/**
* Enumerates all the Objects that currently exist on the Secure Element and have
\p objectType type. A list of object handles is returned in \p objectsHandles.

In order to enumerate all the Objects, set HLSE_ANY_TYPE in \p objectType.

Each object has a unique HLSE_OBJECT_HANDLE value - this value depends on the library implementation.

If \p objectHandles is NULL, then all that the function does is return (in \p *objectHandlesLen) a number of HLSE_OBJECT_HANDLE which would suffice
to hold the returned list.  HLSE_SW_OK is returned by the function.

If \p objectHandles is not NULL, then \p *objectHandlesLen must contain the number of handles in the buffer \p objectHandles.  If that buffer
is large enough to hold number of handles to be returned, then the handles are copied to \p objectHandles, and HLSE_SW_OK is returned by the function.
If the buffer is not large enough, then HLSE_ERR_BUF_TOO_SMALL is returned.  In either case, \p *objectHandlesLen is set to hold the exact number of
handles to be returned.

* \param[in] objectType The type of the Objects to be enumerated
* \param[in, out] objectHandles IN: caller passes a buffer of at least *objectHandlesLen; OUT: contains the handles of the objects
* \param[in, out] objectHandlesLen IN: number of handles in objectHandles. OUT: set to hold the exact number of handles in objectHandles.
*
* \retval ::HLSE_SW_OK Successfull execution
* \retval ::HLSE_ERR_BUF_TOO_SMALL Buffer is too small to return the handles
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE   HLSE_EnumerateObjects(HLSE_OBJECT_TYPE objectType, HLSE_OBJECT_HANDLE* objectHandles, U16* objectHandlesLen);

/**
* Creates or Generates an Object on the Secure Element, and returns a handle to it.

If the object already exists, it depends on the Secure Element behavior whether this function succeeds (e.g. set
a new value) or fail with an error.

\p attributes is an array of attributes that the object should be created with. Some of the attributes may be mandatory, such
as HLSE_ATTR_OBJECT_TYPE and HLSE_ATTR_OBJECT_INDEX (the id of the object), and some are optional.

In case there is a conflict in the attribute list (e.g. 2 differnt object types) it is the responsibility of the
library to detect it and return an error.

* \param[in] attributes The attributes to be used in creating the Object
* \param[in] attributesNum The number of attributes in \p attributes
* \param[in, out] hObject IN: A pointer to a handle (must not be NULL); OUT: The handle of the created Object
*
* \retval ::HLSE_SW_OK Successfull execution
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE   HLSE_CreateObject(HLSE_ATTRIBUTE* attributes, U16 attributesNum, HLSE_OBJECT_HANDLE* hObject);

/**
* Erases an object from the Secure Element.

This means the object with the specified handle can no longer be used.

* \param[in] hObject The handle of the Object to be erased
*
* \retval ::HLSE_SW_OK Successfull execution
*/
HLSE_RET_CODE   HLSE_EraseObject(HLSE_OBJECT_HANDLE hObject);

/**
* Sets the requested Attribute of the Object.

The parameter \p attribute may convey additinal information (e.g. a key value), in addition to the attribute's type.

* \param[in] hObject The handle of the Object that its attribute should be set
* \param[in] attribute The attribute to be Set
*
* \retval ::HLSE_SW_OK Successfull execution
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE   HLSE_SetObjectAttribute(HLSE_OBJECT_HANDLE hObject, HLSE_ATTRIBUTE* attribute);

/**
* Obtains the value of the Object's requested Attribute.

The parameter \p attribute specifies the Type of the attribute to be returned, and the data is
returned in the attribute's value and valueLen members.

If \p attribute->value is NULL, then all that the function does is return (in \p *attribute->valueLen) a number of bytes which would suffice
to hold the value to be returned.  HLSE_SW_OK is returned by the function.

If \p attribute->value is not NULL, then \p *attribute->valueLen must contain the number of bytes in the buffer \p attribute->value.  If that buffer
is large enough to hold the value be returned, then the data is copied to \p attribute->value, and HLSE_SW_OK is returned by the function.
If the buffer is not large enough, then HLSE_ERR_BUF_TOO_SMALL is returned.  In either case, \p *attribute->valueLen is set to hold the exact number of
bytes to be returned.


* \param[in] hObject The handle of the Object that its attribute's value should be obtained
* \param[in, out] attribute The attribute to be obtained
*
* \retval ::HLSE_SW_OK Successfull execution
* \retval ::HLSE_ERR_BUF_TOO_SMALL \p attribute->value is too small to return the data
* \retval ::HLSE_ERR_API_ERROR Invalid function arguments
*/
HLSE_RET_CODE   HLSE_GetObjectAttribute(HLSE_OBJECT_HANDLE hObject, HLSE_ATTRIBUTE* attribute);

// TBD: allow more than one attribute to be set/get in one call


/**
 * Debug Utility
 *
 * Force Read of GPDataTable from gp storage even if already in global memory variable
 *
 * NOTE!! : To be used only for internal testing and Debugging
 * ======
 * currently used to test the GP Table manipulation
 *
 * \retval ::HLSE_SW_OK Successfull execution
 */
HLSE_RET_CODE Debug_ForceReadGPDataTable(void);

#endif // _HLSE_OBJECTS_H_
