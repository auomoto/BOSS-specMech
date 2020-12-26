/*------------------------------------------------------------------------------
usart.c
	Serial read/write routines.

	The #define computation for 9600 baud works for the 3.3MHz processor
	speed. Haven't checked for how fast you can go with a 3.33MHz processor.

	There are three data buffers for transmit and three for receive for a
	total of six I/O buffers. These are the USARTBuf structures defined here.

	BUFSIZE is the length of the I/O buffer. The sendX_buf structures are
	handled as ring buffers while the recvX_buf structures simply collect data
	until a <CR> is received. So commands to the instrument should not come
	too quickly or the buffer could be overwritten. It's probably a good idea
	to receive completely the response from the previous command before sending
	another.
------------------------------------------------------------------------------*/

#ifndef USARTC
#define USARTC

#define	USART_BAUD_RATE(BAUD_RATE)	((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)
#define BUFSIZE 128

// Function Prototypes
void send_USART(uint8_t, uint8_t*, uint8_t);
void init_USART(void);

typedef struct {
	uint8_t					// Serial I/O buffer
		data[BUFSIZE],		// Data to send or data received
		head,				// Ring buffer head index
		tail,				// Ring buffer tail index
		nbytes,				// Number of data bytes in data[];
		nxfrd;				// Temporary counter (number of bytes transferred)
	uint8_t	volatile done;	// Is the transfer complete ('\r' seen)?
} USARTBuf;

USARTBuf
	send0_buf, send1_buf, send3_buf,
	recv0_buf, recv1_buf, recv3_buf;

/*------------------------------------------------------------------------------
void init_USART(void)

	Set up three serial USART ports (USART0, USART1, and USART3) for 9600 baud,
	8N1. The USART2 pins are used for the 32.768 kHz crystal oscillator so that
	port is not available.

	Check the pin positions for alternate USART connections (set the
	PORTMUX.USAROUTEA register to use alternates).

------------------------------------------------------------------------------*/
void init_USART(void)
{

	// USART0 PA0 is TxD, PA1 is RxD, Default pin position
	PORTA.OUTSET = PIN0_bm;
	PORTA.DIRSET = PIN0_bm;
	USART0.BAUD = (uint16_t) USART_BAUD_RATE(9600);
	USART0.CTRLA |= USART_RXCIE_bm;		// Enable receive complete interrupt
	USART0.CTRLB |= USART_TXEN_bm;		// Enable USART transmitter
	USART0.CTRLB |= USART_RXEN_bm;		// Enable USART Receiver
	send0_buf.head = 0;
	send0_buf.tail = 0;
	send0_buf.done = YES;
	recv0_buf.head = 0;
	recv0_buf.tail = 0;
	recv0_buf.data[0] = '\0';
	recv0_buf.done = NO;

	// USART1 PC0 is TxD, PC1 is RxD
	PORTC.OUTSET = PIN0_bm;
	PORTC.DIRSET = PIN0_bm;
	USART1.BAUD = (uint16_t) USART_BAUD_RATE(9600);
	USART1.CTRLB |= USART_TXEN_bm;
	USART1.CTRLB |= USART_RXEN_bm;
	send1_buf.done = YES;
	recv1_buf.done = NO;

	// USART3 PB0 is TxD, PB1 is RxD
	PORTB.OUTSET = PIN0_bm;
	PORTB.DIRSET = PIN0_bm;
	USART3.BAUD = (uint16_t) USART_BAUD_RATE(9600);
	USART3.CTRLB |= USART_TXEN_bm;
	USART3.CTRLB |= USART_RXEN_bm;
	send3_buf.done = YES;
	recv3_buf.done = NO;

}

/*------------------------------------------------------------------------------
void send_USART(uint8_t port, uint8_t *data, uint8_t nbytes)

	Send data out a serial USART port.

	Input:
		port - The USARTn port (0, 1, or 3)
		data - Contains the data to send
		nbytes - Number of bytes to send

------------------------------------------------------------------------------*/
void send_USART(uint8_t port, uint8_t *data, uint8_t nbytes)
{

	uint8_t i;

	switch (port) {
		case 0:
			for (i = 0; i < nbytes; i++) {
				send0_buf.data[send0_buf.head] = *data++;
				send0_buf.head = (send0_buf.head + 1) % BUFSIZE;
			}
			USART0.CTRLA |= USART_DREIE_bm;		// Enable interrupts
			break;

		case 1:
			for (i = 0; i < nbytes; i++) {
				send1_buf.data[send1_buf.head] = *data++;
				send1_buf.head = (send1_buf.head + 1) % BUFSIZE;
			}
			USART1.CTRLA |= USART_DREIE_bm;		// Enable interrupts
			break;

		case 3:
			for (i = 0; i < nbytes; i++) {
				send3_buf.data[send3_buf.head] = *data++;
				send3_buf.head = (send3_buf.head + 1) % BUFSIZE;
			}
			USART3.CTRLA |= USART_DREIE_bm;		// Enable interrupts
			break;

		default:
			break;
	}

}

