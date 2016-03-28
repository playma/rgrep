#include <string.h>
#include <stdio.h>
#include "bmsearch.h"

void bmsearch_init(struct bmsearch *bm, unsigned char* pattern) {
	bm->pattern = pattern;
	bm->pat_len = strlen(bm->pattern);
	bm_prep(bm);
}

void inverse_bmsearch_init(struct bmsearch *bm, unsigned char* pattern) {
	bm->pattern = pattern;
	bm->pat_len = strlen(bm->pattern);
	inverse_bm_prep(bm);
}

void bm_prep(struct bmsearch *bm) {
	int i;
	int shift1;
	int pat_len = bm->pat_len;
	unsigned char *pattern = bm->pattern;
	int *shift = bm->shift;

	for(i=0 ; i<ALPHABET_LEN ; i++)	shift[i] = pat_len;
	shift1 = pat_len;
	for(i=0 ; i < pat_len ; i++) {
		/*
		if(INSENSITIVE) {
			if(pattern[i] >= 'A' && pattern[i] <= 'Z') {
				pattern[i] = pattern[i] - 'A' + 'a';
			}
		}*/
		shift[pattern[i]] = pat_len - i -1;
		if(pattern[i] == pattern[pat_len-1] && i!=pat_len-1 ) {
			shift1 = pat_len - i - 1;
		}
	}
	bm->shift1 = shift1;
}

void inverse_bm_prep(struct bmsearch *bm) {
	int i;
	int shift1;
	int pat_len = bm->pat_len;
	unsigned char *pattern = bm->pattern;
	int *shift = bm->shift;

	for(i=0 ; i<ALPHABET_LEN ; i++)	shift[i] = pat_len;
	shift1 = pat_len;
	for(i=pat_len-1 ; i >= 0 ; i--) {
		/*
		if(INSENSITIVE) {
			if(pattern[i] >= 'A' && pattern[i] <= 'Z') {
				pattern[i] = pattern[i] - 'A' + 'a';
			}
		}*/
		shift[pattern[i]] = pat_len - i -1;
		if(pattern[i] == pattern[0] && i!=0 ) {
			shift1 = pat_len - i - 1;
		}
	}
	bm->shift1 = shift1;
}

char* bm_search(struct bmsearch* bm, unsigned char *strbeg, unsigned char *strend) {
	int c;
	unsigned char *ptr = strbeg;
	int pat_len = bm->pat_len;
	int strLength = strend - strbeg + 1;

	if(pat_len > strLength ) {
		return NULL;
	}
	
	while(ptr < (strbeg + strLength - pat_len + 1) ) {
		/*
		if(INSENSITIVE) {
			if(*ptr >= 'A' && *ptr <= 'Z') {
				*ptr = *ptr - 'A' + 'a';
			}
		}*/
		while( c = bm->shift[(unsigned char)*ptr] ) {
			ptr += c;
			if(ptr > ( strbeg + strLength - pat_len)) {
				break;
			}
		}
		if(trymatch(ptr-pat_len + 1, strend, bm->pattern)) {
			return ptr - pat_len + 1;
		}
		else {
			ptr += bm->shift1;
//			printf("shift1\n");
		}
	}

	return -1;
}

int trymatch(unsigned char *strbeg, unsigned char *strend, unsigned char *pattern) {
	int i;
	int patternLength = strlen(pattern);
	int strLength = strend - strbeg + 1;
	if( strLength< patternLength) return 0;
	//printf("trymatch : %s\n", str);
	
	for(i=0; i<patternLength; i++) {	
		/*
		if(INSENSITIVE) {
			if(str[i] >= 'A' && str[i] <= 'Z') {
				str[i] = str[i] - 'A' + 'a';
				//printf("str[i] :%c\n", str[i]);
			}
		}*/
		if(strbeg[i]!=pattern[i]) {
			return 0;
		}
	}
	return 1;
}

char* inverse_bm_search(struct bmsearch* bm, unsigned char *strbeg, unsigned char *strend) {
	int c;
	int pat_len = bm->pat_len;
	int strLength = strend - strbeg + 1;

	char *ptr = strend;

	if( pat_len > strLength ) {
		return NULL;
	}
	
	while(ptr > (strend - strLength + pat_len - 1) ) {
		/*
		if(INSENSITIVE) {
			if(*ptr >= 'A' && *ptr <= 'Z') {
				*ptr = *ptr - 'A' + 'a';
			}
		}*/
		while( c = bm->shift[(unsigned char)*ptr] ) {
			ptr -= c;
			if(ptr < ( strend - strLength + pat_len))	break;
		}
		if(trymatch(ptr-pat_len + 1, strend,bm->pattern)) {
			return ptr - pat_len + 1;
		}
		else {
			ptr -= bm->shift1;
		}
	}
	return -1;
}

