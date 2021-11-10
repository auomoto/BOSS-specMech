#include "globals.h"
#include "timers.h"
#include "roboclaw.h"
#include "usart.h"

USARTBuf send0_buf, send1_buf, send3_buf, recv0_buf, recv1_buf, recv3_buf;

/*------------------------------------------------------------------------------
void init_USART(void)
	Set up three serial USART ports (USART0, USART1, and USART3). The USART2
	pins are used for the 32.768 kHz crystal oscillator so that port is not
	available.

	USART0 is connected to the Lantronix XPort EtherNet transceiver.
	USART1 is connected to the three RoboClaw motor controllers.
	USART3 is connected to the liquid nitrogen controller.

	Check the pin positions for alternate USART connections (set the
	PORTMUX.USAROUTEA register to use alternates). We don't use alternates
	here.
------------------------------------------------------------------------------*/
void init_USART(void)
{

	// USART0 PA0 is TxD, PA1 is RxD, Default pin position
	PORTA.OUTSET = PIN0_bm;
	PORTA.DIRSET = PIN0_bm;
//	USART0.BAUD = (uint16_t) USART_BAUD_RATE(9600);
	USART0.BAUD = (uint16_t) USART_BAUD_RATE(115200);
	USART0.CTRLA |= USART_RXCIE_bm;		// Enable receive complete interrupt
	USART0.CTRLB |= USART_TXEN_bm;		// Enable USART transmitter
	USART0.CTRLB |= USART_RXEN_bm;		// Enable USART receiver
	send0_buf.head = 0;					// Set up send/receive buffers
	send0_buf.tail = 0;
	send0_buf.done = YES;
	recv0_buf.head = 0;
	recv0_buf.tail = 0;
	recv0_buf.data[0] = '\0';
	recv0_buf.length = 0;
	recv0_buf.done = NO;

	// USART1 PC0 is TxD, PC1 is RxD
	PORTC.OUTSET = PIN0_bm;
	PORTC.DIRSET = PIN0_bm;
	USART1.BAUD = (uint16_t) USART_BAUD_RATE(38400);
	USART1.CTRLA |= USART_RXCIE_bm;		// Enable receive complete interrupt
	USART1.CTRLB |= USART_TXEN_bm;		// Enable USART transmitter
	USART1.CTRLB |= USART_RXEN_bm;		// Enable USART receiver
	send1_buf.head = 0;
	send1_buf.tail = 0;
	send1_buf.done = YES;
	recv1_buf.head = 0;
	recv1_buf.tail = 0;
	recv1_buf.data[0] = '\0';
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
		port: The USARTn port (0, 1, or 3)
		data: The data to send
		nbytes: Number of bytes to send

	Output:
		None

	Returns:
		Nothing

	How it works:
		This copies the data array into the send0_buf buffer and the enables
		"transmit data register empty" interrupt (DREIE). The USARTn_DRE_vect
		puts the bytes into the transmit register until the tail catches up
		to the head of the circular buffer.
------------------------------------------------------------------------------*/
void send_USART(uint8_t port, uint8_t *data, uint8_t nbytes)
{

	uint8_t i;
	uint16_t crc;

	switch (port) {
		case 0:
			send0_buf.done = NO;
			for (i = 0; i < nbytes; i++) {
				send0_buf.data[send0_buf.head] = *data++;
				send0_buf.head = (send0_buf.head + 1) % BUFSIZE;
			}
			USART0.CTRLA |= USART_DREIE_bm;		// Enable interrupts
			start_TCB0(10);						// 10 ms ticks
			while (send0_buf.done == NO) {
				if (ticks > 100) {				// 1 second enough?
					stop_TCB0();
					send0_buf.done = YES;
					return;
				}
			}
			stop_TCB0();
			break;

		case 1:			// Timeouts are handled in caller routines
			crc = crc16(data, nbytes);
			for (i = 0; i < nbytes; i++) {
				send1_buf.data[send1_buf.head] = *data++;
				send1_buf.head = (send1_buf.head + 1) % BUFSIZE;
			}
			send1_buf.data[send1_buf.head] = (crc >> 8);
			send1_buf.head = (send1_buf.head + 1) % BUFSIZE;
			send1_buf.data[send1_buf.head] = (crc & 0xFF);
			send1_buf.head = (send1_buf.head + 1) % BUFSIZE;
			send1_buf.nbytes = nbytes + 2;
			send1_buf.nxfrd = 0;
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

/*------------------------------------------------------------------------------
ISR(USART0_RXC_vect)
	A byte at USART0 has been received. This is the channel to the high level
	control program coming in through the EtherNET port.

	If the character received is not a <CR> ('\r') then the nxfrd (number of
	bytes transferred) is incremented.

	If the character received is a <CR> ('\r'), a string terminator ('\0') is
	inserted into the buffer instead of the '\r'. The done flag is set and the
	nxfrd value set back to 0.
------------------------------------------------------------------------------*/
ISR(USART0_RXC_vect)
{

	uint8_t c;

	if (recv0_buf.length < BUFSIZE) {
		c = USART0.RXDATAL;
		if ((char) c == '\r') {
			recv0_buf.done = YES;
			recv0_buf.data[recv0_buf.head] = '\0';
		} else {
			recv0_buf.data[recv0_buf.head] = c;
		}
		recv0_buf.length++;
		recv0_buf.head = (recv0_buf.head + 1) % BUFSIZE;
	}
}

/*------------------------------------------------------------------------------
ISR(USART0_DRE_vect)
	Transmit data register empty interrupt. When the transmit data register
	(USART0.TXDATAL) is empty and the interrupt is enabled, you end up here.
	
	Here, we send out another byte of data and then compare the number of
	bytes already transferred (nxfrd) with the number requested (nbytes).
	If nxfrd == nbytes, then we set the send0_buf.done flag to YES and turn
	off this interrupt.

	Sending is started by calling send_USART(port).
------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------
ISR(USART1_RXC_vect)
	A byte at USART1 has been received.

------------------------------------------------------------------------------*/
ISR(USART1_RXC_vect)
{

	uint8_t c;

	c = USART1.RXDATAL;

	if (recv1_buf.nxfrd < recv1_buf.nbytes) {
		recv1_buf.data[recv1_buf.nxfrd++] = c;
	}

	if (recv1_buf.nxfrd >= recv1_buf.nbytes) {
		recv1_buf.done = YES;
	}

}

/*------------------------------------------------------------------------------
ISR(USART1_DRE_vect)
	Transmit data register empty interrupt. When the transmit data register
	(USART1.TXDATAL) is empty and the interrupt is enabled, you end up here.
	
	Here, we send out another byte of data and then compare the number of
	bytes already transferred (nxfrd) with the number requested (nbytes).
	If nxfrd == nbytes, then we set the send0_buf.done flag to YES and turn
	off this interrupt.

	Sending is started by calling send_USART(port).
------------------------------------------------------------------------------*/
ISR(USART1_DRE_vect)
{

	USART1.CTRLA &= ~USART_DREIE_bm;	// Turn off interrupts
	if (send1_buf.head - send1_buf.tail) {
		USART1.TXDATAL = send1_buf.data[send1_buf.tail++];
		send1_buf.tail %= BUFSIZE;
		USART1.CTRLA |= USART_DREIE_bm;		// Turn on interrupts
	} else {
		send1_buf.done = YES;
	}

}

/*------------------------------------------------------------------------------
ISR(USART3_RXC_vect)
	A byte at USART3 has been received.

CHANGE AS NEEDED:
	If the character received is not a <CR> ('\r') then the nxfrd (number of
	bytes transferred) is incremented.

	If the character received is a <CR> ('\r'), a string terminator ('\0') is
	inserted into the buffer instead of the '\r'. The done flag is set and the
	nxfrd value set back to 0.
------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------
ISR(USART3_DRE_vect)
	Transmit data register empty interrupt. When the transmit data register
	(USART3.TXDATAL) is empty and the interrupt is enabled, you end up here.
	
	Here, we send out another byte of data and then compare the number of
	bytes already transferred (nxfrd) with the number requested (nbytes).
	If nxfrd == nbytes, then we set the send0_buf.done flag to YES and turn
	off this interrupt.

	Sending is started by calling send_USART(port).
------------------------------------------------------------------------------*/
ISR(USART3_DRE_vect)
{

	USART3.TXDATAL = send3_buf.data[send3_buf.nxfrd++];
	if (send3_buf.nxfrd >= send3_buf.nbytes) {
		USART3.CTRLA &= ~USART_DREIE_bm;	// Turn off interrupts
		send3_buf.done = YES;
	}

}
