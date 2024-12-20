#include "bitmap.h"

int find_available_inode(FILE *disk) {
    iBitmap inodeBitmap;
    fseek(disk, BLOCK_SIZE, SEEK_SET);
    fread(&inodeBitmap, sizeof(inodeBitmap), 1, disk);

    int inode_number = -1;
    for (int i = 0; i < NUM_INODES; i++) {
        if (!(inodeBitmap.iBit[i] == 1)) {
            inode_number = i;
            break;
        }
    }

    if (inode_number == -1) {
        printf("Error: No available inodes.\n");
    }

    return inode_number;
}