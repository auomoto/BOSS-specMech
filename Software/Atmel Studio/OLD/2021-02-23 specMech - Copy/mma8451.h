#ifndef MMA8451H
#define MMA8451H

#define MMA8451ADDR			(0x1D)	// Two are allowed on the TWI bus
#define MMA8451OUTXMSB		(0x01)	// Start address of output data
#define MMA8451WHOAMI		(0x0D)	// MMA8451 WHO_AM_I (0x1A is the answer)
#define MMA8451HFCUTOFF		(0x0F)	// MMA8451 HP_FILTER_CUTOFF
#define MMA8451CTRLREG1		(0x2A)	// MMA8451 CTRL_REG1
#define MMA8451CTRLREG2		(0x2B)	// MMA8451 CTRL_REG2
uint8_t init_MMA8451(void);
uint8_t read_MMA8451(uint8_t, uint8_t, uint8_t*, uint8_t);
uint8_t write_MMA8451(uint8_t, uint8_t, uint8_t);

#endif