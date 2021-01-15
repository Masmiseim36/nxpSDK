/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef CONTAINERS_H
#define CONTAINERS_H

/*!
 * @file    container.h
 * @brief   Contains macros and functions that are common and generic across
 *          OSA container data types.
 *
 * Data types have been defined for iteration to allow applications to be as
 * portable as possible, and include: list, doubly linked list, vector, and
 * associated container iterators.
 * Additional iterators can be defined by the application developer using the
 * criteria below.
 * Each container may define a set of iterator functions and an
 * iterator data type.  The macros and functions detailed below can operate
 * on all container types as long as the iterators are defined.  The 'container'
 * prefix for the container macros specifies the container type.
 *
 * The iterator functions should look like these:
 *
 * Iterator <i>container</i>_iter_begin(container *data) -
 * Returns an iterator pointing to the first element in the container.
 *
 * Iterator <i>container</i>_iter_end(container *data) -
 * Returns an iterator pointing to the end of the container.
 *
 * Iterator <i>container</i>_iter_next(Iterator iter) -
 * Returns an iterator pointing to the next element in the container.
 *
 * Iterator <i>container</i>_iter_prev(Iterator iter) -
 * Returns an iterator pointing to the previous element in the container.
 *
 * void *<i>container</i>_iter_node(Iterator iter) -
 * Returns the data contained in the element pointed to by the iterator.
 *
 * bool <i>container</i>_iter_equal(Iterator iter_a, Iterator iter_b) -
 * Compares two iterators for equality.
 */

/*!
 * @ingroup libcommon
 * @{
 */

/*!
 * @brief Returns the iterator pointing to the end of the container.
 *
 * @param name prefix of the container (eg: list)
 * @param c pointer to container data (eg: List*)
 * @param ci container iterator (eg: ListIter)
 */
#define container_iter_end(name, c, ci) \
    (name##_iter_equal(ci, name##_iter_end(c)))

/*!
 * @brief Iterate over each element in a container.
 *
 * @param name prefix of the container (eg: list)
 * @param c pointer to container data (eg: List*)
 * @param ci container iterator (eg: ListIter)
 * @param n Output storing the data inside the container.
 *
 * Note the ', 1' hack in the test condition: this is done so that it always
 * returns true.  This allows the _iter_end() test to be the only one used to
 * break out, and it will disallow out-of-bounds memory access by doing it
 * first with the &&.
 */
#define container_foreach(name, c, ci, n) \
    for (ci = name##_iter_begin(c); \
         (!container_iter_end(name, c, ci) && \
         ((n = name##_iter_node(ci)), 1)); \
         ci = name##_iter_next(ci))

#endif
/*! @} */
