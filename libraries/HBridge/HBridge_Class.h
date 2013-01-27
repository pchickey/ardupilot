
#ifndef __HBRIDGE_CLASS_H__
#define __HBRIDGE_CLASS_H__

/** 
 * dual h-bridge abstraction, for robot motor control 
 * pat hickey, 27 Jan 2013 
 */

class HBridge {
public:

    /* initializes the underlying hardware */
    virtual void init() = 0;

    /* left and right channels are floats between -1.0 to +1.0.
     * -1.0 for full reverse, 0.0 for off, +1.0 for full forwards. */
    virtual void write(float left, float right) = 0;

    /* set both outputs enabled or disabled. disabled by default after init(). */
    virtual void output_enabled(bool enabled) = 0;

    /* set pwm frequency. do this only when output disabled! */
    virtual void set_freq(uint16_t freq_hz) = 0;
};

#endif // __HBRIDGE_CLASS_H__

