#include "pico/stdlib.h"
#include "hardware_drive.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define OFF_CHANNEL 15

#define FPWM 20000

//////////////////////////////////////////////////////////////////////
///
/// Slice CHAN GPIO  Motor.PIN   WHEEL
///    0    A   16      1.2   Left Front
///         B   17      1.1
///    7    A   14      2.2   Right Front
///         B   15      2.1
///    6    A   12      3.2   Left Rear
///         B   13      3.1
///    5    A   10      4.2   Right Rear
///         B   11      4.1
///
///
///////////////////////////////////////////////////////////
typedef struct
{
	uint slice;
	int pinchana;
	int pinchanb;
	int is_inv;
	pwm_config pc;

} cs_mot_t;

static cs_mot_t cs_mot[] =
{
{ 0, 16, 17, false,
{ } },
{ 7, 14, 15, true,
{ } },
{ 6, 12, 13, false,
{ } },
{ 5, 10, 11, true,
{ } }, };

static void motor_init2(cs_mot_t *s)
{
	// Tell GPIO they are allocated to the PWM
	gpio_init(s->pinchana);
	gpio_init(s->pinchanb);
	gpio_set_function(s->pinchana, GPIO_FUNC_PWM);
	gpio_set_function(s->pinchanb, GPIO_FUNC_PWM);

	s->pc = pwm_get_default_config();
	pwm_config_set_clkdiv_mode(&s->pc, PWM_DIV_FREE_RUNNING);

	double d;
	{
		d = (double) clock_get_hz(clk_sys) / (double) FPWM;
		d /= (double) DRIVE_MAX;
	}
	pwm_config_set_clkdiv(&s->pc, d);

	s->pc.top = DRIVE_MAX - 1;

	pwm_init(s->slice, &s->pc, false);

	pwm_set_chan_level(s->slice, PWM_CHAN_A, OFF_CHANNEL);
	pwm_set_chan_level(s->slice, PWM_CHAN_B, OFF_CHANNEL);

	pwm_set_enabled(s->slice, true);
}

void motor_init(void)
{
	for (int i = FRONT_LEFT; i <= REAR_RIGHT; i++)
	{
		cs_mot_t *m = &cs_mot[i];
		motor_init2(m);
	}
}

void motor_set(const enum WHEEL w, int pwm_val)
{
	cs_mot_t *m = &cs_mot[w];
	if(m->is_inv) pwm_val = -pwm_val;
	if (pwm_val > 0)
	{
		pwm_set_chan_level(m->slice, PWM_CHAN_A, DRIVE_MAX);
		pwm_set_chan_level(m->slice, PWM_CHAN_B, DRIVE_MAX - pwm_val);
	}
	else
	{
		pwm_set_chan_level(m->slice, PWM_CHAN_A, DRIVE_MAX + pwm_val);
		pwm_set_chan_level(m->slice, PWM_CHAN_B, DRIVE_MAX);
	}
}
