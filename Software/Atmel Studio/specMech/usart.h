#ifndef USARTH
#define USARTH

#define BUFSIZE 254
#define	USART_BAUD_RATE(BAUD_RATE)	((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)

typedef struct {
	uint8_t				// Serial I/O buffer
		data[BUFSIZE],		// Data to send or data received
		head,				// Ring buffer head index
		tail,				// Ring buffer tail index
		nbytes,				// Number of data bytes in data[];
		length,				// Number of unread bytes in the FIFO
		nxfrd;				// Temporary counter (number of bytes transferred)
	uint8_t	volatile done;	// Is the transfer complete ('\r' seen)?
} USARTBuf;

typedef struct {
	uint8_t data[BUFSIZE], n2xfr, nxfrd;
} USART1Buf;

extern USARTBuf
	send0_buf, send3_buf,
	recv0_buf, recv3_buf;

extern USART1Buf
	ser_send1, ser_recv1;

void init_USART(void);
void send_USART(uint8_t, uint8_t*, uint8_t);
void send_USART1(uint8_t*, uint8_t);

#endif