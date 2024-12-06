#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define INODE_BLOCK 2  // starting at block 2 for inode storage
#define INODE_SIZE 32   // Size of each inode in bytes
#define MAX_BLOCKS 4096 // Total number of blocks on the disk
#define DIRECTORY_ENTRY_SIZE 32

typedef struct {
    uint32_t size;          // Size of the file in bytes
    uint32_t flags;         // File type (flat or directory)
    uint16_t blocks[10];    // First 10 blocks
    uint16_t indirect_block;
    uint16_t double_indirect_block;
} Inode;

typedef struct {
    uint32_t inode_number;
    char filename[31];  // Null-terminated string
} DirectoryEntry;

// Function to initialize an inode
void init_inode(FILE *disk, uint32_t inode_number) {
    // Create a new Inode structure and initialize to zero
    Inode inode;
    memset(&inode, 0, sizeof(Inode));

    // Assign the inode number (we might add more initialization logic later)
    inode.size = 0;
    inode.flags = 0;  // File type, set to 0 for an empty inode (no file type)

    // Set the position in the disk for the inode based on the inode_number
    fseek(disk, INODE_BLOCK * INODE_SIZE + inode_number * INODE_SIZE, SEEK_SET);
    fwrite(&inode, sizeof(Inode), 1, disk);  // Write the inode to the disk
}

// Function to read an inode from the disk
void read_inode(FILE *disk, uint32_t inode_number, Inode *inode) {
    // Calculate the correct position in the disk for the given inode_number
    fseek(disk, INODE_BLOCK * INODE_SIZE + inode_number * INODE_SIZE, SEEK_SET);

    // Read the inode data into the provided inode structure
    fread(inode, sizeof(Inode), 1, disk);
}

// Function to write an inode to the disk
void write_inode(FILE *disk, uint32_t inode_number, Inode *inode) {
    // Set the correct position in the disk for the inode
    fseek(disk, INODE_BLOCK * INODE_SIZE + inode_number * INODE_SIZE, SEEK_SET);

    // Write the updated inode structure to the disk
    fwrite(inode, sizeof(Inode), 1, disk);
}

