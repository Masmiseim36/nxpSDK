/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _VECTOR_H_
#define _VECTOR_H_

/*!
 * @file    vector.h
 * @brief   vector data type implementation
 */

#include <stdbool.h>

/*
 * STRUCTURES AND OTHER TYPEDEFS
 */
typedef struct _Vector {
    void           *v_pdata;    /**< start address of dynamic array */
    uint32_t        data_sz;    /**< size of each data element */
    uint32_t        size;       /**< number of elements stored */
    uint32_t        cap;        /**< current capacity of this vector */
} Vector;

/*
 * vector iterator type
 */
typedef struct _VectorIter {
    void *node;         /**< pointer to address of a data entry in the vector */
    uint32_t data_sz;   /**< size of each data element */
} VectorIter;

/*!  @{
 *  @name VECTOR FUNCTION PROTOTYPES
 */

/*!
 * @ingroup libcommon
 * Create a vector with minimal space.
 * The vector will allocate initial memory using this formula:
 *          VECTOR_SIZE_MIN * data_sz
 *
 * @param   data_sz  Size of each data member in vector
 * @param   cap_init Initial capacity size of the created vector,
 *          if this parameter is set to 0, then a default minimal capacity is
 *          used.
 *
 * @returns vector pointer or NULL if create failed
 */
Vector *vector_create(const uint32_t data_sz, uint32_t cap_init);

/*!
 * Destroys the vector and frees the internal memory.
 *
 * @param   vt Pointer to a vector
 *
 * @returns  none
 */
void vector_destroy(Vector *vt);

/*!
 * Returns the capacity of a vector.
 * This value is how many data entries can be stored in the memory currently
 * allocated for this vector.
 *
 * @param   vt Pointer to a vector
 *
 * @returns  capacity of a vector, 0 if the vector is NULL
 */
uint32_t vector_capacity(const Vector *vt);

/*!
 * Returns the size of a vector.
 * The size is the number of data entries saved in a vector.
 *
 * @param   vt  Pointer to a vector
 * @returns  size of a vector, 0 if the vector is NULL
 */
uint32_t vector_size(const Vector *vt);

/*!
 * Check if a vector is empty.
 *
 * @param   vt Pointer to a vector
 *
 * @returns  bool @todo what returns?
 */
bool vector_is_empty(const Vector *vt);

/*!
 * Clear all the data in a vector.
 * Sets the vector capacity to default minimal, and sets size to 0.
 * Memory will be deleted only if SHRINK_SIZE_AFTER_DATA_REMOVE is defined.
 *
 * @param   vt Pointer to a vector
 *
 * @returns  osa error code
 * @retval  ERRCODE_NO_ERROR           No error
 * @retval  ERRCODE_INVALID_ARGUMENT   Parameter vt is NULL
 * @retval  ERRCODE_OUT_OF_MEMORY      Reallocation of memory failed
 */
int vector_clear(Vector *vt);

/*!
 * Increases the memory allocation for a vector to be able to contain 'size'
 * data entries.  The new capacity will be the larger of the former capacity or
 * the new size request.
 *
 * @param   vt      Pointer to a vector
 * @param   size    Amount of data space required
 *
 * @returns  osa error code
 * @retval  ERRCODE_NO_ERROR           OK
 * @retval  ERRCODE_INVALID_ARGUMENT   Vector is NULL
 * @retval  ERRCODE_OUT_OF_MEMORY      Not enough memory
 */
int vector_reserve(Vector *vt, const uint32_t size);

/*!
 * Pushes data to the end of a vector.
 * If there is not enough space, the vector will expand.
 *
 * @param   vt      Pointer to a vector
 * @param   data    Pointer to the data
 *
 * @returns  osa error code
 * @retval  ERRCODE_NO_ERROR           OK
 * @retval  ERRCODE_INVALID_ARGUMENT   Vector is NULL or data is NULL
 * @retval  ERRCODE_OUT_OF_MEMORY      Not enough memory
 */
int vector_pushback(Vector *vt, const void *data);

/*!
 * Remove the data entry at the end of a vector and return its contents.
 * Memory will be deleted only if SHRINK_SIZE_AFTER_DATA_REMOVE is defined.
 *
 * @param   vt      Pointer to a vector
 * @param   data    pointer to memory block for the returned data (allocated
 *                  by the caller)
 *
 * @returns  osa error code
 * @retval  ERRCODE_NO_ERROR           OK
 * @retval  ERRCODE_INVALID_ARGUMENT   Vector is NULL or data is NULL
 * @retval  ERRCODE_GENERAL_ERROR      Vector is empty
 * @retval  ERRCODE_OUT_OF_MEMORY      Not enough memory
 */
int vector_popback(Vector *vt, void *data);

/*!
 * Get data from the vector using the index of a data member.
 *
 * @param   vt      pointer to a vector
 * @param   index   data index in a vector
 * @param   data    pointer to memory block for the returned data (allocated
 *                  by the caller)
 *
 * @returns  osa error code
 * @retval  ERRCODE_NO_ERROR           OK
 * @retval  ERRCODE_INVALID_ARGUMENT   Vector is NULL or data is NULL or index
 * out of range
 * @retval  ERRCODE_GENERAL_ERROR      Vector is empty
 */
