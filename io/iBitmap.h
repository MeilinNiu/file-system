#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
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