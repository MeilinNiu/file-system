#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include "superblock.h"

#define MAX_NUM_BLOCKS_HEX 1026 // 4096 number of 1 in decimal has 1026 character in hex

/*
dBitmap (data bitmap) structure:
    block: Array of data block bit (0 or 1) 
        to represent whether each data block is free (0) or allocated (1).
    MAX_NUM_BLOCKS is the number of data blocks in the disk
    change uint8_t dBit[MAX_NUM_BLOCKS] to an array of hexidecimal number
*/

typedef struct{
    char dBit[MAX_NUM_BLOCKS_HEX];
} dBitmap;