int vector_at(const Vector *vt, const uint32_t index, void *data);

/*!
 * Assign data to a vector using an index, replacing previous contents.
 *
 * @param   vt      pointer to a vector
 * @param   index   data index in a vector.  Must be less than the size of the
 *                  vector.
 * @param   data    pointer to the data
 *
 * @returns  osa error code
 * @retval  ERRCODE_NO_ERROR           OK
 * @retval  ERRCODE_INVALID_ARGUMENT   Vector is NULL or data is NULL or index
 * out of range
 * @retval  ERRCODE_GENERAL_ERROR      Vector is empty
 */
int vector_assign(const Vector *vt, const uint32_t index, const void *data);

/*!
 * Insert data to a vector using an index.
 * The size of the vector will increase and the data after the index will
 * move up one.
 *
 * @param   vt      Pointer to a vector
 * @param   index   Data index in a vector.  Must be less than the vector size
 * @param   data    Pointer to the data
 *
 * @returns  osa error code
 * @retval  ERRCODE_NO_ERROR              OK
 * @retval  ERRCODE_INVALID_ARGUMENT      Vector is NULL or data is NULL or
 *                                        index out of range
 */
int vector_insert_at(Vector *vt, const uint32_t index, const void *data);

/*!
 * Remove data from a vector using an index.
 * Data after the index will be moved down one.
 * Memory will be deleted only if SHRINK_SIZE_AFTER_DATA_REMOVE is defined.
 *
 * @param   vt      Pointer to a vector
 * @param   index   Data index in a vector
 *
 * @returns  osa error code
 * @retval  ERRCODE_NO_ERROR           OK
 * @retval  ERRCODE_INVALID_ARGUMENT   Vector is NULL or index out of range
 * @retval  ERRCODE_OUT_OF_MEMORY      Not enough memory
 */
int vector_remove_at(Vector *vt, const uint32_t index);

/*!
 * Swap the contents of two data entries in a vector.
 *
 * @param   vt  Pointer to a vector
 * @param   a   Data index in a vector
 * @param   b   Data index in a vector
 *
 * @returns  osa error code
 * @retval  ERRCODE_NO_ERROR           OK
 * @retval  ERRCODE_INVALID_ARGUMENT   Vector is NULL or index out of range
 * @retval  ERRCODE_OUT_OF_MEMORY      Not enough memory
 */
int vector_swap(const Vector *vt, const uint32_t a, const uint32_t b);

/*!
 * Get the iterator for the first data entry of a vector.
 * This allows other functions to step through the vector entries or to
 * access the first entry in the vector.
 *
 * @param   vt Pointer to a vector
 * @returns  iterator, NULL if the vector is NULL
 */
VectorIter vector_iter_begin(const Vector *vt);

/*!
 * Get the iterator for the end of a vector.
 * This is the point after the last entry in the vector, where a new data
 * entry would be added.
 *
 * @param   vt pointer to a vector
 *
 * @returns  iterator, NULL if the vector is NULL
 */
VectorIter vector_iter_end(const Vector *vt);

/*!
 * Get the next iterator in a vector.
 * This gives access to the next data entry in the vector.
 *
 * @param   iter    Current iterator
 *
 * @returns  iterator, NULL if the vector is NULL
 */
VectorIter vector_iter_next(VectorIter iter);

/*!
 * Get the previous iterator in a vector.
 * This gives access to the previous data entry in the vector.
 *
 * @param   vt      Pointer to a vector
 * @param   iter    Current iterator
 *
 * @returns  iterator, NULL if the vector is NULL
 */
VectorIter vector_iter_prev(const Vector *vt, VectorIter iter);

/*!
 * Compare two iterators for equality.
 * They are equal if they point to the same data entry in the vector.
 *
 * @param   a   Iterator a
 * @param   b   Iterator b
 *
 * @returns  TRUE if two iterators are equal, else FALSE
 */
bool vector_iter_equal(const VectorIter a, const VectorIter b);

/*!
 * Get the index of the data entry an iterator is pointing to.
 *
 * @param   vt      pointer to a vector
 * @param   iter    current iterator
 * @param   at      save the index value
 *
 * @returns  osa error code
 * @retval  ERRCODE_NO_ERROR          OK
 * @retval  ERRCODE_INVALID_ARGUMENT  Vector or at is NULL or iterator out of
 *                                    range
 */
int vector_iter_at(const Vector *vt, VectorIter iter, uint32_t *at);

/*!
 * Get the pointer to the data entry in a vector indicated by the iterator.
 *
 * @param   iter    Current iterator
 *
 * @returns  Internal node pointer of data member in a vector.
 *          NULL indicates an error
 */
void *vector_iter_node(const VectorIter iter);

/*!
 * Get the data from the entry indicated by the iterator.
 *
 * @param   vt      Pointer to a vector
 * @param   iter    Current iterator
 * @param   data    Save the value
 *
 * @returns  osa error code
 * @retval  ERRCODE_NO_ERROR          OK
 * @retval  ERRCODE_INVALID_ARGUMENT  Vector or data is NULL or iterator out of
 *                                    range
 */
int vector_iter_value(const Vector *vt, VectorIter iter, void *data);

/*! @} */

#endif
