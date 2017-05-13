#include "encoder.h"

short converttonode(unsigned char freq, unsigned char sym_loc) {
	short temp = (freq << 8 & 0xFF00) | sym_loc ;
	return temp;
}
unsigned char getleaf(short node){
	return (node >> 8);
}
unsigned char getfreq(short node){
	unsigned char ret = (unsigned char)(node & 0xFF);
	return ret;
}
short reverseBits(short num){

    unsigned int count = sizeof(num) * 8 - 1;
    short reverse_num = num;
    short iter;

    num >>= 1;
    LOOP_D: for(iter = 0; iter < 250; iter++)
    {
       if(num != 0) {
		   reverse_num <<= 1;
		   reverse_num |= num & 1;
		   num >>= 1;
		   count--;
       }
    }
    reverse_num <<= count;
    return reverse_num;
}
short code_identifier(short* tree, unsigned int code_index, unsigned int table_len){

	unsigned int iter;
	unsigned short code,bitlen;
	unsigned char left, right;
	code = 0;
	bitlen = 0;
	LOOP_B: for(iter = 0; iter < 2*MAX_SYMBOL; iter++)
	{
		if (NO_VAL == getleaf(tree[iter]) && iter < (table_len + (table_len-1)))
		{
			if(getfreq(tree[iter+1]) < getfreq(tree[iter+2]))
			{
				left = getleaf(tree[iter+1]);
				right = getleaf(tree[iter+2]);
			}
			else
			{
				left = getleaf(tree[iter+2]);
				right = getleaf(tree[iter+1]);
			}

			if(left == code_index)
			{
				code &= ~(1 << bitlen);
				return code;
			}
			else if(right == code_index)
			{
				code |= 1 << bitlen;
				return code;
			}
			else{
				if(right == NO_VAL){
					code |= 1 << bitlen;
					bitlen++;
				}
				else{
					code &= ~(1 << bitlen);
					bitlen++;
				}
			}
		}
	}
	return 0;
}
short bitlen_identifier(short* tree, unsigned int code_index, unsigned int table_len){

	unsigned int iter;
	unsigned short code,bitlen;
	unsigned char left, right;
	code = 0;
	bitlen = 0;
	LOOP_C: for(iter = 0; iter < 2*MAX_SYMBOL; iter++)
		{
			if (NO_VAL == getleaf(tree[iter]) && iter < (table_len + (table_len-1)))
			{
			if(getfreq(tree[iter+1]) < getfreq(tree[iter+2]))
			{
				left = getleaf(tree[iter+1]);
				right = getleaf(tree[iter+2]);
			}
			else
			{
				left = getleaf(tree[iter+2]);
				right = getleaf(tree[iter+1]);
			}


			if(left == code_index)
			{
				code &= ~(1 << bitlen);
				return bitlen+1;
			}
			else if(right == code_index)
			{
				code |= 1 << bitlen;
				return bitlen+1;
			}
			else{
				if(right == NO_VAL){
					code |= 1 << bitlen;
					bitlen++;
				}
				else{
					code &= ~(1 << bitlen);
					bitlen++;
				}
			}
		}
	}
	return 0;
}
void encoder(unsigned char input[MAX_INPUT], unsigned int length, short out_tree[MAX_INPUT], short out_codebook[MAX_INPUT], short out_length[MAX_INPUT]){

	unsigned char freq[MAX_SYMBOL] = {0};
	short table[MAX_SYMBOL];
	short tree[MAX_SYMBOL];
	short code_book[MAX_SYMBOL] = {0};
	short length_book[MAX_SYMBOL] = {0};
	unsigned char code_length;
	unsigned int table_len;
	unsigned char sym_loc;
	unsigned int iter;

	table_len = 0;
	int temp;

	//Frequency distribution
	LOOP_X: for(iter = 0; iter < MAX_INPUT; iter++)
	{
		if(iter < length){
			temp = input[iter] - REF_VAL_INDEX;
			freq[temp]++;
		}
	}

	LOOP_F: for (sym_loc = 0; sym_loc < MAX_SYMBOL; sym_loc++)
	{
		if(freq[sym_loc] != 0)
			{
				table[table_len] = converttonode(sym_loc, freq[sym_loc]);
				table_len++;
			}
	}

	//Sorting symbol table
	LOOP_Y: for(iter = 0; iter < MAX_SYMBOL; iter++)
	{
		if(iter < table_len) {
			unsigned int fv = getfreq(table[iter]);
			unsigned int count;
			LOOP_G: for(count = iter+1; count < MAX_SYMBOL; count++)
			{
				if(count < table_len){
				unsigned int fd = getfreq(table[count]);
					if(fv > fd) {
						unsigned short swap_var = table[count];
						table[count] = table[iter];
						table[iter] = swap_var;
						fv = getfreq(swap_var);
					}
				}
			}
		}
	}

	//Binary Search Tree
	unsigned int k = ((2*(table_len-1)));
	tree[k] = table[0];
	unsigned char freq_sum = getfreq(table[0]);
	LOOP_Z: for(iter = 1; iter < MAX_SYMBOL; iter++)
	{
		if(iter < table_len) {
			unsigned char temp_freq;
			temp_freq = getfreq(freq_sum) + getfreq(table[iter]);
			table[iter+(table_len-1)] = converttonode(NO_VAL,temp_freq);
			freq_sum = temp_freq;
			tree[k-1] = table[iter];
			tree[k-2] = converttonode(NO_VAL,temp_freq);
			k = k - 2;
		}
	}

	//Code book
	LOOP_A: for(iter = 0; iter < MAX_SYMBOL; iter++)
	{
		if(iter < table_len) {
			unsigned int code_index = (getleaf(table[iter]));
			code_book[code_index] = code_identifier(tree, code_index, table_len);
			code_book[code_index] = reverseBits(code_book[code_index]);
			length_book[code_index] = bitlen_identifier(tree, code_index, table_len);
		}
	}

	//Print the table
//	printf("\nSymbol table\n");
//	for(iter = 0; iter < (table_len); iter++)
//		printf("  %c(%d)", (getleaf(table[iter]))+REF_VAL_INDEX, getfreq(table[iter]));
	printf("\nBinary Tree\n");
	for(iter = 0; iter < (table_len + (table_len-1)); iter++)
		printf("  %c(%d)", (getleaf(tree[iter]))+REF_VAL_INDEX, getfreq(tree[iter]));
//	printf("\nCode book\n");
//	for(iter = 0; iter < table_len; iter++){
//		printf("  %c(0x%X)", ((getleaf(table[iter]))+REF_VAL_INDEX), code_book[((getleaf(table[iter])))]);
//	}
//	printf("\nBitlength book\n");
//	for(iter = 0; iter < table_len; iter++){
//		printf("  %c(%d)", ((getleaf(table[iter]))+REF_VAL_INDEX), length_book[((getleaf(table[iter])))]);
//	}

	LOOP_E: for(iter = 0; iter < MAX_SYMBOL; iter++){
		out_tree[iter] = tree[iter];
		out_codebook[iter] = code_book[iter];
		out_length[iter] = length_book[iter];
	}

	//Encode Data
}
