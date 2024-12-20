#include "inode.h"

// Function to initialize an inode
void init_inode(FILE *disk, uint inode_number) {
    // Create a new Inode structure and initialize to zero
    Inode inode;
    memset(&inode, 0, sizeof(Inode));

    // Initialize the features of the inode
    inode.size = 0;
    inode.mode = 0;
    inode.uid = 0;
    inode.atime = 0;
    // Set the current creation time to the ctime,
    // representing the number of seconds since january 1st 1970 of the last time this inode was created.
    inode.ctime = (uint32_t)time(NULL);
    inode.mtime = 0;
    inode.atime = 0;


    // Set the position in the disk for the inode based on the inode_number
    fseek(disk, START_INODE_BLOCK * sizeof(Inode) + inode_number * sizeof(Inode), SEEK_SET);
    fwrite(&inode, sizeof(Inode), 1, disk);  // Write the inode to the disk
}



// Function to read an inode from the disk
void read_inode(FILE *disk, uint inode_number) {
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

// Function to delete an inode by its inode number
void delete_inode(FILE *disk, uint inode_number) {

}

