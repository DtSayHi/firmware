#include "MosTeleAdc.h"

#ifdef MOS_TELE

#include <esp_adc/adc_cali.h>
#include <esp_adc/adc_cali_scheme.h>

namespace
{
adc_oneshot_unit_handle_t adcHandle = nullptr;
adc_cali_handle_t calibrationHandle = nullptr;

bool configureChannel(adc_channel_t channel)
{
    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    return adc_oneshot_config_channel(adcHandle, channel, &config) == ESP_OK;
}
} // namespace

bool mosTeleAdcInit()
{
    if (adcHandle)
        return true;

    adc_oneshot_unit_init_cfg_t unitConfig = {
        .unit_id = ADC_UNIT_1,
    };
    if (adc_oneshot_new_unit(&unitConfig, &adcHandle) != ESP_OK)
        return false;

    if (!configureChannel(ADC_CHANNEL) || !configureChannel(MOS_TELE_JOYSTICK_X_CHANNEL) ||
        !configureChannel(MOS_TELE_JOYSTICK_Y_CHANNEL))
        return false;

    adc_cali_curve_fitting_config_t calibrationConfig = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    if (adc_cali_create_scheme_curve_fitting(&calibrationConfig, &calibrationHandle) != ESP_OK)
        calibrationHandle = nullptr;

    return true;
}

bool mosTeleAdcReadMilliVolts(adc_channel_t channel, uint16_t &millivolts)
{
    if (!adcHandle)
        return false;

    int raw = 0;
    if (adc_oneshot_read(adcHandle, channel, &raw) != ESP_OK)
        return false;

    int converted = 0;
    if (calibrationHandle) {
        if (adc_cali_raw_to_voltage(calibrationHandle, raw, &converted) != ESP_OK)
            return false;
    } else {
        converted = raw * 3100 / 4095;
    }

    millivolts = static_cast<uint16_t>(converted);
    return true;
}

#endif
