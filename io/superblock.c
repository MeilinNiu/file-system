#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "freeblock.c"

#define DISK_SIZE 4096  // Total number of blocks on the disk
#define BLOCK_SIZE 512  // Size of each block (in bytes)
#define MAGIC_NUMBER 0xDEADBEEF  // Example magic number

typedef struct {
    uint32_t magic_number;
    uint32_t block_count;
    uint32_t inode_count;
} Superblock;


// Function to initialize the superblock
void init_superblock(FILE *disk) {
    Superblock superblock;
    superblock.magic_number = MAGIC_NUMBER;
    superblock.block_count = DISK_SIZE;
    superblock.inode_count = 128;  // Assume we have 128 inodes

    fseek(disk, 0, SEEK_SET);
    fwrite(&superblock, sizeof(Superblock), 1, disk);  // Write superblock to disk
}