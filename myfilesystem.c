#include "myfilesystem.h"
#include "bmsearch.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define handle_error(msg) \
	do{ perror(msg); exit(EXIT_FAILURE); } while(0)

#define min(a,b) ((a)<(b)?(a):(b))

int myread(char *filename, unsigned char* pattern) {
	int i;
	int fd;
	int fileSize;
	int length;
	int offset, offset_old, offset_match, offset_recbeg, offset_recend;
	offset = offset_old = offset_match = offset_recbeg = offset_recend = 0;
	int match_count =0;
	unsigned char *addr;	
	unsigned char *ptr;
	unsigned char *rec_beg;
	unsigned char *match;
	unsigned char *rec_end;
	unsigned char *recbeg = "@\n";
	unsigned char *tmp;
	unsigned char *limit;
	int offset_tmp;
	unsigned char* addr_tmp;
	
	struct stat st;
	struct bmsearch bm;
	struct bmsearch search_rec_end;
	struct bmsearch search_rec_beg;
	
	bmsearch_init(&bm, pattern);
	bmsearch_init(&search_rec_end, recbeg);
	inverse_bmsearch_init(&search_rec_beg, recbeg);

	fd = open(filename, O_RDONLY);
	if(fd < 0) {
		handle_error("open");
		return -1;
	}
	stat(filename, &st);
	fileSize = st.st_size;
	
	//printf("%d %d\n", fd, fileSize);
	
	offset = 0;

	while(offset < fileSize) {
		length = BLOCKSIZE;
		addr = mmap(NULL, BLOCKSIZE, PROT_READ, MAP_PRIVATE, fd, offset);
		check_mmap(addr);
		offset_recend = offset;
	
		limit = addr - offset + fileSize -1;

		while((match = bm_search(&bm, addr + offset_recend - offset, min((addr+length-1), limit))) != -1) {
		//while((match = bm_search(&bm, addr + offset_recend - offset, addr+length-1)) != -1) {
			//match pattern	
			offset_match = match - addr + offset;
		
//			printf("## match pattern at %d\n", offset_match);	

			//find rec start position
			//printf("## find rec start\n");
			while( (rec_beg = inverse_bm_search(&search_rec_beg, addr, addr+offset_match-offset)) == -1) {
				//printf("## can nod find rec_start. mmap new\n");
				length += BLOCKSIZE;
				offset -= BLOCKSIZE;
				munmap(addr, length);
				addr = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, offset);
				check_mmap(addr);
			}
			offset_recbeg = rec_beg - addr + offset;
			//printf("## find begin at %d\n", offset_recbeg);

			//find rec end position	
			while( (rec_end = bm_search(&search_rec_end, addr+offset_match-offset , min((addr+length), limit))) == -1 ) {
			//while( (rec_end = bm_search(&search_rec_end, addr+offset_match-offset , addr+length)) == -1 ) {
				//printf("can not find rec_end. mmap new\n");
				length += BLOCKSIZE;	
				munmap(addr, BLOCKSIZE);
				addr = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, offset);
				check_mmap(addr);
				//exit(1);
			}
			offset_recend = rec_end - addr + offset;
			//printf("## find recend at %d\n", offset_recend);
			match_count++;
			//printf("rec size: %d\n", offset_recend-offset_recbeg);
			/*
			if(offset_recend-offset_recbeg < 0) {
				printf("size < 0\n");
				exit(1);
			}*/
			//get rec
			/*
			unsigned char *r = rec_beg;
			while(r <= rec_end) {
				printf("%c", *r);
				r++;
			}
			*/
			//exit(1);
			//printf("\n");
		}	
		munmap(addr, length);

		offset_old = offset;
		offset += length; 
	}	
	printf("## count: %d\n", match_count);
	//printf("\n");

	return 1;
}

void check_mmap(char *addr) {
	if(addr == MAP_FAILED) {
		handle_error("mmap");
	}
}
