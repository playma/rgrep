//myfilesystem.h
#ifndef _MYFILESYSTEM_H
#define _MYFILESYSTEM_H


#define BLOCKSIZE 30*getpagesize()

int myread(char*, unsigned char*);
void check_mmap(char *);
int get_recbeg_offset(int, int, unsigned char*, unsigned char*);

#endif
