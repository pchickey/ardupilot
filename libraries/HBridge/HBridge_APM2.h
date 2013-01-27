
#ifndef __HBRIDGE_APM2_H__
#define __HBRIDGE_APM2_H__

#include <AP_HAL.h>

#include "HBridge_Class.h"

/* dual h-bridge output. overrides APM2's rcoutput on CH_1 and CH_2, uses those to output
 * a high-frequency pwm for h-bridge control instead. */

class HBridge_APM2 : public HBridge {
public:
    /* constructor takes the pins for the direction select lines. */
    HBridge_APM2(AP_HAL::DigitalSource *left_dirselect,
                 AP_HAL::DigitalSource *right_dirselect,
                 float deadband) :
        _left_dirselect(left_dirselect),
        _right_dirselect(right_dirselect),
        _deadband(deadband)
    {}

    /* initializes the underlying hardware. After calling init, the hal's
     * rcoutput will be totally broken for channels 1 and 2, and if you make
     * hal calls to those channels, it'll break this code! so, use with care. */
    void init();

    /* left and right channels are floats between -1.0 to +1.0.
     * -1.0 for full reverse, 0.0 for off, +1.0 for full forwards. */
    void write(float left, float right);
   
    /* outputs will be high-z when disabled */
    void output_enabled(bool enabled);

    /* best to keep this below 20khz */
    void set_freq(uint16_t freq_hz);

private:
    uint16_t _calc_timer_period(uint16_t freq_hz);
    void _set_timer_period(uint16_t period);
    uint16_t _timer_period;

    /* throttle between 0.0f and 1.0f. */
    uint16_t _pwm_output_of_throttle(float throttle);

    void _write_pwm(uint16_t left, uint16_t right);

    AP_HAL::DigitalSource* _left_dirselect;
    AP_HAL::DigitalSource* _right_dirselect;
    uint16_t _freq;
    float _deadband;
    
};

#endif // __HBRIDGE_APM2_H__

