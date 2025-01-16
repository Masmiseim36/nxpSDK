/*
 * Copyright 2022-2024 NXP.
 *
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef _MPP_VERSION_H
#define _MPP_VERSION_H

/* MPP version */
#define MPP_VERSION_MAJOR 3
#define MPP_VERSION_MINOR 2
#ifdef MPP_COMMIT
#define MPP_VERSION_COMMIT MPP_COMMIT
#else
#define MPP_VERSION_COMMIT 0
#endif

#endif /* _MPP_VERSION_H */
