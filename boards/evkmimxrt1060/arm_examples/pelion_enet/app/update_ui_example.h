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

#ifndef UPDATE_UI_EXAMPLE_H
#define UPDATE_UI_EXAMPLE_H

#include "mbed-cloud-client/MbedCloudClient.h"

#ifdef MBED_CLOUD_CLIENT_SUPPORT_UPDATE
/**
 * @brief Function for authorizing firmware downloads and reboots.
 * @param request The request under consideration.
 * @param priority The priority under consideration.
 */
void update_authorize_priority_handler(int32_t request, uint64_t priority);

/**
 * @brief Callback function for reporting the firmware download progress.
 * @param progress Received bytes.
 * @param total Total amount of bytes to be received.
 */
void update_progress(uint32_t progress, uint32_t total);

/**
 * @brief Set the cloud client instance for the update UI to use
 * @param[in] client pointer to the cloud client instance
 */
void update_ui_set_cloud_client(MbedCloudClient* client);


#endif // MBED_CLOUD_CLIENT_SUPPORT_UPDATE

#endif // UPDATE_UI_EXAMPLE_H
