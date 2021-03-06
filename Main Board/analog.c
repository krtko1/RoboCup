#include "analog.h"
#include "config.h"
#include "includes/chprintf.h"


void mx_set(uint8_t num) {
    if (num & 0b1) {
        palClearPad(GPIOA,5);
    } else {
        palSetPad(GPIOA,5);
    }

    if (num & 0b10) {
        palClearPad(GPIOB,6);
    } else {
        palSetPad(GPIOB,6);
    }

    if (num & 0b100) {
        palClearPad(GPIOC,7);
    } else {
        palSetPad(GPIOC,7);
    }
}

void mx_specify(bool a, bool b, bool c) {
    if (a){
        palSetPad(GPIOA,5);
    } else {
        palClearPad(GPIOA,5);
    }
    if (b) {
        palSetPad(GPIOB,6);
    } else {
        palClearPad(GPIOB,6);
    }
    if (c) {
        palSetPad(GPIOC,7);
    } else {
        palClearPad(GPIOC,7);
    }
}

static const ADCConversionGroup line_sensors_cfg1  = {
    FALSE,
    1,
    NULL,
    NULL,
    0,
    ADC_CR2_SWSTART,
    0,
    ADC_SMPR2_SMP_AN6(ADC_SAMPLE_3),
    ADC_SQR1_NUM_CH(1),
    0,
    ADC_SQR3_SQ1_N(ADC_CHANNEL_IN6)
};

static const ADCConversionGroup line_sensors_cfg2  = {
    FALSE,
    1,
    NULL,
    NULL,
    0,
    ADC_CR2_SWSTART,
    0,
    ADC_SMPR2_SMP_AN7(ADC_SAMPLE_3),
    ADC_SQR1_NUM_CH(1),
    0,
    ADC_SQR3_SQ1_N(ADC_CHANNEL_IN7)
};

static const ADCConversionGroup line_sensors_testcfg  = {
    FALSE,
    1,
    NULL,
    NULL,
    0,
    ADC_CR2_SWSTART,
    ADC_SMPR1_SMP_AN11(ADC_SAMPLE_3),
    0,
    ADC_SQR1_NUM_CH(1),
    0,
    ADC_SQR3_SQ1_N(ADC_CHANNEL_IN11)
};

THD_WORKING_AREA(waSensorThread, 512);
THD_FUNCTION(SensorThread, arg) {

    (void)arg;

    int16_t sensors[NUM_OF_SENSORS];
    adcsample_t sensor_value1;
    adcsample_t sensor_value2;

    while(1) {
        for (uint8_t i = 0; i < NUM_OF_SENSORS/2; i++) {
            mx_set(i);
            adcConvert(&ADCD1, &line_sensors_cfg1, &sensor_value1, 1);
            adcConvert(&ADCD1, &line_sensors_cfg2, &sensor_value2, 1);
            sensors[i] = sensor_value1;
            sensors[i+8] = sensor_value2;
        }
        // output
        
        for(int8_t i = 0; i < NUM_OF_SENSORS; i++)
        {
            int16_t output = sensors[i];
            if (output < 800) { output=1; }
            //chprintf(&SD1, "%5d ", output);
        }
        //chprintf(&SD1, "\n\r");
        chThdSleepMicroseconds(100);
    }

}

THD_WORKING_AREA(waSensorTestThread, 64);
THD_FUNCTION(SensorTestThread, arg) {

    (void)arg;

    adcsample_t sensor_value1;

    while(1) {
        //mx_set(7);
        adcConvert(&ADCD1, &line_sensors_cfg2 /*&line_sensors_testcfg*/, &sensor_value1, 1);
        chprintf(&SD1, "%d \n\r", sensor_value1);
        chThdSleepMicroseconds(100);
    }

}

void init_sensor_thread() {
    chThdCreateStatic(waSensorThread, sizeof(waSensorThread), NORMALPRIO, SensorThread, NULL);
    //chThdCreateStatic(waSensorTestThread, sizeof(waSensorTestThread), NORMALPRIO, SensorTestThread, NULL);
}
