#include "m5stack_8encoder_binary_sensor.h"

namespace esphome {
namespace m5stack_8encoder {

void M5Stack8EncoderSwitchBinarySensor::update() {
  int8_t out = this->parent_->read_switch();
  if (out == -1) {
    this->status_set_warning("Could not read binary sensor state from M5Stack 8Encoder.");
    return;
  }
  this->publish_state(out != 0);
  this->status_clear_warning();
}

}  // namespace m5stack_8encoder
}  // namespace esphome