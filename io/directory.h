#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdint.h>
#include <stddef.h>

#define MAX_LEN_FILENAME 31
#define MAX_ENTRIES_PER_DIR 100
#define MAX_PATH_LENGTH 128
#define MAX_DIRECTORIES 50

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

// Directory
typedef struct {
    char path[MAX_PATH_LENGTH];               // Path of the directory
    DirectoryEntry entries[MAX_ENTRIES_PER_DIR];
    size_t entryCount;                        // Number of entries in the directory
} Directory;

// Directory Manager
typedef struct {
    Directory directories[MAX_DIRECTORIES];
    size_t directoryCount;                    // Number of directories
} DirectoryManager;

// Functions
void initDirectoryManager(DirectoryManager *dm);
void createRootDirectory(DirectoryManager *dm, uint8_t rootInodeId);
int addEntry(DirectoryManager *dm, const char *path, const DirectoryEntry *entry);
int getEntry(const DirectoryManager *dm, const char *path, const char *fileName, DirectoryEntry *outputEntry);
int removeEntry(DirectoryManager *dm, const char *path, const char *fileName);
size_t listEntries(const DirectoryManager *dm, const char *path, DirectoryEntry *outputEntries, size_t bufferSize);

#endif