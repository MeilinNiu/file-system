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
typedef struct {
    uint8_t iBit[NUM_INODES];
} iBitmap;

/*
dBitmap (data bitmap) structure:
    block: Array of data block bit (0 or 1) 
        to represent whether each data block is free (0) or allocated (1).
    MAX_NUM_BLOCKS is the number of data blocks in the disk

    In this case, by calculating (NUM_INODES / BLOCK_SIZE + 1) and (sizeof(Superblock) / BLOCK_SIZE + 1),
    I can know Superblock and iBitmap are stored in 2 separate blocks. 
    InodeTable will take up 16 blocks (NUM_INODES * sizeof(Inode) / BLOCK_SIZE + 1 = 128 * 64 / 512 + 1 = 16).
    Hence, there are 4094 blocks left for data block bitmap and data blocks.

    Therefore, data block bitmap will take up nearly 4078/512 + 1 = 8 blocks.
    Here I assume dBit has length of MAX_NUM_BLOCKS for simplicity.

*/

typedef struct {
    char dBit[MAX_NUM_BLOCKS];
} dBitmap;

/*
The function to find an available inode in the inode bitmap
    disk: The file pointer to the disk
    return: The inode number of the available inode
*/
int find_available_inode(FILE *disk);