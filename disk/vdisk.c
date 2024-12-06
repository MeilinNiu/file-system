#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "block.h"  // Include the header file for function declarations

#define BLOCK_SIZE 512   // Size of each block in bytes
#define MAX_BLOCKS 4096 // Total number of blocks on the disk

// Function to read a block from the disk into a buffer
void read_block(FILE *disk, uint32_t block_number, void *buffer) {
    if (block_number >= MAX_BLOCKS) {
        printf("Error: Block number out of bounds.\n");
        return;
    }

    // Calculate the byte offset for the specified block number
    fseek(disk, block_number * BLOCK_SIZE, SEEK_SET);

    // Read the block from the disk into the buffer
    fread(buffer, BLOCK_SIZE, 1, disk);
}

// Function to write a block to the disk from the provided data
void write_block(FILE *disk, uint32_t block_number, void *data) {
    if (block_number >= MAX_BLOCKS) {
        printf("Error: Block number out of bounds.\n");
        return;
    }

    // Calculate the byte offset for the specified block number
    fseek(disk, block_number * BLOCK_SIZE, SEEK_SET);

    // Write the data to the specified block
    fwrite(data, BLOCK_SIZE, 1, disk);
}

// Function to allocate a block on the disk (mark it as used)
void allocate_block(FILE *disk, uint32_t block_number) {
    if (block_number >= MAX_BLOCKS) {
        printf("Error: Block number out of bounds.\n");
        return;
    }

    // Seek to the free block vector (block 1), and set the bit for this block to 0 (allocated)
    uint8_t byte;
    uint32_t byte_offset = block_number / 8;    // Determine the byte position in the free block vector
    uint8_t bit_offset = block_number % 8;      // Determine the bit position within the byte

    fseek(disk, BLOCK_SIZE + byte_offset, SEEK_SET);  // Move to the correct byte in the free block vector
    fread(&byte, 1, 1, disk);  // Read the byte

    byte &= ~(1 << (7 - bit_offset));  // Set the corresponding bit to 0 (allocated)

    // Write the updated byte back to the disk
    fseek(disk, BLOCK_SIZE + byte_offset, SEEK_SET);
    fwrite(&byte, 1, 1, disk);
}

// Function to free a block on the disk (mark it as unused)
void free_block(FILE *disk, uint32_t block_number) {
    if (block_number >= MAX_BLOCKS) {
        printf("Error: Block number out of bounds.\n");
        return;
    }

    // Seek to the free block vector (block 1), and set the bit for this block to 1 (free)
    uint8_t byte;
    uint32_t byte_offset = block_number / 8;    // Determine the byte position in the free block vector
    uint8_t bit_offset = block_number % 8;      // Determine the bit position within the byte

    fseek(disk, BLOCK_SIZE + byte_offset, SEEK_SET);  // Move to the correct byte in the free block vector
    fread(&byte, 1, 1, disk);  // Read the byte

    byte |= (1 << (7 - bit_offset));  // Set the corresponding bit to 1 (free)

    // Write the updated byte back to the disk
    fseek(disk, BLOCK_SIZE + byte_offset, SEEK_SET);
    fwrite(&byte, 1, 1, disk);
}