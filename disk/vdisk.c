#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "superblock.h"
#include "inode.h"
#include "bitmap.h"

void create_file(FILE *disk, const char *file_name, uint16_t userId, uint16_t mode) {
    // Check the iBitmap to find an available inode
    int inode_number = find_available_inode(disk);
    if (inode_number == -1) {
        return;
}
    // Allocate an inode for the file
    InodeTable inodeTable;
    memset(&inode, 0, sizeof(Inode));
    inode.uid = userId;
    inode.mode = mode;
    inode.ctime = (uint32_t)time(NULL);

    // Update the iBitmap
    iBitmap.data[inode_number / 8] |= (1 << (inode_number % 8));
    fseek(disk, BLOCK_SIZE, SEEK_SET);
    fwrite(&iBitmap, sizeof(Bitmap), 1, disk);

    // Update the directory structure to include the file name and inode number
    // (Assuming a simple directory structure for demonstration purposes)
    // In a real implementation, you would need to handle directories properly

    // Check the dBitmap to find an available data block
    dBitmap dBitmap;
    fseek(disk, 2 * BLOCK_SIZE, SEEK_SET);
    fread(&dBitmap, sizeof(Bitmap), 1, disk);

    int block_number = -1;
    for (int i = 0; i < MAX_NUM_BLOCKS; i++) {
        if (!(dBitmap.data[i / 8] & (1 << (i % 8)))) {
            block_number = i;
            break;
        }
    }

    if (block_number == -1) {
        printf("Error: No available data blocks.\n");
        return;
    }

    // Allocate a data block for the file
    inode.direct_blocks[0] = block_number;

    // Update the dBitmap
    dBitmap.data[block_number / 8] |= (1 << (block_number % 8));
    fseek(disk, 2 * BLOCK_SIZE, SEEK_SET);
    fwrite(&dBitmap, sizeof(Bitmap), 1, disk);

    // Write the inode to the disk
    fseek(disk, START_INODE_BLOCK * BLOCK_SIZE + inode_number * sizeof(Inode), SEEK_SET);
    fwrite(&inode, sizeof(Inode), 1, disk);

    // Print the result
    printf("%s was created in block %d with metadata in inode %d\n", file_name, block_number, inode_number);
}

// Function to write data to a file
void write_file(FILE *disk, const char *file_path, const char *data, uint16_t userId) {
    // Locate the file's inode via the directory structure
    Inode inode;
    int inode_number = locate_inode(disk, file_path, &inode);

    if (inode_number == -1) {
        printf("Error: File not found.\n");
        return;
    }

    // Check user permissions
    if (userId != inode.uid) {
        if (inode.mode == 0 || inode.mode == 0x0100) {
            printf("User %d doesn't have permission to write in %s.\n", userId, file_path);
            return;
        }
    }

    // Locate the specific data block and write data to free blocks sequentially
    dBitmap dBitmap;
    fseek(disk, 2 * BLOCK_SIZE, SEEK_SET);
    fread(&dBitmap, sizeof(dBitmap), 1, disk);

    int block_number = -1;
    for (int i = 0; i < MAX_NUM_BLOCKS; i++) {
        if (!(dBitmap.data[i / 8] & (1 << (i % 8)))) {
            block_number = i;
            break;
        }
    }

    if (block_number == -1) {
        printf("Error: No available data blocks.\n");
        return;
    }

    // Write data to the block
    fseek(disk, block_number * BLOCK_SIZE, SEEK_SET);
    fwrite(data, strlen(data), 1, disk);

    // Update the inode with block pointers, file size, and mtime
    inode.direct_blocks[0] = block_number;
    inode.size = strlen(data);
    inode.mtime = (uint32_t)time(NULL);

    // Update the dBitmap
    dBitmap.data[block_number / 8] |= (1 << (block_number % 8));
    fseek(disk, 2 * BLOCK_SIZE, SEEK_SET);
    fwrite(&dBitmap, sizeof(Bitmap), 1, disk);

    // Write the inode to the disk
    fseek(disk, START_INODE_BLOCK * BLOCK_SIZE + inode_number * sizeof(Inode), SEEK_SET);
    fwrite(&inode, sizeof(Inode), 1, disk);

    printf("Data written to the file %s.\n", file_path);
}

