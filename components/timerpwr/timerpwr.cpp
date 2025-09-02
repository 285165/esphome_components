#include "timerpwr.h"
#include "esphome/core/log.h"

namespace esphome {
namespace timerpwr {

static const char *const TAG = "timerpwr.sensor";

static const uint8_t AXP2101_REGISTER_PMU_STATUS2 = 0x90;
static const uint8_t AXP2101_REGISTER_BATTERY_LEVEL = 0x70;

float TIMERPWR::get_setup_priority() const { return setup_priority::DATA; }

void TIMERPWR::update() {
  uint8_t data;
  if (this->read_register(AXP2101_REGISTER_BATTERY_LEVEL, &data, 1) != i2c::NO_ERROR) {
    ESP_LOGE(TAG, "Unable to read from device");
    return;
  }
  if (this->battery_level_ != nullptr)
    this->battery_level_->publish_state(data);
}

void TIMERPWR::setup() {
  ESP_LOGCONFIG(TAG, "Setting up TIMERPWR...");
}

void TIMERPWR::loop() {
  uint8_t data;
  if (this->charging_ != nullptr) {
    if (this->read_register(AXP2101_REGISTER_PMU_STATUS2, &data, 1) != i2c::NO_ERROR)
      return;
    this->charging_->publish_state((data & 0x01) == 0x01);
  }
}

void TIMERPWR::dump_config() {
  ESP_LOGCONFIG(TAG, "TIMERPWR:");
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Connection with TIMERPWR failed!");
  }
  LOG_UPDATE_INTERVAL(this);
  LOG_SENSOR("  ", "Battery Level", this->battery_level_);
}

}  // namespace timerpwr
}  // namespace esphome
