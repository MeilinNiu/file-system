#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "superblock.c"

typedef struct {
    uint8_t blocks[512];  // Bitmap to track 4096 blocks
} FreeBlockVector;

// Function to initialize the free block vector
void init_free_block_vector(FILE *disk) {
    FreeBlockVector free_block_vector;
    memset(free_block_vector.blocks, 0xFF, sizeof(free_block_vector.blocks));  // Mark all blocks as free (set all bits to 1)

    // Mark the first 10 blocks as allocated (reserved for system use)
    free_block_vector.blocks[0] = 0x00;  // Block 0
    free_block_vector.blocks[1] = 0x00;  // Block 1
    free_block_vector.blocks[2] = 0x00;  // Block 2
    free_block_vector.blocks[3] = 0x00;  // Block 3
    free_block_vector.blocks[4] = 0x00;  // Block 4
    free_block_vector.blocks[5] = 0x00;  // Block 5
    free_block_vector.blocks[6] = 0x00;  // Block 6
    free_block_vector.blocks[7] = 0x00;  // Block 7
    free_block_vector.blocks[8] = 0x00;  // Block 8
    free_block_vector.blocks[9] = 0x00;  // Block 9

    fseek(disk, BLOCK_SIZE, SEEK_SET);  // Move to block 1 (free block vector location)
    fwrite(&free_block_vector, sizeof(FreeBlockVector), 1, disk);  // Write free block vector to disk
}