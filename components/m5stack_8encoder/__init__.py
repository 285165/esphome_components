import esphome.codegen as cg
from esphome.components import i2c
import esphome.config_validation as cv
from esphome.const import CONF_ID

DEPENDENCIES = ["i2c"]
CODEOWNERS = ["@285165"]
MULTI_CONF = True

CONF_M5STACK_8ENCODER_ID = "m5stack_8encoder_id"

m5stack_8encoder_ns = cg.esphome_ns.namespace("m5stack_8encoder")
M5Stack8EncoderComponent = m5stack_8encoder_ns.class_(
    "M5Stack8EncoderComponent",
    i2c.I2CDevice,
    cg.Component,
)

AnalogBits = m5stack_8encoder_ns.enum("AnalogBits")


CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(M5Stack8EncoderComponent),
    }
).extend(i2c.i2c_device_schema(0x41))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)