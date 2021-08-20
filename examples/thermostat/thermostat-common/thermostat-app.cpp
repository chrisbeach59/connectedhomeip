#include <app/common/gen/attribute-id.h>
#include <app/common/gen/cluster-id.h>
#include <app/util/af-types.h>
#include <app/util/af.h>
#include <app/util/attribute-storage.h>
#include <app/util/util.h>

using namespace chip;
using namespace chip::Inet;
using namespace chip::Transport;
using namespace chip::DeviceLayer;

// Defines for specific thermosts
#define THERMOSTAT_NUM_OF_WEEKLY_TRANSITIONS 2

#define THERMOSTAT_NUM_OF_DAILY_TRANSITIONS 4

#define THERMOSTAT_SUPPORTS_HEAT 1

#define THERMOSTAT_SUPPORTS_COOL 1

#define THERMOSTAT_MODE_HEAT 0x01
#define THERMOSTAT_MODE_COOL 0x02

#define MONDAY 0x01
#define TUESDAY 0x02
#define WEDNESDAY 0x04
#define THURSDAY 0x08
#define FRIDAY 0x10
#define SATURDAY 0x20
#define SUNDAY 0x40

#define WEEKDAY (MONDAY | TUESDAY | WEDNESDAY | THURSDAY | FRIDAY)
#define WEEKEND (SATURDAY | SUNDAY)

#if THERMOSTAT_SUPPORTS_HEAT && THERMOSTAT_SUPPORTS_COOL
#define THERMOSTAT_SUPPORTED_MODES (THERMOSTAT_MODE_HEAT | THERMOSTAT_MODE_COOL)
#elif THERMOSTAT_SUPORTS_HEAT
#define THERMOSTAT_SUPPORTED_MODES THERMOSTAT_MODE_HEAT
#elif THERMOSTAT_SUPORTS_COOL
#define THERMOSTAT_SUPPORTED_MODES THERMOSTAT_MODE_COOL
#else
#error "Thermostat must support at least one mode"
#endif

typedef struct
{
    uint16_t TimeInSecondsFromMidnight;
#if (THERMOSTAT_SUPPORTS_HEAT == 1)
    int16_t ScheduledHeatingSetpoint;
#endif
#if (THERMOSTAT_SUPPORTS_COOL == 1)
    int16_t ScheduledCoolingSetpoint;
#endif
} thermostatScheduledTransition_t;

thermostatScheduledTransition_t thermostatDailyTransitions;

// thermostatDailyTransitions.TimeInSecondsFromMidnight = 60*60*6;
// thermostatDailyTransitions.ScheduledCoolingSetpoint = 2200;
// thermostatDailyTransitions.ScheduledHeatingSetpoint = 2000;

typedef struct
{
    uint8_t Days;
    thermostatScheduledTransition_t thermostatDailyTransitions[THERMOSTAT_NUM_OF_DAILY_TRANSITIONS];
} thermostatDailySchedule_t;

thermostatDailySchedule_t ThermostatWeeklySchedule[THERMOSTAT_NUM_OF_WEEKLY_TRANSITIONS];

void thermostatClusterInitializeSchedule(void)
{
#if (THERMOSTAT_NUM_OF_WEEKLY_TRANSITIONS == 2)
    ThermostatWeeklySchedule[0].Days = WEEKDAY;
    ThermostatWeeklySchedule[1].Days = WEEKEND;
#elif (THERMOSTAT_NUM_OF_WEEKLY_TRANSITIONS == 7)
    ThermostatWeeklySchedule[0].Days = MONDAY;
    ThermostatWeeklySchedule[1].Days = TUESDAY;
    ThermostatWeeklySchedule[2].Days = WEDNESDAY;
    ThermostatWeeklySchedule[3].Days = THURSDAY;
    ThermostatWeeklySchedule[4].Days = FRIDAY;
    ThermostatWeeklySchedule[5].Days = SATURDAY;
    ThermostatWeeklySchedule[6].Days = SUNDAY;
#else
#error "Only 2 or 7 weekly transitions are supported"
#endif
}

