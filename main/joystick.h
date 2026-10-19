#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"

/* HW-504 wiring (module powered from 3.3V, ADC1 only: ADC2 is unusable with BT) */
#define JOY_VRX_GPIO        34  /* ADC1_CHANNEL_6 */
#define JOY_VRY_GPIO        35  /* ADC1_CHANNEL_7 */
#define JOY_SW_GPIO         32  /* active LOW, internal pull-up */
#define JOY_ADC_CH_X        ADC_CHANNEL_6
#define JOY_ADC_CH_Y        ADC_CHANNEL_7

#define JOY_INVERT_X        0
#define JOY_INVERT_Y        1   /* push up -> negative Y (HID convention) */
#define JOY_DEADZONE        250 /* raw counts around center (~6% of range) */

esp_err_t joystick_init(void);
void joystick_read_axes(int8_t *x, int8_t *y);
bool joystick_read_button(void);
