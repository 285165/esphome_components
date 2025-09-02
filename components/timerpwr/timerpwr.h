#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace timerpwr {

class TIMERPWR : public PollingComponent, public i2c::I2CDevice {
 public:
  void set_battery_level(sensor::Sensor *battery_level) { this->battery_level_ = battery_level; }
  void set_battery_voltage(sensor::Sensor *battery_voltage) { this->battery_voltage_ = battery_voltage; }
  void set_battery_current(sensor::Sensor *battery_current) { this->battery_current_ = battery_current; }
  void set_charging(binary_sensor::BinarySensor *charging) { this->charging_ = charging; }

  void update() override;
  void setup() override;
  void loop() override;
  void dump_config() override;

  float get_setup_priority() const override;

 protected:
  sensor::Sensor *battery_level_{nullptr};
  sensor::Sensor *battery_voltage_{nullptr};
  sensor::Sensor *battery_current_{nullptr};
  binary_sensor::BinarySensor *charging_{nullptr};
};

}  // namespace timerpwr
}  // namespace esphome