#include <AP_HAL.h>
#if CONFIG_HAL_BOARD == HAL_BOARD_APM2

#include <AP_Math.h>

#include "HBridge_APM2.h"

#include <avr/io.h>

extern const AP_HAL::HAL& hal;

void HBridge_APM2::init() {
    // --------------------- TIMER1: CH_1 and CH_2 -----------------------
    hal.gpio->pinMode(12,GPIO_OUTPUT); // CH_1 (PB6/OC1B)
    hal.gpio->pinMode(11,GPIO_OUTPUT); // CH_2 (PB5/OC1A)

    // WGM: 1 1 1 0. Clear Timer on Compare, TOP is ICR1.
    // CS11: prescale by 8 => 0.5us tick (2mhz)
    TCCR1A =((1<<WGM11));
    TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS11);
    OCR1A = 0xFFFF; // Init OCR registers to nil output signal
    OCR1B = 0xFFFF;

    set_freq(1000); // basic default?
}

void HBridge_APM2::write(float left, float right) {
    _left_dirselect->write( (left < 0) ? 0 : 1 );
    _right_dirselect->write( (right < 0) ? 0 : 1 );

    uint16_t left_pwm = _pwm_output_of_throttle(fabs(left));
    uint16_t right_pwm = _pwm_output_of_throttle(fabs(right));

    _write_pwm(left, right);
}

/* Output freq (1/period) control */
void HBridge_APM2::set_freq(uint16_t freq_hz) {
    uint16_t per = _calc_timer_period(freq_hz);
    _set_timer_period(per);
}

void HBridge_APM2::_set_timer_period(uint16_t per) {
    _timer_period = per;
    ICR1 = per;
}

/* Output active/highZ control */
void HBridge_APM2::output_enabled(bool on) {
    if (on) {
        TCCR1A |= (1<<COM1B1); // CH_1 : OC1B
        TCCR1A |= (1<<COM1A1); // CH_2 : OC1A
    } else {
        TCCR1A &= ~(1<<COM1B1); // CH_1 : OC1B
        TCCR1A &= ~(1<<COM1A1); // CH_2 : OC1A
    }
}

/* throttle between 0.0f and 1.0f.
 * output is zero when throttle below _deadband */
uint16_t HBridge_APM2::_pwm_output_of_throttle(float throttle) {
    if (throttle < _deadband) {
        return 0;
    }
    if (throttle > 1.0f) {
        return _timer_period;
    }
    return (throttle * _timer_period);
}

/* Output, either single channel or bulk array of channels */
void HBridge_APM2::_write_pwm(uint16_t left_pwm, uint16_t right_pwm) {
    OCR1B = left_pwm;// out1
    OCR1A = right_pwm;// out2
}

uint16_t HBridge_APM2::_calc_timer_period(uint16_t speed_hz) {
    /* input clock to pwm is fixed at 0.5us period (2MHz) */
    return 2000000UL / speed_hz;
}

#endif
