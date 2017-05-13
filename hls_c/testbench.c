#include "encoder.h"
#include <string.h>
int main(){

	unsigned char input[MAX_INPUT] = {"hellowoow"};
	short out_tree[MAX_SYMBOL];
	short out_codebook[MAX_SYMBOL];
	short out_bitlength[MAX_SYMBOL];
	unsigned int iter;

	encoder(input, 9, out_tree, out_codebook, out_bitlength);

	for(iter = 0; iter < 9; iter++){
		printf("input_data %c = 0x%X = %d\n", input[iter], out_codebook[input[iter]-97],out_bitlength[input[iter]-97] );
	}
	return 0;
}
