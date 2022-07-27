#ifndef TYPES_H
#define TYPES_H

typedef	signed char			int8_t;
typedef	signed short		int16_t;
typedef	signed long			int32_t;

typedef	unsigned char		uint8_t;
typedef	unsigned short	uint16_t;
typedef	unsigned long		uint32_t;


typedef enum{  FALSE=0,TRUE=1} BOOL;

#ifndef NULL
#define NULL     (void*)(0)
#endif

#define		SV(n)			(uint8_t)(1<<n)

#endif
