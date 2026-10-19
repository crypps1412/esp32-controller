#include "joystick.h"

#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_check.h"
#include "esp_idf_version.h"
#include "esp_log.h"

/* ADC_ATTEN_DB_12 is an enum (not a macro) and exists since IDF 5.2 */
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 2, 0)
#define JOY_ATTEN           ADC_ATTEN_DB_12
#else
#define JOY_ATTEN           ADC_ATTEN_DB_11
#endif

#define JOY_ADC_MAX         4095
#define JOY_CAL_SAMPLES     32
#define JOY_READ_SAMPLES    4

static const char *TAG = "joystick";

static adc_oneshot_unit_handle_t s_adc;
static int s_center_x = JOY_ADC_MAX / 2;
static int s_center_y = JOY_ADC_MAX / 2;

static int read_avg(adc_channel_t ch, int samples)
{
    int sum = 0;
    int n = 0;
    for (int i = 0; i < samples; i++) {
        int raw = 0;
        if (adc_oneshot_read(s_adc, ch, &raw) == ESP_OK) {
            sum += raw;
            n++;
        }
    }
    if (n == 0) {
        return ch == JOY_ADC_CH_X ? s_center_x : s_center_y;
    }
    return sum / n;
}

static int8_t scale_axis(int raw, int center, bool invert)
{
    int d = raw - center;
    int v;

    if (d > -JOY_DEADZONE && d < JOY_DEADZONE) {
        return 0;
    }
    if (d > 0) {
        int span = JOY_ADC_MAX - center - JOY_DEADZONE;
        v = span > 0 ? (d - JOY_DEADZONE) * 127 / span : 127;
    } else {
        int span = center - JOY_DEADZONE;
        v = span > 0 ? (d + JOY_DEADZONE) * 127 / span : -127;
    }
    if (v > 127) {
        v = 127;
    } else if (v < -127) {
        v = -127;
    }
    return (int8_t)(invert ? -v : v);
}

esp_err_t joystick_init(void)
{
    adc_oneshot_unit_init_cfg_t unit_cfg = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_RETURN_ON_ERROR(adc_oneshot_new_unit(&unit_cfg, &s_adc), TAG, "adc unit init failed");

    adc_oneshot_chan_cfg_t chan_cfg = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = JOY_ATTEN,
    };
    ESP_RETURN_ON_ERROR(adc_oneshot_config_channel(s_adc, JOY_ADC_CH_X, &chan_cfg), TAG, "config X failed");
    ESP_RETURN_ON_ERROR(adc_oneshot_config_channel(s_adc, JOY_ADC_CH_Y, &chan_cfg), TAG, "config Y failed");

    gpio_config_t io_cfg = {
        .pin_bit_mask = 1ULL << JOY_SW_GPIO,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_RETURN_ON_ERROR(gpio_config(&io_cfg), TAG, "button gpio config failed");

    s_center_x = read_avg(JOY_ADC_CH_X, JOY_CAL_SAMPLES);
    s_center_y = read_avg(JOY_ADC_CH_Y, JOY_CAL_SAMPLES);
    ESP_LOGI(TAG, "center x=%d y=%d", s_center_x, s_center_y);
    return ESP_OK;
}

void joystick_read_axes(int8_t *x, int8_t *y)
{
    *x = scale_axis(read_avg(JOY_ADC_CH_X, JOY_READ_SAMPLES), s_center_x, JOY_INVERT_X);
    *y = scale_axis(read_avg(JOY_ADC_CH_Y, JOY_READ_SAMPLES), s_center_y, JOY_INVERT_Y);
}

bool joystick_read_button(void)
{
    return gpio_get_level(JOY_SW_GPIO) == 0;
}
