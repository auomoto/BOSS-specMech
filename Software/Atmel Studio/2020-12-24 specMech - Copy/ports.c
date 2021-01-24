#ifndef PORTSC
#define PORTSC
#endif

// Function Prototypes
void init_PORTS(void);

/*------------------------------------------------------------------------------
void init_PORTS(void)
	Sets all I/O ports to be inputs with pullups enabled except ADC ports,
	which have their inputs disabled. This is just to make sure nothing lurks
	in the background by accident.

------------------------------------------------------------------------------*/
void init_PORTS(void)
{

	PORTA.PIN0CTRL = PORT_PULLUPEN_bm;
	PORTA.PIN1CTRL = PORT_PULLUPEN_bm;
	PORTA.PIN2CTRL = PORT_PULLUPEN_bm;
	PORTA.PIN3CTRL = PORT_PULLUPEN_bm;
	PORTA.PIN4CTRL = PORT_PULLUPEN_bm;
	PORTA.PIN5CTRL = PORT_PULLUPEN_bm;
	PORTA.PIN6CTRL = PORT_PULLUPEN_bm;
	PORTA.PIN7CTRL = PORT_PULLUPEN_bm;

	PORTB.PIN0CTRL = PORT_PULLUPEN_bm;
	PORTB.PIN1CTRL = PORT_PULLUPEN_bm;
	PORTB.PIN2CTRL = PORT_PULLUPEN_bm;
	PORTB.PIN3CTRL = PORT_PULLUPEN_bm;
	PORTB.PIN4CTRL = PORT_PULLUPEN_bm;
	PORTB.PIN5CTRL = PORT_PULLUPEN_bm;

	PORTC.PIN0CTRL = PORT_PULLUPEN_bm;
	PORTC.PIN1CTRL = PORT_PULLUPEN_bm;
	PORTC.PIN2CTRL = PORT_PULLUPEN_bm;
	PORTC.PIN3CTRL = PORT_PULLUPEN_bm;
	PORTC.PIN4CTRL = PORT_PULLUPEN_bm;
	PORTC.PIN5CTRL = PORT_PULLUPEN_bm;
	PORTC.PIN6CTRL = PORT_PULLUPEN_bm;
	PORTC.PIN7CTRL = PORT_PULLUPEN_bm;

	// ADC pins are on port D
	PORTD.PIN0CTRL = PORT_ISC_INPUT_DISABLE_gc;
	PORTD.PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc;
	PORTD.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;
	PORTD.PIN3CTRL = PORT_ISC_INPUT_DISABLE_gc;
	PORTD.PIN4CTRL = PORT_ISC_INPUT_DISABLE_gc;
	PORTD.PIN5CTRL = PORT_ISC_INPUT_DISABLE_gc;
	PORTD.PIN6CTRL = PORT_ISC_INPUT_DISABLE_gc;
	PORTD.PIN7CTRL = PORT_ISC_INPUT_DISABLE_gc;

	PORTE.PIN0CTRL = PORT_PULLUPEN_bm;
	PORTE.PIN1CTRL = PORT_PULLUPEN_bm;
	PORTE.PIN2CTRL = PORT_PULLUPEN_bm;
	PORTE.PIN3CTRL = PORT_PULLUPEN_bm;

	PORTF.PIN0CTRL = PORT_PULLUPEN_bm;
	PORTF.PIN1CTRL = PORT_PULLUPEN_bm;
	PORTF.PIN2CTRL = PORT_PULLUPEN_bm;
	PORTF.PIN3CTRL = PORT_PULLUPEN_bm;
	PORTF.PIN4CTRL = PORT_PULLUPEN_bm;
	PORTF.PIN5CTRL = PORT_PULLUPEN_bm;
	PORTF.PIN6CTRL = PORT_PULLUPEN_bm;

}