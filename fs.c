
#include "fs.h"
#include "disk.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define FS_MAGIC           0xf0f03410
#define INODES_PER_BLOCK   128
#define POINTERS_PER_INODE 5
#define POINTERS_PER_BLOCK 1024
#define MOUNTED 		   0

struct fs_superblock {
	int magic;			//magic number
	int nblocks;		//total number of blocks in the file
	int ninodeblocks;	//number of inode blocks in the file
	int ninodes;		//total number of inodes in the file
};

struct fs_inode {
	int isvalid;						//if the inode is valid
	int size;							//size of entire file?
	int direct[POINTERS_PER_INODE];		//array of direct pointers
	int indirect;
};

union fs_block {
	struct fs_superblock super;					//access through fs_block.super
	struct fs_inode inode[INODES_PER_BLOCK]; 	//access through fs_block.inode[x]
	int pointers[POINTERS_PER_BLOCK];			//I think these are all of the pointers for the file
	char data[DISK_BLOCK_SIZE];					//contains all of the data for the block
};

int fs_format()
{
	//calls superblock write and inode write and you need the file bitmap stuff
	return 0;
}

void fs_debug()
{
	union fs_block block;

	disk_read(0,block.data);
	if (block.super.magic!=FS_MAGIC)
	{
		printf("Error magic isn't working (the magic number is not right) \n");
	}else if (block.super.magic==FS_MAGIC)
	{
		printf("Magic number is valid\n");
	}

	printf("superblock:\n");
	printf("    %d blocks on disk\n",block.super.nblocks);
	printf("    %d inode blocks\n",block.super.ninodeblocks);
	printf("    %d inodes in total\n",block.super.ninodes);
	union fs_block iNodeBlock;

	disk_read(1,iNodeBlock.data);
	for (int i = 0; i < POINTERS_PER_INODE; i++)
	{
		if (iNodeBlock.inode[i].isvalid)
		{
			printf("inode: ");
			printf("	Size of inode %d: %d\n",i,iNodeBlock.inode[i].size);
			printf("	Direct blocks: ");
			for (i = 0; i < POINTERS_PER_INODE; i++)
			{
				if (iNodeBlock.inode->direct[i]!=0)
				{
					printf(" %d", iNodeBlock.inode->direct[i]);
				}
			}
			printf("\n");
			if (iNodeBlock.inode->indirect!=0)
			{
				printf("	indirect block: %d", iNodeBlock.inode->indirect);
				//TODO indirect pointers print out

			}


			
		}
		
	}
}

int fs_mount()
{
	return 0;
}

int fs_create()
{
	return 0;
}

int fs_delete( int inumber )
{
	return 0;
}

int fs_getsize( int inumber )
{
	return -1;
}

int fs_read( int inumber, char *data, int length, int offset )
{

	return 0;
}

int fs_write( int inumber, const char *data, int length, int offset )
{
	return 0;
}

void inode_write( int inumber, struct fs_inode *inode ){
	
}

void inode_read(int inumber, struct fs_inode *inode ){
	int offset = inumber/INODES_PER_BLOCK;	//which block past 1 is the inode at
	union fs_block iNodeRead;
	disk_read(1,iNodeRead.data);
	inode->size = iNodeRead.inode[offset].size; //need to take size/4096 = #of blocks needed to read all the data
	//if ceiling(size/4096)<5, then we only need to use the direct pointers
	while (sizeremain > 0)
	{
		/* code */
	}
	
	inode->isvalid = iNodeRead.inode[offset].isvalid;
	//inode->direct = iNodeRead.inode[offset].direct;
	//inode->indirect = iNodeRead.inode[offset].indirect;

}