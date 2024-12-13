#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "superblock.h"

/*
iBitmap (inode bitmap) structure:
    iBit: Array of inode bit (0 or 1) 
        to represent whether each inode is free (0) or allocated (1).
    NUM_INODES is the number of inodes in the disk
*/
typedef struct{
    uint8_t iBit[NUM_INODES];
} iBitmap;

/*
dBitmap (data bitmap) structure:
    block: Array of data block bit (0 or 1) 
        to represent whether each data block is free (0) or allocated (1).
    MAX_NUM_BLOCKS is the number of data blocks in the disk
*/

typedef struct{
    char dBit[MAX_NUM_BLOCKS - ];
} dBitmap;