bool thermostatUpdateSchedule(uint8_t numberOfTransitionsForSequence, uint8_t daysOfWeekForSequence, uint8_t modeForSequence,
                              uint8_t * payload)
{
    // PRE_CONDITIONS Called from emberAfThermostatClusterSetWeeklyScheduleCallback()
    // which has verified the size and pre-conditions
    uint16_t TransitionTime        = 0;
    int16_t TransitionHeatSetpoint = -1;
    int16_t TransitionCoolSetpoint = -1;

    // Find the day in the weekly schedule
    for (int day = 0; day < THERMOSTAT_NUM_OF_WEEKLY_TRANSITIONS; day++)
    {
        if ((ThermostatWeeklySchedule[day].Days & daysOfWeekForSequence) != 0)
        {
            // This transition applies to this entry
            // Check the mode for the transition
            if ((modeForSequence & THERMOSTAT_SUPPORTED_MODES) != 0)
            {
                uint8_t * p = payload;
                int transition;
                for (transition = 0; transition < numberOfTransitionsForSequence; transition++)
                {
                    TransitionTime = emberAfGetInt16u(p, 0, 2);
                    ThermostatWeeklySchedule[day].thermostatDailyTransitions[transition].TimeInSecondsFromMidnight = TransitionTime;
                    p += 2;
                    if ((modeForSequence & THERMOSTAT_MODE_HEAT) == THERMOSTAT_MODE_HEAT)
                    {
                        TransitionHeatSetpoint = emberAfGetInt16s(p, 0, 2);
                        p += 2;
#if (THERMOSTAT_SUPPORTS_HEAT != 0)
                        ThermostatWeeklySchedule[day].thermostatDailyTransitions[transition].ScheduledHeatingSetpoint =
                            TransitionHeatSetpoint;
#endif
                    }
                    if ((modeForSequence | THERMOSTAT_MODE_COOL) == THERMOSTAT_MODE_COOL)
                    {
                        TransitionCoolSetpoint = emberAfGetInt16s(p, 0, 2);
                        p += 2;
#if (THERMOSTAT_SUPPORTS_COOL != 0)
                        ThermostatWeeklySchedule[day].thermostatDailyTransitions[transition].ScheduledCoolingSetpoint =
                            TransitionCoolSetpoint;
#endif
                    }
                }
            }
        }
    }
    return true;
}

// This file provides API's to update and read data in the Matter Thermostat Cluster

// This function is called when the thermostat senses that local temperature has changed to update the Matter Cluster
bool thermostatClusterSetLocalTemperature(int16_t CurrentTemp)
{
    return true;
}

// Called when the local UI changes the cooling setpoint
bool thermostatClusterSetOccupiedCoolingSetpoint(int16_t Setpoint)
{

    return true;
}

// Called when the local UI changes the heating setpoint
bool thermostatClusterSetOccupiedHeatingSetpoint(int16_t Setpoint)
{
    return true;
}

// Scheduling API's
void thermostatClusterClearAllScheduleTransitions()
{
    // TODO
}

EmberAfStatus thermostatClusterSetStartOfWeek(chip::EndpointId endpoint, uint8_t StartOfWeek)
{
    return emberAfWriteServerAttribute(endpoint, ZCL_THERMOSTAT_CLUSTER_ID, ZCL_START_OF_WEEK_ATTRIBUTE_ID, &StartOfWeek,
                                       ZCL_BITMAP8_ATTRIBUTE_TYPE);
}

EmberAfStatus thermostatClusterSetNumberOfWeeklyTransitions(chip::EndpointId endpoint, uint8_t WeeklyTransitions)
{
    return emberAfWriteServerAttribute(endpoint, ZCL_THERMOSTAT_CLUSTER_ID, ZCL_NUMBER_OF_WEEKLY_TRANSITIONS_ATTRIBUTE_ID,
                                       &WeeklyTransitions, ZCL_INT8U_ATTRIBUTE_TYPE);
}

EmberAfStatus thermostatClusterSetNumberOfDailyTransitions(chip::EndpointId endpoint, uint8_t DailyTransitions)
{
    return emberAfWriteServerAttribute(endpoint, ZCL_THERMOSTAT_CLUSTER_ID, ZCL_NUMBER_OF_DAILY_TRANSITIONS_ATTRIBUTE_ID,
                                       &DailyTransitions, ZCL_INT8U_ATTRIBUTE_TYPE);
}
