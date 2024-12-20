#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MAX_NUM_BLOCKS 4096 // Total number of blocks on the disk
#define BLOCK_SIZE 512  // Size of each block (in bytes)
#define NUM_INODES 128  // Assume we have 128 inodes
#define NUM_DIRECT_BLOCKS 10  // Assume the number of direct pointers in the inode is 8

#define START_INODE_BLOCK 10  // The block number where the inode table starts

/*
Superblock structure
1. magic_number: A unique number to identify the file system type
2. block_count: Total number of blocks on the disk
3. inode_count: Total number of inodes in the file system
4. inode_start: where the inode table begins (block number)
*/
typedef struct {
    uint32_t block_count; 
    uint32_t inode_count;
    uint32_t inode_start;
    uint32_t dBitmap_start;
    uint32_t iBitmap_start;
    uint32_t data_start;
} Superblock;


void init_superblock(FILE *disk);

#endif