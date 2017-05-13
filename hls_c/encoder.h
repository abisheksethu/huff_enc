#include <stdio.h>
#define MAX_INPUT		100
#define MAX_SYMBOL		26
#define REF_VAL_INDEX	97
#define NO_VAL			126-REF_VAL_INDEX

void encoder(unsigned char input[MAX_INPUT], unsigned int length, short out_tree[MAX_SYMBOL], short out_codebook[MAX_SYMBOL], short out_length[MAX_SYMBOL]);
