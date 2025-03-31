#include <DriverEncoder.h>

namespace DriverEncoder {
    namespace ENCODERS {
        //ENCODER FORWARD_LEFT = ENCODER(PCNT_UNIT_0,4,5,0.24f);
        ENCODER FORWARD = ENCODER(PCNT_UNIT_0,4,5,0.24f);
        ENCODER MIDDLE = ENCODER(PCNT_UNIT_1,21,22,0.24f);
        ENCODER BACKWARD = ENCODER(PCNT_UNIT_2,25,26,0.24f);
    }
}

void DriverEncoder::start()
{
    //configEncoder(&ENCODERS::FORWARD_LEFT);
    configEncoder(&ENCODERS::FORWARD);
    configEncoder(&ENCODERS::MIDDLE);
    configEncoder(&ENCODERS::BACKWARD);
}


float DriverEncoder::getVelocity(ENCODERS::ENCODER *encoder)
{
    // El encoder es capaz de leer la velocidad, devolverla y almacenarla
    // ENCODER DIF
    int16_t encoder_value;
    pcnt_get_counter_value(encoder->PCNT_UNIT, &encoder_value);
    int dif_encoder = (int)encoder_value - (int)encoder->previous_encoder_value;
    // TIME DIF
    uint32_t time_ = millis();
    uint32_t dif_time = time_ - encoder->previous_time;
    // Si fuera de la cota, ha dado un salto de vuelta, retornar la velocidad anterior por sencillez, y actualizar valores
    if(abs(dif_encoder) > 10000) {
        encoder->previous_encoder_value = encoder_value;
        encoder->previous_time = time_;
        return encoder->speed;
    }

    // Cota de seguridad de salto de vuelta, si dentro de la cota, leer velocidad
    float speed = ((float)dif_encoder / (float)dif_time) * encoder->encoder_factor;

    // Actualizar valores
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
