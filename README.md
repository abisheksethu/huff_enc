## huffman encoder algo ##

The Huffman Encoding algorithm represents a higher probability alphabets with the smaller representation. The dynamic Huffman encoding begins with one initial pass to all the inputs to determine the frequency distribution of characters. After calculating the histogram, Huffman tree is computed to build the codebook. Finally, the input characters are encoded using codebook and respective bit length.

The design calculates dynamic frequency distribution from the input ASCII characters and stores them in an internal ram. Each node is consisting of 16 bit which represents character and frequency. First byte is the symbol name while the lower byte is the frequency. Creation of binary search tree and codebook. Code needs to be asserted twice; for implementing frequency distribution and sorting the characters based on frequency and coding the data. Algorithm flow is shown below

** Algorithm **

* Compute Frequency Distribution
* Build Symbol table based on the frequency
* Build a Binary search tree
* The new node whose frequency is the sum of the two symbols are inserted into binary search tree
* Code book is created by traversing the binary tree for the character
* Encode the input data based on the codebook

Note: Implemented upto codebook creation