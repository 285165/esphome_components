#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "../m5_timerpwr.h"

namespace esphome {
namespace m5_timerpwr {

class M5_timerpwrSensor : public sensor::Sensor, public Component {
 public:
  void setup() override;
  void dump_config() override;

  void on_update(int32_t value) override;

 protected:
  uint8_t number_{0};
  int32_t value_{0};
  int32_t min_value_{INT32_MIN};
  int32_t max_value_{INT32_MAX};
};

}  // namespace m5_timerpwr
}  // namespace esphome