/*---------------------------------------------------------------------
ISR(USART0_RXC_vect)
	A byte at USART0 has been received. This is the channel to the
	high level control program coming in through the EtherNET port.

	If the character received is not a <CR> ('\r') then the nxfrd
	(number of bytes transferred) is incremented.

	If the character received is a <CR> ('\r'), a string terminator
	('\0') is inserted into the buffer instead of the '\r'. The done
	flag is set and the nxfrd value set back to 0.

---------------------------------------------------------------------*/
ISR(USART0_RXC_vect)
{

	uint8_t c;

	c = USART0.RXDATAL;
	if ((char) c == '\r') {
		recv0_buf.done = YES;
		recv0_buf.data[recv0_buf.head] = '\0';
	} else {
		recv0_buf.data[recv0_buf.head] = c;
	}
	recv0_buf.head = (recv0_buf.head + 1) % BUFSIZE;

/*
	if (((char) c == '\r') || (recv0_buf.nxfrd >= (BUFSIZE-1))) {
		recv0_buf.done = YES;
		recv0_buf.data[recv0_buf.nxfrd] = 0;	// String terminator
		recv0_buf.nbytes = recv0_buf.nxfrd;
		recv0_buf.nxfrd = 0;
		return;
	} else {
		recv0_buf.data[recv0_buf.nxfrd++] = c;
	}
*/
}

/*---------------------------------------------------------------------
ISR(USART0_DRE_vect)
	Transmit data register empty interrupt. When the transmit data
	register (USART0.TXDATAL) is empty and the interrupt is enabled,
	you end up here.
	
	Here, we send out another byte of data and then compare the number
	of bytes already transferred (nxfrd) with the number requested
	(nbytes). If nxfrd == nbytes, then we set the send0_buf.done flag
	to YES and turn off this interrupt.

	Sending is started by calling send_USART(port).

---------------------------------------------------------------------*/
ISR(USART0_DRE_vect)
{

	USART0.CTRLA &= ~USART_DREIE_bm;	// Turn off interrupts
	if (send0_buf.head - send0_buf.tail) {
		USART0.TXDATAL = send0_buf.data[send0_buf.tail++];
		send0_buf.tail %= BUFSIZE;
		USART0.CTRLA |= USART_DREIE_bm;		// Turn on interrupts
	} else {
		send0_buf.done = YES;
	}

}

/*---------------------------------------------------------------------
ISR(USART1_RXC_vect)
	A byte at USART1 has been received.

CHANGE AS NEEDED:
	If the character received is not a <CR> ('\r') then the nxfrd
	(number of bytes transferred) is incremented.

	If the character received is a <CR> ('\r'), a string terminator
	('\0') is inserted into the buffer instead of the '\r'. The done
	flag is set and the nxfrd value set back to 0.

---------------------------------------------------------------------*/
ISR(USART1_RXC_vect)
{

	uint8_t c;

	c = USART1.RXDATAL;
	if (((char) c == '\r') || (recv1_buf.nxfrd >= (BUFSIZE-1))) {
		recv1_buf.done = YES;
		recv1_buf.data[recv1_buf.nxfrd] = 0;	// String terminator
		recv1_buf.nbytes = recv1_buf.nxfrd;
		recv1_buf.nxfrd = 0;
		return;
	} else {
		recv1_buf.data[recv1_buf.nxfrd++] = c;
	}

}

/*---------------------------------------------------------------------
ISR(USART1_DRE_vect)
	Transmit data register empty interrupt. When the transmit data
	register (USART1.TXDATAL) is empty and the interrupt is enabled,
	you end up here.
	
	Here, we send out another byte of data and then compare the number
	of bytes already transferred (nxfrd) with the number requested
	(nbytes). If nxfrd == nbytes, then we set the send0_buf.done flag
	to YES and turn off this interrupt.

	Sending is started by calling send_USART(port).

---------------------------------------------------------------------*/
ISR(USART1_DRE_vect)
{

	USART1.TXDATAL = send1_buf.data[send1_buf.nxfrd++];
	if (send1_buf.nxfrd >= send1_buf.nbytes) {
		USART1.CTRLA &= ~USART_DREIE_bm;	// Turn off interrupts
		send1_buf.done = YES;
	}

}

/*---------------------------------------------------------------------
ISR(USART3_RXC_vect)
	A byte at USART3 has been received.

CHANGE AS NEEDED:
	If the character received is not a <CR> ('\r') then the nxfrd
	(number of bytes transferred) is incremented.

	If the character received is a <CR> ('\r'), a string terminator
	('\0') is inserted into the buffer instead of the '\r'. The done
	flag is set and the nxfrd value set back to 0.

---------------------------------------------------------------------*/
ISR(USART3_RXC_vect)
{

	uint8_t c;

	c = USART3.RXDATAL;
	if (((char) c == '\r') || (recv3_buf.nxfrd >= (BUFSIZE-1))) {
		recv3_buf.done = YES;
		recv3_buf.data[recv3_buf.nxfrd] = 0;	// String terminator
		recv3_buf.nbytes = recv3_buf.nxfrd;
		recv3_buf.nxfrd = 0;
		return;
	} else {
		recv3_buf.data[recv3_buf.nxfrd++] = c;
	}

}

/*---------------------------------------------------------------------
ISR(USART3_DRE_vect)
	Transmit data register empty interrupt. When the transmit data
	register (USART3.TXDATAL) is empty and the interrupt is enabled,
	you end up here.
	
	Here, we send out another byte of data and then compare the number
	of bytes already transferred (nxfrd) with the number requested
	(nbytes). If nxfrd == nbytes, then we set the send0_buf.done flag
	to YES and turn off this interrupt.

	Sending is started by calling send_USART(port).

---------------------------------------------------------------------*/
ISR(USART3_DRE_vect)
{

	USART3.TXDATAL = send3_buf.data[send3_buf.nxfrd++];
	if (send3_buf.nxfrd >= send3_buf.nbytes) {
		USART3.CTRLA &= ~USART_DREIE_bm;	// Turn off interrupts
		send3_buf.done = YES;
	}

}

#endif
