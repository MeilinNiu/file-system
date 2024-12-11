#include "superblock.h"

// Function to initialize the superblock
void init_superblock(FILE *disk) {
    Superblock superblock;
    superblock.magic_number = MAGIC_NUMBER;
    superblock.block_count = MAX_NUM_BLOCKS;
    superblock.inode_count = NUM_INODES;  
    superblock.inode_start = START_INODE_BLOCK;  // Assume inode table starts at block 3

    fseek(disk, 0, SEEK_SET);
    fwrite(&superblock, sizeof(Superblock), 1, disk);  // Write superblock to disk
}