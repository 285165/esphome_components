#include "m5stack_8angle.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"

namespace esphome {
namespace m5stack_8encoder {

static const char *const TAG = "m5stack_8encoder";

void M5Stack8EncoderComponent::setup() {
  i2c::ErrorCode err;

  err = this->read(nullptr, 0);
  if (err != i2c::NO_ERROR) {
    ESP_LOGE(TAG, "I2C error %02X", err);
    this->mark_failed();
    return;
  };

  err = this->read_register(M5STACK_8ENCODER_REGISTER_FW_VERSION, &this->fw_version_, 1);
  if (err != i2c::NO_ERROR) {
    ESP_LOGE(TAG, "I2C error %02X", err);
    this->mark_failed();
    return;
  };
}

void M5Stack8EncoderComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "M5STACK_8ENCODER:");
  LOG_I2C_DEVICE(this);
  ESP_LOGCONFIG(TAG, "  Firmware version: %d ", this->fw_version_);
}

float M5Stack8EncoderComponent::read_knob_pos(uint8_t channel, AnalogBits bits) {
  int32_t raw_pos = this->read_knob_pos_raw(channel, bits);
  if (raw_pos == -1) {
    return NAN;
  }
  return (float) raw_pos / ((1 << bits) - 1);
}

int32_t M5Stack8EncoderComponent::read_knob_pos_raw(uint8_t channel, AnalogBits bits) {
  uint16_t knob_pos = 0;
  i2c::ErrorCode err;
  if (bits == BITS_8) {
    err = this->read_register(M5STACK_8ANGLE_REGISTER_ANALOG_INPUT_8B + channel, (uint8_t *) &knob_pos, 1);
  } else if (bits == BITS_12) {
    err = this->read_register(M5STACK_8ANGLE_REGISTER_ANALOG_INPUT_12B + (channel * 2), (uint8_t *) &knob_pos, 2);
  } else {
    ESP_LOGE(TAG, "Invalid number of bits: %d", bits);
    return -1;
  }
  if (err == i2c::NO_ERROR) {
    return knob_pos;
  } else {
    return -1;
  }
}

int8_t M5Stack8EncoderComponent::read_switch() {
  uint8_t out;
  i2c::ErrorCode err = this->read_register(M5STACK_8ANGLE_REGISTER_DIGITAL_INPUT, (uint8_t *) &out, 1);
  if (err == i2c::NO_ERROR) {
    return out ? 1 : 0;
  } else {
    return -1;
  }
}

float M5Stack8EncoderComponent::get_setup_priority() const { return setup_priority::DATA; }

}  // namespace m5stack_8encoder
}  // namespace esphome