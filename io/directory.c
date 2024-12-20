#include "directory.h"
#include "inode.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Structure to hold a directory table
typedef struct {
    char path[MAX_PATH_LENGTH];
    DirectoryEntry entries[MAX_ENTRIES_PER_DIR];
    size_t entryCount;
} Directory;

// Static directory table
static Directory directoryTable[MAX_DIRECTORIES];
static size_t directoryCount = 0;

// Helper function to validate file/directory names
static void validateName(const char *name) {
    if (name == NULL || strlen(name) == 0 || strlen(name) > MAX_LEN_FILENAME) {
        fprintf(stderr, "Error: Invalid name. Must be 1-30 characters long.\n");
        exit(EXIT_FAILURE);
    }
}

// Initialize the DirectoryManager
void createRootDirectory(DirectoryManager *dm, uint8_t rootInodeId) {
    for (size_t i = 0; i < directoryCount; i++) {
        if (strcmp(directoryTable[i].path, "/") == 0) {
            fprintf(stderr, "Error: Root directory already exists.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Initialize the root directory
    strcpy(directoryTable[directoryCount].path, "/");
    directoryTable[directoryCount].entryCount = 0;
    directoryCount++;
    printf("Root directory created successfully.\n");
}

// Add an entry to a directory
int addEntry(DirectoryManager *dm, const char *path, const DirectoryEntry *entry) {
    validateName(entry->filename[MAX_LEN_FILENAME]);  // Null-terminated string

    for (size_t i = 0; i < directoryCount; i++) {
        if (strcmp(directoryTable[i].path, path) == 0) {
            // Check for duplicates
            for (size_t j = 0; j < directoryTable[i].entryCount; j++) {
                if (strcmp(directoryTable[i].entries[j].filename, entry->filename) == 0) {
                    fprintf(stderr, "Error: File already exists: %s\n", entry->filename);
                    exit(EXIT_FAILURE);
                }
            }

            // Add the entry
            if (directoryTable[i].entryCount >= MAX_ENTRIES_PER_DIR) {
                fprintf(stderr, "Error: Directory is full.\n");
                exit(EXIT_FAILURE);
            }
            directoryTable[i].entries[directoryTable[i].entryCount++] = *entry;
            printf("Entry '%s' added to directory '%s'.\n", entry->filename, path);
            return;
        }
    }

    fprintf(stderr, "Error: Directory does not exist: %s\n", path);
    exit(EXIT_FAILURE);
}

// Get an entry from a directory
DirectoryEntry getEntry(const DirectoryManager *dm, const char *path, const char *fileName, DirectoryEntry *outputEntry) {
    for (size_t i = 0; i < directoryCount; i++) {
        if (strcmp(directoryTable[i].path, path) == 0) {
            for (size_t j = 0; j < directoryTable[i].entryCount; j++) {
                if (strcmp(directoryTable[i].entries[j].filename, fileName) == 0) {
                    return directoryTable[i].entries[j];
                }
            }
            fprintf(stderr, "Error: File not found: %s\n", fileName);
            exit(EXIT_FAILURE);
        }
    }

    fprintf(stderr, "Error: Directory does not exist: %s\n", path);
    exit(EXIT_FAILURE);
}

// Remove an entry from a directory
void removeEntry(const char *path, const char *fileName) {
    for (size_t i = 0; i < directoryCount; i++) {
        if (strcmp(directoryTable[i].path, path) == 0) {
            for (size_t j = 0; j < directoryTable[i].entryCount; j++) {
                if (strcmp(directoryTable[i].entries[j].filename, fileName) == 0) {
                    // Shift entries to remove the file
                    for (size_t k = j; k < directoryTable[i].entryCount - 1; k++) {
                        directoryTable[i].entries[k] = directoryTable[i].entries[k + 1];
                    }
                    directoryTable[i].entryCount--;
                    printf("Entry '%s' removed from directory '%s'.\n", fileName, path);
                    return;
                }
            }
            fprintf(stderr, "Error: File not found: %s\n", fileName);
            exit(EXIT_FAILURE);
        }
    }

    fprintf(stderr, "Error: Directory does not exist: %s\n", path);
    exit(EXIT_FAILURE);
}

// List all entries in a directory
void listEntries(const char *path) {
    for (size_t i = 0; i < directoryCount; i++) {
        if (strcmp(directoryTable[i].path, path) == 0) {
            printf("Listing entries in directory '%s':\n", path);
            for (size_t j = 0; j < directoryTable[i].entryCount; j++) {
                printf(" - %s (inode: %d)\n", directoryTable[i].entries[j].filename, directoryTable[i].entries[j].inode_number);
            }
            return;
        }
    }

    fprintf(stderr, "Error: Directory does not exist: %s\n", path);
    exit(EXIT_FAILURE);
}

// Load root directory
void loadRootDirectory(uint8_t rootInodeId, const Inode *rootInode) {
    if (rootInode->file_type != 2) { // Directory type
        fprintf(stderr, "Error: Root inode is not a directory.\n");
        exit(EXIT_FAILURE);
    }

    createRootDirectory(NULL, rootInodeId); // Assuming NULL for DirectoryManager as it's not provided
    printf("Root directory loaded successfully.\n");
}
