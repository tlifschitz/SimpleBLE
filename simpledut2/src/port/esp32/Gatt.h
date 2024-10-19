#include "GattBase.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_bt.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "nvs_flash.h"

#include "esp_bt_defs.h"
#include "esp_bt_device.h"
#include "esp_bt_main.h"
#include "esp_gap_ble_api.h"
#include "esp_gatt_common_api.h"
#include "esp_gatts_api.h"

#include "sdkconfig.h"

class Gatt : public GattBase {
  public:
    std::string start(const std::string& name, int interval) override { return "starting gatt server"; }

    std::string stop() override { return "stopping gatt server"; }

    std::string status() override { return "gatt server status"; }

    std::string disconnect() override { return "disconnecting from device"; }
};