
#include "fs.h"
#include "disk.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>

#define FS_MAGIC           0xf0f03410
#define INODES_PER_BLOCK   128
#define POINTERS_PER_INODE 5
#define POINTERS_PER_BLOCK 1024
bool MOUNTED = false;
bool *bitmap;			//global array keeping track of the used data blocks
bool *inodemap;			//global array keeping track of the used inodes

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
	int indirect;						//gives the location of the indirect block that will give the rest of the indirect blocks
};

union fs_block {
	struct fs_superblock super;					//access through fs_block.super
	struct fs_inode inode[INODES_PER_BLOCK]; 	//access through fs_block.inode[x]
	int pointers[POINTERS_PER_BLOCK];			//I think these are all of the pointers for the file
	char data[DISK_BLOCK_SIZE];					//contains all of the data for the block
};



int fs_format() //superblock write and inode write and you need the file bitmap stuff
{
	if (MOUNTED)
	{
		printf("disk already mounted. failure\n");
		return -1;
	}

	else{
		union fs_block block;
		block.super.nblocks=disk_size();
		block.super.magic = FS_MAGIC;
		block.super.ninodeblocks = block.super.nblocks * .1;
		block.super.ninodes = block.super.ninodeblocks * INODES_PER_BLOCK; 
		return 1;
	}
	return 0; //otherwise also if you get here, you've really screwed up
}

void fs_debug()
{
	union fs_block block;

	disk_read(0,block.data);
	if (block.super.magic!=FS_MAGIC)
	{
		printf("Error magic isn't working (the magic number is not right) \n");
		
	}
	else if (block.super.magic==FS_MAGIC)
	{
		printf("Magic number is valid\n");
		printf("superblock:\n");
		printf("    %d blocks on disk\n",block.super.nblocks);
		printf("    %d inode blocks\n",block.super.ninodeblocks);
		printf("    %d inodes in total\n\n",block.super.ninodes);
		union fs_block inodeblock;// variable for inodeblock
		union fs_block indirect;
		disk_read(1,inodeblock.data);
		for (int i = 0; i < POINTERS_PER_INODE; i++)
		{
			if (inodeblock.inode[i].isvalid)
			{
				printf("Inode %d: %d\n",i,inodeblock.inode[i].size);
				printf("Direct blocks: \n");
				
				for (int j = 0; j < POINTERS_PER_INODE; j++)//print out the direct blocks
				{
					if (inodeblock.inode[i].direct[j]!=0)//if the direct block has stuff in it
					{
						printf("	direct[%d]: %d\n",j,inodeblock.inode[i].direct[j]);
					}
				}
				printf("\n");
				if (inodeblock.inode[i].indirect!=0)
				{
					printf("	indirect block: %d\n", inodeblock.inode[i].indirect);
					// indirect pointers print out
					disk_read(inodeblock.inode[i].indirect,indirect.data); // read in the indirect data block of pointers
					for (int m = 0; m < POINTERS_PER_BLOCK; m++)
					{
						if (indirect.pointers[m]!=0)
						{
							printf("	indirect pointer[%d]: %d\n", m, indirect.pointers[m]);
						}
					}
				}
				
			}
			
		}
	}
}

int fs_mount()
{
	union fs_block block;
	// idk how to check for a filesystem
	disk_read(0,block.data);
	if (block.super.magic==FS_MAGIC)//check superblock
	{
		bitmap = (bool *)malloc(disk_size()*sizeof(bool));
		inodemap = (bool *)malloc(block.super.ninodes*sizeof(bool));

		//make bitmap and make which one is free




		MOUNTED = true;
	}


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

void inode_save( int inumber, struct fs_inode *inode ){
	
}

void inode_load(int inumber, struct fs_inode *inode )//needs to be called after reading
{
	union fs_block block;
	int locat = inumber/INODES_PER_BLOCK; // gives which inode block to load
	disk_read(locat+1, block.data);
	assert(block.super.ninodes>inumber);

}