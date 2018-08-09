/*
 * \file EventManager.c
 * This is a source file for the EventManager module.
 *
 * The Clear BSD License
 * Copyright 2013-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the
 * disclaimer below) provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
 * GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include <stdlib.h>

#include "EventManager.h"

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
static Observer *CreateObserver(void *callee, void (*Callback) (void *callee, void *object));
static void DestroyObserver(Observer *obs);

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
/*! *********************************************************************************
 * \brief Create a new event manager.
 * \details An EventMager is basically a linked list of Observers.
 *
 * \return pointer to the newly created EventManager
 ********************************************************************************* */
EventManager *CreateEventManager()
{
    EventManager *evt = (EventManager *) calloc(1, sizeof(EventManager));
    if (evt == NULL) {
        return NULL;
    }

    evt->obsList = CreateObserver(NULL, NULL);
    return evt;
}

/*! *********************************************************************************
 * \brief Destroy an event manager.
 * \details An EventMager is basically a linked list of Observers, so we destroy each
 * Observer and afterwards free the EventManager object.
 *
 * \param[in] evt  pointer to the EventManager to be destroyed
 *
 * \return None
 ********************************************************************************* */
void DestroyEventManager(EventManager *evt)
{
    Observer *crtObs;
    while (evt->obsList != NULL) {
        crtObs = evt->obsList;
        evt->obsList = crtObs->next;
        DestroyObserver(crtObs);
        crtObs = NULL;
    }

    free(evt);
}

/*! *********************************************************************************
 * \brief Adds a new Observer to the EventManager.
 *
 * \param[in] evt       pointer to the EventManager
 * \param[in] callee    the object to get notified
 * \param[in] Callback  pointer to a function that will be executed at notify
 *
 * \return None
 ********************************************************************************* */
void RegisterToEventManager(EventManager *evt, void *callee, void (*Callback) (void *callee, void *object))
{
    Observer *obs = CreateObserver(callee, Callback);
    obs->next = evt->obsList->next;
    evt->obsList->next = obs;
}

void DeregisterFromEvent(EventManager *evt, void *callee)
{
    Observer *crt = evt->obsList->next;
    Observer *prev = evt->obsList;
    while (crt != NULL && crt->callee != callee) {
        prev = crt;
        crt = crt->next;
    }

    if (crt != NULL) {
        prev->next = crt->next;
        DestroyObserver(crt);
        crt = NULL;
    }
}

void NotifyOnEvent(EventManager *evt, void *object)
{
    Observer *crt = evt->obsList->next;
    while (crt != NULL) {
        Observer *next = crt->next;
        if (crt->Callback != NULL) {
            crt->Callback(crt->callee, object);
        }
        crt = next;
    }
}

void NotifyOnSameEvent(EventManager *evt, void *object, void *(*func) (void *))
{
    Observer *crt = evt->obsList->next;
    while (crt != NULL) {
        if (crt->Callback != NULL) {
            crt->Callback(crt->callee, func(object));
        }
        crt = crt->next;
    }
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
static Observer *CreateObserver(void *callee, void (*Callback) (void *callee, void *object))
{
    Observer *obs = (Observer *) calloc(1, sizeof(Observer));
    obs->callee = callee;
    obs->Callback = Callback;
    obs->next = NULL;
    return obs;
}
static void DestroyObserver(Observer *obs)
{
    obs->next = NULL;
    obs->callee = NULL;
    obs->Callback = NULL;
    free(obs);
}
