#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "inode.c"
#include "superblock.c"

#define MAX_FILES 128  // Maximum number of open files

// Structure to keep track of open files
typedef struct {
    uint32_t inode_number;
    uint32_t current_position;  // Position in the file for reading/writing
} OpenFile;

OpenFile open_files[MAX_FILES];  // Array to store open file information

// Function to open a file for reading or writing
int open_file(const char *filename, int mode) {
    // Search for the file in the root directory (this can be expanded later for other directories)
    FILE *disk = fopen("vdisk", "r+b");
    if (!disk) {
        printf("Error: Unable to open disk file.\n");
        return -1;
    }

    // Search the directory for the file (simplified for this example)
    DirectoryEntry entry;
    Inode inode;
    int inode_number = -1;

    // Iterate through the directory to find the file's inode
    // Assume the file exists and its inode is at a fixed index
    inode_number = 1;  // Example: file inode number 1 for simplicity
    read_inode(disk, inode_number, &inode);

    // Open the file by storing its inode number and setting the current position
    for (int i = 0; i < MAX_FILES; i++) {
        if (open_files[i].inode_number == 0) {  // Find an empty slot
            open_files[i].inode_number = inode_number;
            open_files[i].current_position = 0;  // Start at the beginning of the file
            fclose(disk);
            return i;  // Return the file descriptor (index in the open_files array)
        }
    }

    fclose(disk);
    return -1;  // No free file descriptors available
}

// Function to create a new file
int create_file(const char *filename) {
    FILE *disk = fopen("vdisk", "r+b");
    if (!disk) {
        printf("Error: Unable to open disk file.\n");
        return -1;
    }

    // Allocate a new inode for the file
    int inode_number = 1;  // For simplicity, assume inode number 1 for the new file
    Inode inode;
    memset(&inode, 0, sizeof(Inode));  // Initialize the inode to zero
    inode.flags = 1;  // Mark as a regular file
    inode.size = 0;  // Initially, the file size is 0

    // Write the inode to the disk
    write_inode(disk, inode_number, &inode);

    // Create a new directory entry for the file
    DirectoryEntry entry;
    entry.inode_number = inode_number;
    strncpy(entry.filename, filename, sizeof(entry.filename) - 1);  // Store the filename

    // Write the directory entry to the directory block (simplified)
    // Assume the directory is already set up.
    //

    fclose(disk);
    return inode_number;  // Return the inode number of the newly created file
}

// Function to write data to a file
int write_file(int fd, void *data, size_t size) {
    if (fd < 0 || fd >= MAX_FILES || open_files[fd].inode_number == 0) {
        printf("Error: Invalid file descriptor.\n");
        return -1;
    }

    FILE *disk = fopen("vdisk", "r+b");
    if (!disk) {
        printf("Error: Unable to open disk file.\n");
        return -1;
    }

    uint32_t inode_number = open_files[fd].inode_number;
    Inode inode;
    read_inode(disk, inode_number, &inode);

    // Write data to the file, starting at the current position
    uint32_t block_number = open_files[fd].current_position / BLOCK_SIZE;
    uint32_t offset = open_files[fd].current_position % BLOCK_SIZE;

    // Assume writing to the first block for simplicity
    write_block(disk, inode.blocks[block_number], data);

    // Update the inode with the new file size and write it back to disk
    inode.size += size;
    write_inode(disk, inode_number, &inode);

    // Update the file's current position
    open_files[fd].current_position += size;

    fclose(disk);
    return size;  // Return the number of bytes written
}

// Function to read data from a file
int read_file(int fd, void *buffer, size_t size) {
    if (fd < 0 || fd >= MAX_FILES || open_files[fd].inode_number == 0) {
        printf("Error: Invalid file descriptor.\n");
        return -1;
    }

    FILE *disk = fopen("vdisk", "r+b");
    if (!disk) {
        printf("Error: Unable to open disk file.\n");
        return -1;
    }

    uint32_t inode_number = open_files[fd].inode_number;
    Inode inode;
    read_inode(disk, inode_number, &inode);

    // Read data from the file, starting at the current position
    uint32_t block_number = open_files[fd].current_position / BLOCK_SIZE;
    uint32_t offset = open_files[fd].current_position % BLOCK_SIZE;

    // Assume reading from the first block for simplicity
    read_block(disk, inode.blocks[block_number], buffer);

    // Update the file's current position
    open_files[fd].current_position += size;

    fclose(disk);
    return size;  // Return the number of bytes read
}

// Function to delete a file
int delete_file(const char *filename) {
    FILE *disk = fopen("vdisk", "r+b");
    if (!disk) {
        printf("Error: Unable to open disk file.\n");
        return -1;
    }

    // Search for the file's inode number (simplified for this example)
    int inode_number = 1;  // Example: file inode number 1 for simplicity

    // Free the blocks associated with the inode
    Inode inode;
    read_inode(disk, inode_number, &inode);

    // Free the blocks (simplified here for example)
    for (int i = 0; i < 10; i++) {
        if (inode.blocks[i] != 0) {
            free_block(disk, inode.blocks[i]);  // Free each block
        }
    }

    // Free the inode
    write_inode(disk, inode_number, &inode);  // Reset the inode (mark it free)

    // Remove the directory entry for the file (simplified here)
    fclose(disk);
    return 0;  // Success
}