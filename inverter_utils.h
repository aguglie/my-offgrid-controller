
#define MEASURED_ABSORPTIONS 51
#define MIN_CHARGING_CURRENT 0
#define MAX_CHARGING_CURRENT 50

uint16_t measured_absorptions_matrix[MEASURED_ABSORPTIONS] = {
    0,   // 0A
    170, // 1A --> 1A is unstable
    325, // 2A
    373, // ...
    415,
    458,
    537,
    594,
    651,
    708,
    765,
    822,
    879,
    936,
    993,
    1050,
    1107,
    1164,
    1221,
    1278,
    1335,
    1392,
    1449,
    1506,
    1563,
    1620,
    1677,
    1734,
    1791,
    1848,
    1905,
    1962,
    2019,
    2076,
    2133,
    2190,
    2247,
    2304,
    2361,
    2418,
    2475,
    2532,
    2589,
    2646,
    2703,
    2760,
    2817,
    2874,
    2931,
    2988,
    3046 // 50A
};

uint16_t estimate_watt_consumption_from_charging_current(uint16_t charging_current)
{
    if (charging_current > 50)
    {
        return measured_absorptions_matrix[50];
    }
    return measured_absorptions_matrix[charging_current];
}

uint16_t calculate_next_max_charging_current(uint16_t current_max_charging_current, int16_t wanted_delta_watt)
{
    if (wanted_delta_watt == 0)
    {
        return current_max_charging_current;
    }

    uint16_t current_consumption = estimate_watt_consumption_from_charging_current(current_max_charging_current);
    ESP_LOGI("automation", "Current estimated charger consumption: %d", current_consumption);

    if (wanted_delta_watt > 0)
    {
        for (uint16_t i = current_max_charging_current; i < MAX_CHARGING_CURRENT; i++)
        {
            uint16_t next_consumption = estimate_watt_consumption_from_charging_current(i);
            if (next_consumption > current_consumption + wanted_delta_watt)
            {
                return fmax(0, i - 1);
            }
        }

        return MAX_CHARGING_CURRENT;
    }
    else
    {
        for (uint16_t i = current_max_charging_current; i > MIN_CHARGING_CURRENT; i--)
        {
            uint16_t next_consumption = estimate_watt_consumption_from_charging_current(i);
            if (next_consumption < current_consumption + wanted_delta_watt)
            {
                return fmin(50, i + 1);
            }
        }

        return MIN_CHARGING_CURRENT;
    }
}
