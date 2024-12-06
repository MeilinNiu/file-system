# File System
Directory Structure
```bash
/File_System_Project
│
├── /disk              # Contains the virtual disk file 
│   └── vdisk          
│
├── /io                # Contains the main FS code (file_operations.c)
│   ├── file_operations.c  # Main FS code
│   ├── superblock.c   # Code for initializing the superblock
│   ├── freeblock.c    # Code for initializing the free block vector
│   └── inode.c     # Code for initializing inode and directory entry
│
├── /test              # Contains test programs
│   ├── test01.c       # Test program for disk operations
│   ├── test02.c       # Test program for file system operations
│   └── test03.c       # Test program for crash recovery and checkpointing
│
└── Makefile           # For compiling and linking the project
```
