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

#include <platform/CHIPDeviceLayer.h>
#include <platform/PlatformManager.h>

#include "gen/attribute-id.h"
#include "gen/cluster-id.h"
#include <app/Command.h>
#include <app/chip-zcl-zpro-codec.h>
#include <app/server/Mdns.h>
#include <app/util/af-types.h>
#include <app/util/af.h>
#include <app/util/attribute-storage.h>
#include <app/util/util.h>
#include <core/CHIPError.h>
#include <support/CHIPMem.h>
#include <support/RandUtils.h>

#include "AppMain.h"

#include <cassert>
#include <iostream>

using namespace chip;
using namespace chip::Inet;
using namespace chip::Transport;
using namespace chip::DeviceLayer;

uint8_t translateZclTemp(int16_t temperature)
{
    int32_t x = temperature;
    // rearrangement of
    // = (x * (9/5) / 100 + 32) * 2;
    // the added 250 is for proper rounding.
    // a rounding technique that only works
    // with positive numbers
    return (uint8_t)((x * 9 * 2 + 250) / (5 * 100) + 64);
}

void emberAfPostAttributeChangeCallback(EndpointId endpoint, ClusterId clusterId, AttributeId attributeId, uint8_t mask,
                                        uint16_t manufacturerCode, uint8_t type, uint16_t size, uint8_t * value)
{
    printf("emberAfPostAttributeChangeCallback!\r\n");
    printf("Endpoint: %i\r\n", endpoint);
    printf("clusterId: 0x%x\r\n", clusterId);
    printf("attributeId: 0x%x\r\n", attributeId);
    printf("mask: 0x%x\r\n", mask);
    printf("manufacturerCode: 0x%x\r\n", manufacturerCode);
    printf("type: 0x%x\r\n", type);
    printf("size: %i\r\n", size);
    printf("value: %i\r\n", *((uint16_t *) value));

    uint8_t convertedTemp;
    // TODO Setpoints are showing as type 0x21 not 0x29.  Need to research this
    switch (clusterId)
    {
    case ZCL_THERMOSTAT_CLUSTER_ID: {
        switch (attributeId)
        {
        case ZCL_OCCUPIED_COOLING_SETPOINT_ATTRIBUTE_ID: {
            convertedTemp = translateZclTemp(*((int16_t *) value));
            printf("====================================\r\n");
            printf("Cooling Setpoint now set to: %d", convertedTemp / 2);
            if (convertedTemp % 2 != 0)
                printf(".5\r\n");
            else
                printf("\r\n");
            printf("====================================\r\n");

            break;
        }
        case ZCL_OCCUPIED_HEATING_SETPOINT_ATTRIBUTE_ID: {
            convertedTemp = translateZclTemp(*((int16_t *) value));
            printf("====================================\r\n");
            printf("Heating Setpoint now set to: %d", convertedTemp / 2);
            if (convertedTemp % 2 != 0)
                printf(".5\r\n");
            else
                printf("\r\n");
            printf("====================================\r\n");
            break;
        }
        default: {
        }
        }
        break;

    default: {
    }
    }
    }
}

bool emberAfBasicClusterMfgSpecificPingCallback(chip::app::Command * commandObj)
{
    emberAfSendDefaultResponse(emberAfCurrentCommand(), EMBER_ZCL_STATUS_SUCCESS);
    return true;
}

int main(int argc, char * argv[])
{
    VerifyOrDie(ChipLinuxAppInit(argc, argv) == 0);
    ChipLinuxAppMainLoop();
    return 0;
}
