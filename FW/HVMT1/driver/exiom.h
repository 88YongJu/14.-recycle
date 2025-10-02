#ifndef EXIOM_H_
#define EXIOM_H_

#include "types.h"

typedef union {
	uint8_t all;                 // The sfrb as 1 byte
	struct {                     // The sfrb as 8 bits
		uint8_t bit0:1,
				bit1:1,
				bit2:1,
				bit3:1,
				bit4:1,
				bit5:1,
				bit6:1,
				bit7:1;
	};
} IOREG8;

#define SFR_IO8(addr) 	(*(volatile IOREG8 *) (addr+0x20))
#define SFR_MEM8(addr) 	(*(volatile IOREG8 *) (addr))

#define ddra	(SFR_IO8(0x01))
#define ddrb	(SFR_IO8(0x04))
#define ddrc	(SFR_IO8(0x07))
#define ddrd	(SFR_IO8(0x0A))
#define ddre	(SFR_IO8(0x0D))
#define ddrf	(SFR_IO8(0x10))
#define ddrg	(SFR_IO8(0x13))

#define porta	(SFR_IO8(0x02))
#define portb	(SFR_IO8(0x05))
#define portc	(SFR_IO8(0x08))
#define portd	(SFR_IO8(0x0B))
#define porte	(SFR_IO8(0x0E))
#define portf	(SFR_IO8(0x11))
#define portg	(SFR_IO8(0x14))

#define pina	(SFR_IO8(0x00))
#define pinb	(SFR_IO8(0x03))
#define pinc	(SFR_IO8(0x06))
#define pind	(SFR_IO8(0x09))
#define pine	(SFR_IO8(0x0C))
#define pinf	(SFR_IO8(0x0F))
#define ping	(SFR_IO8(0x12))



#endif

