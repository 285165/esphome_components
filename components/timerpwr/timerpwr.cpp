#include "timerpwr.h"
#include "esphome/core/log.h"

namespace esphome {
namespace timerpwr {

static const char *const TAG = "timerpwr.sensor";

static const uint8_t AXP2101_REGISTER_PMU_STATUS2 = 0x90;
static const uint8_t AXP2101_REGISTER_BATTERY_LEVEL = 0x70;
static const uint8_t AXP2101_REGISTER_BATTERY_VOLTAGE = 0x70;
static const uint8_t AXP2101_REGISTER_BATTERY_CURRENT = 0x74;
static const uint8_t TIMERPWR_REGISTER_USB_VOLTAGE = 0x60;
static const uint8_t TIMERPWR_REGISTER_USB_CURRENT = 0x64;

float TIMERPWR::get_setup_priority() const { return setup_priority::DATA; }

void TIMERPWR::update() {
  uint8_t data;

  uint8_t battery_voltage0;
  uint8_t battery_voltage1;
  uint8_t battery_current0;
  uint8_t battery_current1;
  uint8_t battery_current2;
  float battery_level;
  float battery_voltage;
  float battery_current;
  float bus_voltage;
  float bus_current;
  const float V_max = 4.2;
  const float V_min = 3.2;

  uint8_t usb_voltage[4];
  if (this->read_register(TIMERPWR_REGISTER_USB_VOLTAGE, usb_voltage, 4) != i2c::ERROR_OK) {
      ESP_LOGE(TAG, "unable to read USB voltage");
      this->mark_failed();
      return;
  } else {
      if (this->usb_voltage_ != nullptr) {
      bus_voltage = (usb_voltage[1]*256+usb_voltage[0])/100.0;
      ESP_LOGI(TAG, "USB voltage: %.2f", usb_voltage );
      this->usb_voltage_->publish_state(bus_voltage);
    }
  }
  uint8_t usb_current[4];
  if (this->read_register(TIMERPWR_REGISTER_USB_CURRENT, usb_current, 4) != i2c::ERROR_OK) {
      ESP_LOGE(TAG, "unable to read USB current");
      this->mark_failed();
      return;
  } else {
      if (this->usb_current_ != nullptr) {
      bus_current = (usb_current[1]*256+usb_current[0])/100.0;
      ESP_LOGI(TAG, "USB current: %.2f", usb_current );
      this->usb_current_->publish_state(bus_current);
    }
  }

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
  }
  if (this->battery_voltage_ != nullptr) {
    battery_voltage = (battery_voltage1*256+battery_voltage0)/100.0;
    ESP_LOGI(TAG, "Battery voltage: %.2f", battery_voltage );
    this->battery_voltage_->publish_state(battery_voltage);
  }

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
  if (this->read_register(AXP2101_REGISTER_BATTERY_CURRENT+3, &battery_current2, 1) != i2c::NO_ERROR) {
    ESP_LOGE(TAG, "Unable to read from device");
    return;
  } else {
    ESP_LOGI(TAG, "Battery current2 read: %d", battery_current2 );
  }
  if (this->battery_current_ != nullptr)
    battery_current = (battery_current2*65536+battery_current1*256+battery_current0)/100.0;
    ESP_LOGI(TAG, "Battery current: %.3f", battery_current);
    this->battery_current_->publish_state(battery_current);

 if (this->read_register(AXP2101_REGISTER_BATTERY_LEVEL, &data, 1) != i2c::NO_ERROR) {
    ESP_LOGE(TAG, "Unable to read from device");
    return;
  }
  if (this->battery_level_ != nullptr)
    battery_level = ((battery_voltage - V_min) / (V_max - V_min)) * 100;
    ESP_LOGI(TAG, "Battery level: %.2f", battery_level );
    this->battery_level_->publish_state(battery_level);
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
  LOG_SENSOR(" ", "Battery Level", this->battery_level_);
  LOG_SENSOR(" ", "Battery Voltage", this->battery_voltage_);
  LOG_SENSOR(" ", "Battery Current", this->battery_current_);
}

}  // namespace timerpwr
}  // namespace esphome
