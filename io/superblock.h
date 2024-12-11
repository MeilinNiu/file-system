#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

#define MAGIC_NUMBER 47494638  // Example magic number for GIF

#define MAX_NUM_BLOCKS 4096 // Total number of blocks on the disk
#define BLOCK_SIZE 512  // Size of each block (in bytes)
#define NUM_INODES 128  // Assume we have 128 inodes
#define START_INODE_BLOCK 3  // starting at block 2 for inode storage (block 0: superblock, block 1: inode bitmap, block 2: data block bitmap)
#define NUM_DIRECT_BLOCKS 8  // Assume the number of direct pointers in the inode is 8
#define MAX_LEN_FILENAME 31  // Maximum length of a filename

/*
Superblock structure
1. magic_number: A unique number to identify the file system type
2. block_count: Total number of blocks on the disk
3. inode_count: Total number of inodes in the file system
4. inode_start: where the inode table begins (block number)
*/
typedef struct {
    uint magic_number;
    uint block_count; 
    uint inode_count;
    uint inode_start;
} Superblock;

void init_superblock(FILE *disk);