#include "timerpwr.h"
#include "esphome/core/log.h"

namespace esphome {
namespace timerpwr {

static const char *const TAG = "timerpwr.sensor";

static const uint8_t AXP2101_REGISTER_PMU_STATUS2 = 0x90;
static const uint8_t AXP2101_REGISTER_BATTERY_LEVEL = 0x70;
static const uint8_t AXP2101_REGISTER_BATTERY_VOLTAGE = 0x70;
static const uint8_t AXP2101_REGISTER_BATTERY_CURRENT = 0x74;

float TIMERPWR::get_setup_priority() const { return setup_priority::DATA; }

void TIMERPWR::update() {
  uint8_t data;
  uint8_t battery_voltage0;
  uint8_t battery_voltage1;
  uint8_t battery_current0;
  uint8_t battery_current1;

  if (this->read_register(AXP2101_REGISTER_BATTERY_LEVEL, &data, 1) != i2c::NO_ERROR) {
    ESP_LOGE(TAG, "Unable to read from device");
    return;
  }
  if (this->battery_level_ != nullptr)
    this->battery_level_->publish_state(80);
  
  if (this->read_register(AXP2101_REGISTER_BATTERY_VOLTAGE, &battery_voltage0, 1) != i2c::NO_ERROR) {
    ESP_LOGE(TAG, "Unable to read from device");
    return;
  } else {
    ESP_LOGI(TAG, "Battery voltage0 read: %d", battery_voltage0 );
  }
  if (this->read_register(AXP2101_REGISTER_BATTERY_VOLTAGE+1, &battery_voltage1, 1) != i2c::NO_ERROR) {
    ESP_LOGE(TAG, "Unable to read from device");
    return;
  } else {
    ESP_LOGI(TAG, "Battery voltage1 read: %d", battery_voltage1 );
    ESP_LOGI(TAG, "Battery voltage: %.2f", (battery_voltage1*256+battery_voltage0)/100.0 );
    // this->battery_voltage_->publish_state((battery_voltage1*256+battery_voltage0)/100.0);
  }
  if (this->battery_voltage_ != nullptr)
    this->battery_voltage_->publish_state(80.0);

  if (this->read_register(AXP2101_REGISTER_BATTERY_CURRENT, &battery_current0, 1) != i2c::NO_ERROR) {
    ESP_LOGE(TAG, "Unable to read from device");
    return;
  } else {
    ESP_LOGI(TAG, "Battery current0 read: %d", battery_current0 );
  }
  if (this->read_register(AXP2101_REGISTER_BATTERY_CURRENT+1, &battery_current1, 1) != i2c::NO_ERROR) {
    ESP_LOGE(TAG, "Unable to read from device");
    return;
  } else {
    ESP_LOGI(TAG, "Battery current1 read: %d", battery_current1 );
  }
  if (this->battery_current_ != nullptr)
    float voltage = (battery_current1*256+battery_current0)/100.0;
    ESP_LOGI(TAG, "Battery current: %.2f", voltage );    
    this->battery_current_->publish_state(voltage);
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
