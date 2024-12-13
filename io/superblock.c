#include "superblock.h"
#include "bitmap.h"
#include "inode.h"

// Function to initialize the superblock
void init_superblock(FILE *disk) {
    Superblock superblock;
    superblock.block_count = MAX_NUM_BLOCKS;
    superblock.inode_count = NUM_INODES;  
    superblock.dBitmap_start = sizeof(Superblock) / BLOCK_SIZE + 1;
    superblock.iBitmap_start = (sizeof(Superblock) / BLOCK_SIZE + 1) + (sizeof(dBitmap) / BLOCK_SIZE + 1);
    // Calculate inode table starts at from which block, this block is not occupied by dBitmap
    // Superblock + dBitmap + iBitmap 
    superblock.inode_start = (sizeof(Superblock) / BLOCK_SIZE + 1) + (sizeof(dBitmap) / BLOCK_SIZE + 1) + (sizeof(iBitmap) / BLOCK_SIZE + 1);  
    superblock.data_start = superblock.inode_start + (NUM_INODES * sizeof(Inode) / BLOCK_SIZE + 1);
    
    // Moves the file pointer to the beginning of the disk
    fseek(disk, 0, SEEK_SET);
    // Write the superblock to the disk
    fwrite(&superblock, sizeof(Superblock), 1, disk);  
}