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
#include "esp_at_core.h"
#include "esp_blufi_api.h"
#include <string.h>
#include <stdio.h>

static void example_event_callback(esp_blufi_cb_event_t event, esp_blufi_cb_param_t *param);
bool esp_at_ublufi_cmd_regist(void);
void app_main(void)
{
    esp_at_main_preprocess();

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_at_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_at_init();

    // int reg_ret = esp_blufi_register_callbacks(example_event_callback);
    int reg_ret = 0;

    // TODO blufi_api.h
    // esp_blufi_send_custom_data_p2e
    // nrf connect <---> esp32_prole(as a phone phone)

    const char* to_at_port = "from_github_action";
    esp_at_port_write_data((uint8_t *)to_at_port, strlen(to_at_port));
    // esp_at_port_write_data((uint8_t *)((reg_ret == -1)? "f" : "s"), strlen("f"));
    // esp_blufi_send_custom_data_p2e((uint8_t *)to_at_port, strlen(to_at_port));
    reg_ret = esp_at_ublufi_cmd_regist();
    esp_at_port_write_data((uint8_t *)((reg_ret == -1)? "f" : "s"), strlen("f"));
}

static void  _print_to_at_string(char* str)
{
    esp_at_port_write_data((uint8_t*)str, strlen(str));
}

static void _print_to_at_hex(uint8_t* hex, uint32_t length)
{
    // 1 byte hex <--> 2 bytes of string
    uint32_t expected_size = 2 * length + 1;
    uint8_t arr[expected_size];
    memset(arr, 0, expected_size);
    // snprintf(arr, expected_size, "%X", )
}

static void example_event_callback(esp_blufi_cb_event_t event, esp_blufi_cb_param_t *param)
{
    switch (event) {
    case ESP_BLUFI_EVENT_RECV_CUSTOM_DATA:
        _print_to_at_string("ESP_BLUFI_EVENT_RECV_CUSTOM_DATA");
        break;
    default:
        break;
    }
}



uint8_t at_testCmd(uint8_t *cmd_name)
{
    _print_to_at_string("at_testCmd\r\n");
    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_queryCmd(uint8_t *cmd_name)
{
    _print_to_at_string("at_queryCmd\r\n");
    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_setupCmd(uint8_t para_num)
{
    _print_to_at_string("at_setupCmd\r\n");
    return ESP_AT_RESULT_CODE_OK;
}

uint8_t at_exeCmd(uint8_t *cmd_name)
{
    _print_to_at_string("at_exeCmd\r\n");
    const char* to_at_port = "from_github_action";
    esp_blufi_send_custom_data_p2e((uint8_t *)to_at_port, strlen(to_at_port));
    return ESP_AT_RESULT_CODE_OK;
}

static const esp_at_cmd_struct at_uart_cmd[] = {
    {"+UBLUFISEND", at_testCmd, at_queryCmd, at_setupCmd, at_exeCmd},
};

bool esp_at_ublufi_cmd_regist(void)
{
    return esp_at_custom_cmd_array_regist(at_uart_cmd, sizeof(at_uart_cmd) / sizeof(esp_at_cmd_struct));
}

ESP_AT_CMD_SET_FIRST_INIT_FN(esp_at_ublufi_cmd_regist);
