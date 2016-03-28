#ifndef _BMSEARCH_H
#define _BMSEARCH_H

#define ALPHABET_LEN 256

struct bmsearch {
	unsigned char *pattern;
	int pat_len;
	int nocase;
	int shift1;
	int shift[ALPHABET_LEN];
};


void bmsearch_init(struct bmsearch*, unsigned char*);
void bm_prep(struct bmsearch*);
char* bm_search(struct bmsearch* , unsigned char*, unsigned char*);
int trymatch(unsigned char* , unsigned char*, unsigned char*);


void inverse_bmsearch_init(struct bmsearch*, unsigned char*);
void inverse_bm_prep(struct bmsearch*);
char* inverse_bm_search(struct bmsearch* , unsigned char*, unsigned char*);

#endif
