#include "inode.h"

// Function to initialize an inode
void init_inode(FILE *disk, uint inode_number) {
    // Create a new Inode structure and initialize to zero
    Inode inode;
    memset(&inode, 0, sizeof(Inode));

    // Assign the inode number (we might add more initialization logic later)
    inode.size = 0;

    // Set the position in the disk for the inode based on the inode_number
    fseek(disk, START_INODE_BLOCK * sizeof(Inode) + inode_number * sizeof(Inode), SEEK_SET);
    fwrite(&inode, sizeof(Inode), 1, disk);  // Write the inode to the disk
}

// Function to read an inode from the disk
void read_inode(FILE *disk, uint inode_number, Inode *inode) {
    // Calculate the correct position in the disk for the given inode_number
    fseek(disk, START_INODE_BLOCK * sizeof(Inode) + inode_number * sizeof(Inode), SEEK_SET);

    // Read the inode data into the provided inode structure
    fread(inode, sizeof(Inode), 1, disk);
}

// Function to write an inode to the disk
void write_inode(FILE *disk, uint inode_number, Inode *inode) {
    // Set the correct position in the disk for the inode
    fseek(disk, START_INODE_BLOCK * sizeof(Inode) + inode_number * sizeof(Inode), SEEK_SET);

    // Write the updated inode structure to the disk
    fwrite(inode, sizeof(Inode), 1, disk);
}