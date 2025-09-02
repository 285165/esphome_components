import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor, i2c, sensor
from esphome.const import (
    CONF_BATTERY_LEVEL,
    CONF_BATTERY_VOLTAGE,
    CONF_CURRENT,
    CONF_ID,
    DEVICE_CLASS_BATTERY,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_VOLTAGE,
    ICON_BATTERY,
    ICON_FLASH,
    ICON_CURRENT_AC,
    STATE_CLASS_MEASUREMENT,
    UNIT_PERCENT,
    UNIT_VOLT,
    UNIT_AMPERE,
)

DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["binary_sensor"]

timerpwr_ns = cg.esphome_ns.namespace("timerpwr")
TIMERPWR = timerpwr_ns.class_(
    "TIMERPWR", cg.PollingComponent, i2c.I2CDevice
)

CONF_CHARGING = "charging"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(TIMERPWR),
            cv.Optional(CONF_BATTERY_LEVEL): sensor.sensor_schema(
                unit_of_measurement=UNIT_PERCENT,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_BATTERY,
                state_class=STATE_CLASS_MEASUREMENT,
                icon=ICON_BATTERY,
            ),
            cv.Optional(CONF_CHARGING): binary_sensor.binary_sensor_schema(
                icon=ICON_FLASH,
            ),
            cv.Optional(CONF_BATTERY_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                icon=ICON_FLASH,
                accuracy_decimals=3,
                device_class=DEVICE_CLASS_VOLTAGE
            ),
            cv.Optional(CONF_CURRENT): sensor.sensor_schema(
                unit_of_measurement=UNIT_AMPERE,
                icon=ICON_CURRENT_AC,
                accuracy_decimals=3,
                device_class=DEVICE_CLASS_CURRENT
            ),
        }
    )
    .extend(cv.polling_component_schema("10s"))
    .extend(i2c.i2c_device_schema(0x56))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    if conf := config.get(CONF_BATTERY_LEVEL):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_battery_level(sens))
    if conf := config.get(CONF_BATTERY_VOLTAGE):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_battery_voltage(sens))
    if conf := config.get(CONF_CURRENT):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_battery_current(sens))
    if conf := config.get(CONF_CHARGING):
        bsens = await binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_charging(bsens))

