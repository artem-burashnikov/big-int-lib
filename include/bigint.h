#include <stdint.h>

#ifndef __BIGINT_H__
#define __BIGINT_H__

typedef enum
{
    POSITIVE = 0,
    NEGATIVE = 1
} Sign;

typedef struct
{
    Sign sign;
    uint64_t capacity;
    char *digits;
} BigInt;

#endif /* BIGINT_H_ */