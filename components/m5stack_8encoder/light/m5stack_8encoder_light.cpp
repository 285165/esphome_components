#include "m5stack_8encoder_light.h"

#include "esphome/core/log.h"

namespace esphome {
namespace m5stack_8encoder {

static const char *const TAG = "m5stack_8encoder.light";

void M5Stack8EncoderLightOutput::setup() {
  RAMAllocator<uint8_t> allocator;
  this->buf_ = allocator.allocate(M5STACK_8ENCODER_NUM_LEDS * M5STACK_8ENCODER_BYTES_PER_LED);
  if (this->buf_ == nullptr) {
    ESP_LOGE(TAG, "Failed to allocate buffer of size %u", M5STACK_8ENCODFER_NUM_LEDS * M5STACK_8ENCODER_BYTES_PER_LED);
    this->mark_failed();
    return;
  };
  memset(this->buf_, 0xFF, M5STACK_8ENCODER_NUM_LEDS * M5STACK_8ENCODER_BYTES_PER_LED);

  this->effect_data_ = allocator.allocate(M5STACK_8ENCODER_NUM_LEDS);
  if (this->effect_data_ == nullptr) {
    ESP_LOGE(TAG, "Failed to allocate effect data of size %u", M5STACK_8ENCODER_NUM_LEDS);
    this->mark_failed();
    return;
  };
  memset(this->effect_data_, 0x00, M5STACK_8ENCODER_NUM_LEDS);
}

void M5Stack8EncoderLightOutput::write_state(light::LightState *state) {
  for (int i = 0; i < M5STACK_8ENCODER_NUM_LEDS;
       i++) {  // write one LED at a time, otherwise the message will be truncated
    this->parent_->write_register(M5STACK_8ENCODER_REGISTER_RGB_24B + i * M5STACK_8ENCODER_BYTES_PER_LED,
                                  this->buf_ + i * M5STACK_8ENCODER_BYTES_PER_LED, M5STACK_8ENCODER_BYTES_PER_LED);
  }
}

light::ESPColorView M5Stack8EncoderLightOutput::get_view_internal(int32_t index) const {
  size_t pos = index * M5STACK_8ENCODER_BYTES_PER_LED;
  // red, green, blue, white, effect_data, color_correction
  return {this->buf_ + pos, this->buf_ + pos + 1,       this->buf_ + pos + 2,
          nullptr,          this->effect_data_ + index, &this->correction_};
}

}  // namespace m5stack_8encoder
}  // namespace esphome