#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_hidd.h"

#include "esp_hid_gap.h"
#include "joystick.h"

#define DEVICE_NAME     "ESP32 Joystick"
#define REPORT_ID       1
#define REPORT_LEN      3
#define POLL_PERIOD_MS  10
#define KEEPALIVE_MS    1000
#define DEBUG_PRINT     1       /* 1 = print joystick state over USB serial */
#define DEBUG_PERIOD_MS 100

static const char *TAG = "main";

static const uint8_t s_report_map[] = {
    0x05, 0x01,        /* Usage Page (Generic Desktop) */
    0x09, 0x05,        /* Usage (Gamepad) */
    0xA1, 0x01,        /* Collection (Application) */
    0x85, REPORT_ID,   /*   Report ID (1) */
    0x05, 0x09,        /*   Usage Page (Button) */
    0x19, 0x01,        /*   Usage Minimum (1) */
    0x29, 0x01,        /*   Usage Maximum (1) */
    0x15, 0x00,        /*   Logical Minimum (0) */
    0x25, 0x01,        /*   Logical Maximum (1) */
    0x75, 0x01,        /*   Report Size (1) */
    0x95, 0x01,        /*   Report Count (1) */
    0x81, 0x02,        /*   Input (Data,Var,Abs) */
    0x75, 0x07,        /*   Report Size (7) */
    0x95, 0x01,        /*   Report Count (1) */
    0x81, 0x03,        /*   Input (Const,Var,Abs) - padding */
    0x05, 0x01,        /*   Usage Page (Generic Desktop) */
    0x09, 0x30,        /*   Usage (X) */
    0x09, 0x31,        /*   Usage (Y) */
    0x15, 0x81,        /*   Logical Minimum (-127) */
    0x25, 0x7F,        /*   Logical Maximum (127) */
    0x75, 0x08,        /*   Report Size (8) */
    0x95, 0x02,        /*   Report Count (2) */
    0x81, 0x02,        /*   Input (Data,Var,Abs) */
    0xC0,              /* End Collection */
};

static esp_hid_raw_report_map_t s_report_maps[] = {
    {
        .data = s_report_map,
        .len = sizeof(s_report_map),
    },
};

static esp_hid_device_config_t s_hid_config = {
    .vendor_id          = 0x16C0,
    .product_id         = 0x05DF,
    .version            = 0x0100,
    .device_name        = DEVICE_NAME,
    .manufacturer_name  = "Espressif",
    .serial_number      = "1234567890",
    .report_maps        = s_report_maps,
    .report_maps_len    = 1,
};

static esp_hidd_dev_t *s_hid_dev;
static volatile bool s_connected;
static volatile bool s_resend;
static volatile uint32_t s_conn_gen;

/* Called by esp_hid_gap.c when BLE pairing/auth completes. The input task is
 * always running and gated on s_connected, so nothing to start here. */
void ble_hid_task_start_up(void)
{
    ESP_LOGI(TAG, "BLE auth complete");
    s_resend = true;
}

static void hidd_event_callback(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    esp_hidd_event_t event = (esp_hidd_event_t)id;

    switch (event) {
    case ESP_HIDD_START_EVENT:
        ESP_LOGI(TAG, "HID started, advertising");
        esp_hid_ble_gap_adv_start();
        break;
    case ESP_HIDD_CONNECT_EVENT:
        ESP_LOGI(TAG, "Connected");
        s_conn_gen++;
        s_connected = true;
        break;
    case ESP_HIDD_DISCONNECT_EVENT:
        ESP_LOGI(TAG, "Disconnected, restarting advertising");
        s_conn_gen++;
        s_connected = false;
        esp_hid_ble_gap_adv_start();
        break;
    default:
        break;
    }
}

static void joystick_task(void *arg)
{
    uint8_t last[REPORT_LEN] = {0};
    bool force = true;
    uint32_t gen = s_conn_gen;
    TickType_t last_sent = 0;

    while (1) {
        uint8_t buf[REPORT_LEN];
        int8_t x, y;

        joystick_read_axes(&x, &y);
        buf[0] = joystick_read_button() ? 0x01 : 0x00;
        buf[1] = (uint8_t)x;
        buf[2] = (uint8_t)y;

#if DEBUG_PRINT
        static TickType_t last_dbg;
        if ((xTaskGetTickCount() - last_dbg) >= pdMS_TO_TICKS(DEBUG_PERIOD_MS)) {
            last_dbg = xTaskGetTickCount();
            ESP_LOGI(TAG, "x=%4d y=%4d btn=%d %s", x, y, buf[0],
                     s_connected ? "connected" : "not connected");
        }
#endif

        if (gen != s_conn_gen) {
            gen = s_conn_gen;
            force = true;
        }
        if (s_resend) {
            s_resend = false;
            force = true;
        }

        if (s_connected) {
            TickType_t now = xTaskGetTickCount();
            if (force || memcmp(buf, last, REPORT_LEN) != 0 ||
                (now - last_sent) >= pdMS_TO_TICKS(KEEPALIVE_MS)) {
                if (esp_hidd_dev_input_set(s_hid_dev, 0, REPORT_ID, buf, REPORT_LEN) == ESP_OK) {
                    memcpy(last, buf, REPORT_LEN);
                    force = false;
                    last_sent = now;
                }
            }
        } else {
            force = true;
        }
        vTaskDelay(pdMS_TO_TICKS(POLL_PERIOD_MS));
    }
}

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(joystick_init());

    ESP_ERROR_CHECK(esp_hid_gap_init(HID_DEV_MODE));
    ESP_ERROR_CHECK(esp_hid_ble_gap_adv_init(ESP_HID_APPEARANCE_GAMEPAD, DEVICE_NAME));
    ESP_ERROR_CHECK(esp_ble_gatts_register_callback(esp_hidd_gatts_event_handler));
    ESP_ERROR_CHECK(esp_hidd_dev_init(&s_hid_config, ESP_HID_TRANSPORT_BLE, hidd_event_callback, &s_hid_dev));

    if (xTaskCreate(joystick_task, "joystick", 4096, NULL, 5, NULL) != pdPASS) {
        ESP_LOGE(TAG, "Failed to create joystick task");
    }
}
