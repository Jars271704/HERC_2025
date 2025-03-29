#include <DriverEncoder.h>

namespace DriverEncoder {
    namespace ENCODERS {
        ENCODER FORWARD_LEFT = ENCODER(PCNT_UNIT_0,4,5,0.24f);
    }
}

void DriverEncoder::start()
{
    configEncoder(&ENCODERS::FORWARD_LEFT);
}


float DriverEncoder::getVelocity(ENCODERS::ENCODER *encoder)
{
    int16_t encoder_value;
    pcnt_get_counter_value(encoder->PCNT_UNIT, &encoder_value);
    int16_t dif_encoder = 0;
    if(encoder->MODE_FORWARD) {
        if(encoder->previous_encoder_value < encoder_value) dif_encoder = encoder_value - encoder->previous_encoder_value;
        else if(encoder->previous_encoder_value > encoder_value) dif_encoder = encoder_value + (CONFIG::MAX_COUNTER - encoder->previous_encoder_value);
    }
    else {
        if(encoder->previous_encoder_value > encoder_value) dif_encoder = encoder->previous_encoder_value - encoder_value;
        else if(encoder->previous_encoder_value < encoder_value) dif_encoder = encoder->previous_encoder_value + (CONFIG::MAX_COUNTER - encoder_value);
    }
    uint32_t time_ = millis();
    uint32_t dif_time = time_ - encoder->previous_time;

    float speed = ((float)dif_encoder / (float)dif_time) * encoder->encoder_factor;

    encoder->previous_encoder_value = encoder_value;
    encoder->previous_time = time_;
    encoder->speed = speed;
    return speed;
}

void DriverEncoder::configEncoder(ENCODERS::ENCODER* encoder)
{
    pcnt_config_t pcnt_config = {};  
    pcnt_config.pulse_gpio_num = encoder->PIN_A; // Pin de la señal A (pulsos)
    pcnt_config.ctrl_gpio_num = encoder->PIN_B;  // Pin de la señal B (dirección)
    pcnt_config.unit = encoder->PCNT_UNIT;
    pcnt_config.channel = PCNT_CHANNEL_0;
    pcnt_config.pos_mode = PCNT_COUNT_INC;  // Incrementar si A cambia con B = LOW
    pcnt_config.neg_mode = PCNT_COUNT_DEC;  // Decrementar si A cambia con B = HIGH
    pcnt_config.lctrl_mode = PCNT_MODE_KEEP; // Mantener la dirección si B = LOW
    pcnt_config.hctrl_mode = PCNT_MODE_REVERSE; // Invertir la dirección si B = HIGH
    pcnt_config.counter_h_lim = 32767;  // Límite superior del contador
    pcnt_config.counter_l_lim = -32767; // Límite inferior del contador
    pcnt_unit_config(&pcnt_config);

    pcnt_counter_pause(encoder->PCNT_UNIT);
    pcnt_counter_clear(encoder->PCNT_UNIT);
    pcnt_counter_resume(encoder->PCNT_UNIT);
}
