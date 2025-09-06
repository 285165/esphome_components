#include "m5_timerpwr_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace m5_timerpwr {

static const char *const TAG = "m5_timerpwr.encoder";

void M5_timerpwrSensor::setup() {
  if (this->value_ < this->min_value_)
    this->value_ = this->min_value_;
  if (this->value_ > this->max_value_)
    this->value_ = this->max_value_;
  this->publish_state(this->value_);
}

void M5_timerpwrSensor::dump_config() {
  LOG_SENSOR("", "M5_timerpwr", this);
  // ESP_LOGCONFIG(TAG, "rotary encoder number %d", this->index_);
}

void M5_timerpwrSensor::on_update(int32_t value) {
  int32_t new_value = this->value_ + value;
  if (new_value < this->min_value_)
    new_value = this->min_value_;
  if (new_value > this->max_value_)
    new_value = this->max_value_;
  if (new_value == this->value_)
    return;
  this->value_ = new_value;
  this->publish_state(new_value);
}

}  // namespace m5_timerpwr
}  // namespace esphome
