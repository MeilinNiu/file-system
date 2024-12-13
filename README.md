# File System
Directory Structure
```bash
/File_System_Project
│
├── /disk              # Contains the virtual disk file 
│   └── vdisk          
│
├── /io                
│   ├── superblock.h
│   ├── superblock.c   # Code for initializing the superblock
│   ├── bitmap.c    # Code for recording the status of inode and data block in iBitmap and dBitmap
│   ├── bitmap.h
│   ├── inode.h
│   └── inode.c     # Code for initializing inode and directory entry
│
└── Makefile           # For compiling and linking the project
```
