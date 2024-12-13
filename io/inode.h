#ifndef INODE_H
#define INODE_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include "superblock.h"

/*
Inode (index node) structure:
size: Size of the file in bytes
mode: 16bit value to indicate the permissions
uid: 16bit value to indicate the user id of the owner
atime: 32bit value representing the number of seconds since january 1st 1970 of the last time this inode was accessed.
ctime: 32bit value representing the number of seconds since january 1st 1970 of the last time this inode was created.
mtime: 32bit value representing the number of seconds since january 1st 1970 of the last time this inode was modified.
direct_blocks[8]: Direct pointers to file data blocks
indirect_block: Pointer to a block of single indirect pointers
double_indirect_block: Pointer to a block of double indirect pointers
triple_indirect_block: Pointer to a block of triple indirect pointers

size of Inode is 64 bytes
*/
typedef struct {
    uint32_t size;    
    uint16_t mode; 
    uint16_t uid;
    uint32_t atime;       
    uint32_t ctime;
    uint32_t mtime;
    uint32_t direct_blocks[NUM_DIRECT_BLOCKS]; 
    uint32_t indirect_block;    
    uint32_t double_indirect_block; 
    uint32_t triple_indirect_block;
} Inode;

/*
Inode Table structure
    inodes: Array of inodes

size of InodeTable is NUM_INODES*sizeof(Inode) = 8192 bytes, taking up 16 blocks (512 bytes per block)
*/
typedef struct {
    Inode inodes[NUM_INODES];
} InodeTable;

/*
Directory Entry structure
    inode_number: The inode number of the file
    filename: The name of the file

size of DirectoryEntry is 32 bytes
*/
typedef struct {
    uint8_t inode_number;
    char filename[MAX_LEN_FILENAME];  // Null-terminated string
} DirectoryEntry;

void init_inode(FILE *disk, uint inode_number);
void read_inode(FILE *disk, uint inode_number, Inode *inode);
void write_inode(FILE *disk, uint inode_number, Inode *inode);
void delete_inode(FILE *disk, uint inode_number);

#endif