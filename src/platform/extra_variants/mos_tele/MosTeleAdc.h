#pragma once

#include "configuration.h"

#ifdef MOS_TELE

#include <esp_adc/adc_oneshot.h>

bool mosTeleAdcInit();
bool mosTeleAdcReadMilliVolts(adc_channel_t channel, uint16_t &millivolts);

#endif
