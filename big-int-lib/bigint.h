#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifndef __BIGINT_H__
#define __BIGINT_H__

typedef struct
{
    bool is_negative;
    size_t length;
    char *digits;
} BigInt;

/* Allocates the memory for the BigInt structure of the specified length. */
BigInt alloc_bigint(size_t length);

/* Converts from string to BigInt. */
BigInt strtobi(char *str);

/* Converts from BigInt to string */
char *bitostr(BigInt n);

#endif /* BIGINT_H_ */