// Function to read data from a file
void read_file(FILE *disk, const char *file_path, size_t offset, size_t size, uint16_t userId) {
    // Locate the file's inode via the directory structure
    Inode inode;
    int inode_number = locate_inode(disk, file_path, &inode);

    if (inode_number == -1) {
        printf("Error: File not found.\n");
        return;
    }

    // Check user permissions
    if (userId != inode.uid) {
        if (inode.mode == 0) {
            printf("User %d doesn't have permission to read the file %s.\n", userId, file_path);
            return;
        }
    }

    // Map the logical offset to disk blocks
    int block_number = inode.direct_blocks[0];
    if (block_number == -1) {
        printf("Error: No data blocks allocated.\n");
        return;
    }

    // Read data from the blocks
    char buffer[BLOCK_SIZE];
    fseek(disk, block_number * BLOCK_SIZE + offset, SEEK_SET);
    fread(buffer, size, 1, disk);

    // Update the atime in the inode
    inode.atime = (uint32_t)time(NULL);
    fseek(disk, START_INODE_BLOCK * BLOCK_SIZE + inode_number * sizeof(Inode), SEEK_SET);
    fwrite(&inode, sizeof(Inode), 1, disk);

    printf("File data: %.*s\n", (int)size, buffer);
}

// Function to delete a file
void delete_file(FILE *disk, const char *file_path) {
    // Locate the file's inode via the directory structure
    Inode inode;
    int inode_number = locate_inode(disk, file_path, &inode);

    if (inode_number == -1) {
        printf("Error: File not found.\n");
        return;
    }

    // Mark data blocks as free in the free block vector
    Bitmap dBitmap;
    fseek(disk, 2 * BLOCK_SIZE, SEEK_SET);
    fread(&dBitmap, sizeof(Bitmap), 1, disk);

    int block_number = inode.direct_blocks[0];
    dBitmap.data[block_number / 8] &= ~(1 << (block_number % 8));
    fseek(disk, 2 * BLOCK_SIZE, SEEK_SET);
    fwrite(&dBitmap, sizeof(Bitmap), 1, disk);

    // Update the bit at index {inode number} in iBitmap from 1 to 0
    Bitmap iBitmap;
    fseek(disk, BLOCK_SIZE, SEEK_SET);
    fread(&iBitmap, sizeof(Bitmap), 1, disk);

    iBitmap.data[inode_number / 8] &= ~(1 << (inode_number % 8));
    fseek(disk, BLOCK_SIZE, SEEK_SET);
    fwrite(&iBitmap, sizeof(Bitmap), 1, disk);

    // Remove the inode and directory entry
    memset(&inode, 0, sizeof(Inode));
    fseek(disk, START_INODE_BLOCK * BLOCK_SIZE + inode_number * sizeof(Inode), SEEK_SET);
    fwrite(&inode, sizeof(Inode), 1, disk);

    printf("%s was deleted along with the data in block %d and metadata in inode %d\n", file_path, block_number, inode_number);
}

// Helper function to locate an inode by file path (simplified for demonstration)
int locate_inode(FILE *disk, const char *file_path, Inode *inode) {
    // For simplicity, assume the file path directly maps to an inode number
    // In a real implementation, you would need to traverse the directory structure
    int inode_number = atoi(file_path); // Simplified: file_path is the inode number as a string
    fseek(disk, START_INODE_BLOCK * BLOCK_SIZE + inode_number * sizeof(Inode), SEEK_SET);
    fread(inode, sizeof(Inode), 1, disk);
    return inode_number;
}