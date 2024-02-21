/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_err.h"

#include "esp_at.h"
#include "esp_at_init.h"
#include "esp_blufi_api.h"
#include <string.h>

void app_main(void)
{
    esp_at_main_preprocess();

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_at_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_blufi_register_callbacks(NULL);

    esp_at_init();

    const char* to_at_port = "from_github_action";
    esp_at_port_write_data((uint8_t *)to_at_port, strlen(to_at_port));
}
