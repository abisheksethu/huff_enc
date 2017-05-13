#include <stdio.h>
#define MAX_INPUT		100
#define MAX_SYMBOL		26
#define REF_VAL_INDEX	97
#define NO_VAL			126-REF_VAL_INDEX

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

    num >>= 1; 
    while(num)
    {
       reverse_num <<= 1;       
       reverse_num |= num & 1;
       num >>= 1;
       count--;
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
	for(iter = 0; iter < (table_len + (table_len-1)); iter++)
	{
		if (NO_VAL == getleaf(tree[iter]))
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

			// printf("parent = %d(%c:%c)\n",getfreq(tree[iter]), left+97,right+97 );
			
			if(left == code_index)
			{
				code &= ~(1 << bitlen);
				// printf("ld..\n");
				// printf("bitlen = %d code = %d\n", bitlen+1, code);
				return code;
			}
			else if(right == code_index)
			{
				code |= 1 << bitlen;
				// printf("rd..\n");
				// printf("bitlen = %d code = %d\n", bitlen+1, code);		
				return code;
			}
			else{
				if(right == NO_VAL){
					code |= 1 << bitlen;
					bitlen++;
					// printf("r..\n");
				}
				else{
					code &= ~(1 << bitlen);
					bitlen++;
					// printf("l..\n");
				}
			}
		}		
	}
}
short bitlen_identifier(short* tree, unsigned int code_index, unsigned int table_len){
	
	unsigned int iter;
	unsigned short code,bitlen;
	unsigned char left, right;
	code = 0;
	bitlen = 0;
	for(iter = 0; iter < (table_len + (table_len-1)); iter++)
	{
		if (NO_VAL == getleaf(tree[iter]))
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

			// printf("parent = %d(%c:%c)\n",getfreq(tree[iter]), left+97,right+97 );
			
			if(left == code_index)
			{
				code &= ~(1 << bitlen);
				// printf("ld..\n");
				// printf("bitlen = %d code = %d\n", bitlen+1, code);
				return bitlen+1;
			}
			else if(right == code_index)
			{
				code |= 1 << bitlen;
				// printf("rd..\n");
				// printf("bitlen = %d code = %d\n", bitlen+1, code);		
				return bitlen+1;
			}
			else{
				if(right == NO_VAL){
					code |= 1 << bitlen;
					bitlen++;
					// printf("r..\n");
				}
				else{
					code &= ~(1 << bitlen);
					bitlen++;
					// printf("l..\n");
				}
			}
		}		
	}
}
void encoder(unsigned char input[MAX_INPUT], unsigned int length){

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
	for(iter = 0; iter < length; iter++)
	{
		temp = input[iter] - REF_VAL_INDEX;
		//printf("-->%d ",temp);
		freq[temp]++;			
	}

	// printf("\n");
	//Create Symbol table - not sorted
	for (sym_loc = 0; sym_loc < MAX_SYMBOL; sym_loc++)
	{
		if(freq[sym_loc] != 0)
			{	
				//printf("-->%c", (sym_loc+REF_VAL_INDEX));
				table[table_len] = converttonode(sym_loc, freq[sym_loc]);
				table_len++;
			}	
	}

	//Sorting symbol table
	for(iter = 0; iter < table_len; iter++)
	{
		unsigned int fv = getfreq(table[iter]);
		unsigned int count;
		for(count = iter+1; count < table_len; count++)
		{
			unsigned int fd = getfreq(table[count]);
			if(fv > fd) {
				unsigned short swap_var = table[count];
				table[count] = table[iter];
				table[iter] = swap_var;  
				fv = getfreq(swap_var);
			}
		}
	}		

	//Binary Search Tree
	unsigned int k = ((2*(table_len-1)));
	tree[k] = table[0];
	unsigned char freq_sum = getfreq(table[0]);
	for(iter = 1; iter < table_len; iter++)
	{	
		unsigned char temp_freq;
		temp_freq = getfreq(freq_sum) + getfreq(table[iter]);
	 	table[iter+(table_len-1)] = converttonode(NO_VAL,temp_freq);
		freq_sum = temp_freq;
	 	tree[k-1] = table[iter];
	 	tree[k-2] = converttonode(NO_VAL,temp_freq);
	    k = k - 2;
	}

	//Code book
	for(iter = 0; iter < (table_len); iter++)
	{
		unsigned int code_index = (getleaf(table[iter]));
		code_book[code_index] = code_identifier(tree, code_index, table_len);
		code_book[code_index] = reverseBits(code_book[code_index]);
		length_book[code_index] = bitlen_identifier(tree, code_index, table_len);
	}

	//Print the table
	printf("\nSymbol table\n");
	for(iter = 0; iter < (table_len); iter++)
		printf("  %c(%d)", (getleaf(table[iter]))+REF_VAL_INDEX, getfreq(table[iter]));	
	printf("\nBinary Tree\n");
	for(iter = 0; iter < (table_len + (table_len-1)); iter++)
		printf("  %c(%d)", (getleaf(tree[iter]))+REF_VAL_INDEX, getfreq(tree[iter]));	
	printf("\nCode book\n");
	for(iter = 0; iter < table_len; iter++){
		printf("  %c(0x%X)", ((getleaf(table[iter]))+REF_VAL_INDEX), code_book[((getleaf(table[iter])))]);	
	}
	printf("\nBitlength book\n");
	for(iter = 0; iter < table_len; iter++){
		printf("  %c(%d)", ((getleaf(table[iter]))+REF_VAL_INDEX), length_book[((getleaf(table[iter])))]);	
	}

	//Encode Data
}


int main(){

	// short check = converttonode(3,2);
	// printf("getleaf = %d\n",getleaf(check));
	// printf("getfreq = %d\n", getfreq(check));

	unsigned char input[MAX_INPUT] = {"abccddddd"};

	encoder(input, 12);

	return 0;
}