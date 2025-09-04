#include "timerpwr.h"
#include "esphome/core/log.h"

namespace esphome {
namespace timerpwr {

static const char *const TAG = "timerpwr.sensor";

static const uint8_t AXP2101_REGISTER_PMU_STATUS2 = 0x90;
static const uint8_t AXP2101_REGISTER_BATTERY_LEVEL = 0x70;
static const uint8_t TIMERPWR_REGISTER_BATTERY_VOLTAGE = 0x70;
static const uint8_t TIMERPWR_REGISTER_BATTERY_CURRENT = 0x74;
static const uint8_t TIMERPWR_REGISTER_USB_VOLTAGE = 0x60;
static const uint8_t TIMERPWR_REGISTER_USB_CURRENT = 0x64;

float TIMERPWR::get_setup_priority() const { return setup_priority::DATA; }

void TIMERPWR::update() {
  uint8_t data;

  float battery_level;
  float battery_voltage_f;
  float battery_current_f;
  float usb_voltage_f;
  float usb_current_f;
  const float V_max = 4.2;
  const float V_min = 3.2;

  uint8_t bytes[4]{ 0xff, 0xff, 0xff, 0xff };
  int32_t value;
  std::memcpy(&value, bytes, sizeof(int32_t));
  ESP_LOGI(TAG, "ttt %d", value );

  uint8_t usb_voltage[4];
  // std::int32_t usb_voltage; 
  if (this->read_register(TIMERPWR_REGISTER_USB_VOLTAGE, usb_voltage, 4) != i2c::ERROR_OK) {
      ESP_LOGE(TAG, "unable to read USB voltage");
      this->mark_failed();
      return;
  } else {
      if (this->usb_voltage_ != nullptr) {
      usb_voltage_f = (usb_voltage[1]*256+usb_voltage[0])/100.0;
      // usb_voltage_f = usb_voltage/100.0;

      ESP_LOGI(TAG, "USB voltage: %.2f", usb_voltage_f );
      // ESP_LOGD(TAG, "USB voltage: %d %d %d %d", usb_voltage[3],usb_voltage[2],usb_voltage[1],usb_voltage[0]);
      this->usb_voltage_->publish_state(usb_voltage_f);
    }
  }
  uint8_t usb_current[4];
  if (this->read_register(TIMERPWR_REGISTER_USB_CURRENT, usb_current, 4) != i2c::ERROR_OK) {
      ESP_LOGE(TAG, "unable to read USB current");
      this->mark_failed();
      return;
  } else {
      if (this->usb_current_ != nullptr) {
      usb_current_f = (65536*usb_current[2]+256*usb_current[1]+usb_current[0])/100.0;
      // usb_current_f = (int16_t*)usb_current/100.0;
      ESP_LOGI(TAG, "USB current: %.2f", usb_current_f );
      ESP_LOGD(TAG, "USB current: %d %d %d %d", usb_current[3],usb_current[2],usb_current[1],usb_current[0]);
      this->usb_current_->publish_state(usb_current_f);
    }
  }

  uint8_t battery_voltage[4];
  if (this->read_register(TIMERPWR_REGISTER_BATTERY_VOLTAGE, battery_voltage, 4) != i2c::NO_ERROR) {
    ESP_LOGE(TAG, "Unable to read from device");
    return;
  } else {
      if (this->battery_voltage_ != nullptr) {
      battery_voltage_f = (65536*battery_voltage[2]+256*battery_voltage[1]+battery_voltage[0])/100.0;
      ESP_LOGI(TAG, "Battery voltage read: %.2f", battery_voltage_f );
      ESP_LOGD(TAG, "Battery voltage: %d %d %d %d", battery_voltage[3],battery_voltage[2],battery_voltage[1],battery_voltage[0]);
      this->battery_voltage_->publish_state(battery_voltage_f);  
    }
  }

    uint8_t battery_current[4];
  if (this->read_register(TIMERPWR_REGISTER_BATTERY_CURRENT, battery_current, 4) != i2c::NO_ERROR) {
    ESP_LOGE(TAG, "Unable to read from device");
    return;
  } else {
      if (this->battery_current_ != nullptr) {
      battery_current_f = (65536*battery_current[2]+256*battery_current[1]+battery_current[0])/100.0;
      // battery_current_f = (int16_t*)battery_current/100.0;
      std::memcpy(&value, battery_current, sizeof(int32_t));
      ESP_LOGI(TAG, "ttt %d", value );
      battery_current_f = value/100.0;

      ESP_LOGI(TAG, "Battery current read: %.2f",battery_current_f );
      ESP_LOGD(TAG, "Battery current: %d %d %d %d", battery_current[3],battery_current[2],battery_current[1],battery_current[0]);
      this->battery_current_->publish_state(battery_current_f);  
    }
  }

 if (this->read_register(AXP2101_REGISTER_BATTERY_LEVEL, &data, 1) != i2c::NO_ERROR) {
    ESP_LOGE(TAG, "Unable to read from device");
    return;
  }
  if (this->battery_level_ != nullptr)
    battery_level = ((battery_voltage_f - V_min) / (V_max - V_min)) * 100;
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
  LOG_SENSOR(" ", "USB Voltage", this->usb_voltage_);
  LOG_SENSOR(" ", "USB Current", this->usb_current_);
}

}  // namespace timerpwr
}  // namespace esphome
