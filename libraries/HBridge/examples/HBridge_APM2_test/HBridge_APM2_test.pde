
#include <AP_Common.h>
#include <AP_Math.h>
#include <AP_Param.h>
#include <AP_Progmem.h>
#include <HBridge.h>

#include <AP_HAL.h>
#include <AP_HAL_AVR.h>

#if CONFIG_HAL_BOARD == HAL_BOARD_APM2
const AP_HAL::HAL& hal = AP_HAL_AVR_APM2;
#else
#error HBridge_APM2_test supports APM2 only!
#endif


AP_HAL::DigitalSource *a_led;
AP_HAL::DigitalSource *b_led;
AP_HAL::DigitalSource *c_led;

HBridge *hbridge;

void loop (void) {
    hal.scheduler->delay(1000);
    c_led->write(1);

    hbridge->write( -0.5f, 0.5f );
    
    hal.scheduler->delay(1000);
    c_led->write(0);
    
    hbridge->write( 0.0f, 0.0f );
    
    hal.scheduler->delay(1000);
    c_led->write(1);

    hbridge->write( 0.75f, -0.25f );
    
    hal.scheduler->delay(1000);
    c_led->write(0);
    
    hbridge->write( 0.0f, 0.0f );
}

void setup (void) {
    a_led = hal.gpio->channel(27);
    b_led = hal.gpio->channel(26);
    c_led = hal.gpio->channel(25);

    a_led->mode(GPIO_OUTPUT);
    b_led->mode(GPIO_OUTPUT);
    c_led->mode(GPIO_OUTPUT);

    a_led->write(0);
    b_led->write(0);
    c_led->write(0);

    hbridge = new HBridge_APM(new HBridge_APMHW_APM2, a_led, b_led, 0.01);
    hbridge->set_freq(2000);
    hbridge->write(0.0f, 0.0f);
    hbridge->output_enabled(true);
}

AP_HAL_MAIN();
