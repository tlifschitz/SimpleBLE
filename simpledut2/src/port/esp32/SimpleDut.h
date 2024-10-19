#include <stdio.h>
#include "SimpleDutBase.h"
#include "driver/usb_serial_jtag.h"
#include "esp_check.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

#define BUF_SIZE (1024)
#define ECHO_TASK_STACK_SIZE (4096)

class SimpleDut : public SimpleDutBase {
  public:
    SimpleDut() {
        // Configure USB SERIAL JTAG
        mSerialConfig.rx_buffer_size = BUF_SIZE;
        mSerialConfig.tx_buffer_size = BUF_SIZE;

        ESP_ERROR_CHECK(usb_serial_jtag_driver_install(&mSerialConfig));
        ESP_LOGI("usb_serial_jtag echo", "USB_SERIAL_JTAG init done");
    }

    void getCommand(std::string& command) override {
        command.clear();
        while (true) {
            int len = usb_serial_jtag_read_bytes(mData, (BUF_SIZE - 1), portMAX_DELAY);
            if (len > 0) {
                mData[len] = '\0';
                command += std::string((char*)mData);
                if (command.find('\n') != std::string::npos) {
                    break;
                }
            }
        }
    }

    void output(const std::string& message) override {
        usb_serial_jtag_write_bytes((const char*)message.c_str(), message.length(), 20 / portTICK_PERIOD_MS);
        if (!message.empty() && message.back() != '\n') {
            usb_serial_jtag_write_bytes("\n", 1, 20 / portTICK_PERIOD_MS);
        }
        usb_serial_jtag_write_bytes("> ", 2, 20 / portTICK_PERIOD_MS);
    }

  private:
    usb_serial_jtag_driver_config_t mSerialConfig;
    uint8_t mData[BUF_SIZE];
};