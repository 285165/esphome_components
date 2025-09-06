import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv

from .. import CONF_M5STACK_8ENCODER_ID, M5Stack8EncoderComponent, m5stack_8encoder_ns

M5Stack8EncoderSwitchBinarySensor = m5stack_8encoder_ns.class_(
    "M5Stack8EncoderSwitchBinarySensor",
    binary_sensor.BinarySensor,
    cg.PollingComponent,
)


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(CONF_M5STACK_8ENCODER_ID): cv.use_id(M5Stack8EncoderComponent),
        }
    )
    .extend(binary_sensor.binary_sensor_schema(M5Stack8EncoderSwitchBinarySensor))
    .extend(cv.polling_component_schema("10s"))
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_M5STACK_8ENCODER_ID])
    sens = await binary_sensor.new_binary_sensor(config)
    cg.add(sens.set_parent(hub))
    await cg.register_component(sens, config)