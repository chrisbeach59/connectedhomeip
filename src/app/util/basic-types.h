/**
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 * @file
 * @brief The include file for all the types for the data model that are not
 *        dependent on an individual application configuration.
 */

#pragma once

#include <stdint.h>

// Pull in NodeId
#include <transport/raw/MessageHeader.h>

namespace chip {
typedef uint8_t EndpointId;
typedef uint16_t ClusterId;
typedef uint16_t AttributeId;
typedef uint16_t GroupId;
typedef uint8_t CommandId;
typedef uint16_t EventId;
typedef uint64_t NodeId;
typedef uint64_t DataVersion;
} // namespace chip
