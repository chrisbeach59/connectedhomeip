/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *    All rights reserved.
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
 *    @file
 *      This file implements the CHIP Device Interface that is used by
 *      applications to interact with the CHIP stack
 *
 */

#include <stdlib.h>

#include "CHIPDeviceManager.h"
#include <app/ConcreteAttributePath.h>
#include <app/util/basic-types.h>
#include <support/CHIPMem.h>
#include <support/CodeUtils.h>
#include <support/ErrorStr.h>

#include <app-common/zap-generated/callback.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app/Command.h>
#include <app/clusters/identify-server/identify-server.h>
#include <app/util/af.h>


using namespace chip;
using namespace chip::app;


namespace chip {

namespace DeviceManager {

using namespace ::chip::DeviceLayer;

void CHIPDeviceManager::CommonDeviceEventHandler(const ChipDeviceEvent * event, intptr_t arg)
{
    CHIPDeviceManagerCallbacks * cb = reinterpret_cast<CHIPDeviceManagerCallbacks *>(arg);
    if (cb != nullptr)
    {
        cb->DeviceEventCallback(event, reinterpret_cast<intptr_t>(cb));
    }
}

CHIP_ERROR CHIPDeviceManager::Init(CHIPDeviceManagerCallbacks * cb)
{
    CHIP_ERROR err;
    mCB = cb;

    // Initialize the CHIP stack.
    err = PlatformMgr().InitChipStack();
    SuccessOrExit(err);

    if (CONFIG_NETWORK_LAYER_BLE)
    {
        ConnectivityMgr().SetBLEAdvertisingEnabled(true);
    }

    err = Platform::MemoryInit();
    SuccessOrExit(err);

    // Register a function to receive events from the CHIP device layer.  Note that calls to
    // this function will happen on the CHIP event loop thread, not the app_main thread.
    PlatformMgr().AddEventHandler(CHIPDeviceManager::CommonDeviceEventHandler, reinterpret_cast<intptr_t>(cb));

    // Start a task to run the CHIP Device event loop.
    err = PlatformMgr().StartEventLoopTask();
    SuccessOrExit(err);

exit:
    return err;
}
} // namespace DeviceManager
} // namespace chip

void emberAfThermostatClusterInitCallback(EndpointId endpoint)
{
    // To prevent warning
    (void) endpoint;
}

// MatterPreAttributeChangeCallback() is called for every cluster.
// As of 8/17/21 cluster specific PreAttributeChangeCalbacks are not yet implemented.

Protocols::InteractionModel::Status MatterPreAttributeChangeCallback(const ConcreteAttributePath & attributePath, uint8_t mask,
                                                                     uint8_t type, uint16_t size, uint8_t * value)
{
    Protocols::InteractionModel::Status status = Protocols::InteractionModel::Status::Success;
    if (attributePath.mClusterId == chip::app::Clusters::Thermostat::Id)
    {
        status = MatterThermostatClusterServerPreAttributeChangedCallback(attributePath, type, size, value);
    }
    return status;
}


void MatterPostAttributeChangeCallback(const chip::app::ConcreteAttributePath & path, uint8_t mask, uint8_t type, uint16_t size,
                                       uint8_t * value)
{
    chip::DeviceManager::CHIPDeviceManagerCallbacks * cb =
        chip::DeviceManager::CHIPDeviceManager::GetInstance().GetCHIPDeviceManagerCallbacks();
    if (cb != nullptr)
    {
        cb->PostAttributeChangeCallback(path.mEndpointId, path.mClusterId, path.mAttributeId, mask, type, size, value);
    }
}
