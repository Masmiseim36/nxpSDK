// ----------------------------------------------------------------------------
// Copyright 2016-2017 ARM Ltd.
//
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------------------------------------------------------


#ifndef RESOURCE_H
#define RESOURCE_H


/**
 * \brief Helper function for creating different kind of resources.
 *        The path of the resource will be "object_id/instance_id/resource_id"
 *        For example if object_id = 1, instance_id = 2, resource_id = 3
 *        the path would be 1/2/3
 *
 * \param list Pointer to the object list,
 *             contains objects to be registered to the server.
 * \param object_id Name of the object in integer format.
 * \param instance_id Name of the instance in integer format.
 * \param resource_id Name of the resource in integer format.
 * \param resource_type Resource type name.
 * \param data_type Data type of the resource value.
 * \param allowed Methods allowed for accessing this resource.
 * \param value Resource value as a null terminated string.
 *              May be set as NULL.
 * \param observable Resource set observable if true.
 * \param cb Function pointer to either:
 *           value_updated_callback2 if allowed & GET_PUT_ALLOWED
 *           OR
 *           execute_callback_2 in if allowed & POST_ALLOWED.
 *           In other cases this parameter is ignored.
 *
 *        NOTE: This function is not designed to support setting both
 *              GET_PUT_ALLOWED and POST_ALLOWED for parameter allowed
 *              at the same time.
 * \param message_status_cb Function pointer to message_status_cb
 */
M2MResource* add_resource(M2MObjectList *list,
                          uint16_t object_id,
                          uint16_t instance_id,
                          uint16_t resource_id,
                          const char *resource_type,
                          M2MResourceInstance::ResourceType data_type,
                          M2MBase::Operation allowed,
                          const char *value,
                          bool observable,
                          void *cb,
                          void *message_status_cb);

#endif //RESOURCE_H
