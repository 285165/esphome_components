import esphome.codegen as cg
from esphome.components import light
import esphome.config_validation as cv
from esphome.const import CONF_OUTPUT_ID

from .. import CONF_M5STACK_8ENCODER_ID, M5Stack8EncoderComponent, m5stack_8encoder_ns

M5Stack8EncoderLightsComponent = m5stack_8encoder_ns.class_(
    "M5Stack8EncoderLightOutput",
    light.AddressableLight,
)


CONFIG_SCHEMA = cv.All(
    light.ADDRESSABLE_LIGHT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_M5STACK_8ENCODER_ID): cv.use_id(M5Stack8EncoderComponent),
            cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(M5Stack8EncoderLightsComponent),
        }
    )
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_M5STACK_8ENCODER_ID])
    lights = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await light.register_light(lights, config)
    await cg.register_component(lights, config)
    cg.add(lights.set_parent(hub))