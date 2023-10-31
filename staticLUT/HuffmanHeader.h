#ifndef _HUFFMAN_HEADER_
#define _HUFFMAN_HEADER_
// Macros
#define MAX_FILE_LENGTH 512000        // Constraint for input file (256 kB)
#define MAX_COMPRESSED_LENGTH 128000  // Constraint for encoded file (128 kB)
#define MAX_PATH_LENGTH 256           // Constraint for file path length
#define MAX_FILENAME_LENGTH 64        // Constraint for filename 
#define MAX_SYMBOLS 128               // ASCII
#define MAX_CODE_LENGTH 16            // Should be a multiple of 8
#define MAX_TABLES 2048               // For Max symbols min table size is over 50
#define MAX_ROWS_PER_TABLE 64         // Sets the maximum number of rows per LUT

// Structs & Typedefs
typedef struct node_t {
    uint8_t symbol;
    float probability;
    struct node_t *leftChild;
    struct node_t *rightChild;
} node_t;                       // Each node_t contains a symbol, its probability in the sample text, and pointers to its children in a tree

typedef struct {
    node_t **treeArray;
    uint32_t numNodes;
    uint32_t maxNodes;
} minHeap_t;                    // Only one minHeap_t should exist at a time: contains a pointer to the root node, # of nodes in the tree, and the max # of nodes 

typedef struct {
    uint8_t symbol;
    uint16_t bitcode;
    uint32_t code_length;
} huffCode_t;                   // Each huffCode_t contains a symbol, its Huffman Encoding, and the length of its encoding 

typedef struct {
    uint8_t symbol;
    uint32_t code_length;
} lut;                          // Each lut contains a symbol paired with its huffman encoding length

/***** Min Heap *****/
node_t* allocateNewNode(             // Dynamically allocates a min-heap node containing a symbol-probability pair
    uint8_t symbol,
    float probability);
uint32_t isLeaf(                // Returns 1 if the given node is a leaf node; returns -1 otherwise
    node_t* node);
void swapNodes(                      // Swaps two node pointers with each other
    node_t** n1,
    node_t** n2);
void heapify(                        // Standard min-heapify
    minHeap_t* minHeap, 
    uint32_t i);
node_t* popMin(                      // Pop the root, set the last node as the new root, and heapify
    minHeap_t* minHeap); 
minHeap_t* createMinHeap(            // Dynamically allocates memory for min-heap and tree array 
    uint32_t maxNodes);
void insertMinHeap(                  // Insert a new node into the correct position of the min-heap
    minHeap_t* minHeap,
    node_t* newNode);
minHeap_t* initMinHeap(              // Create and initialize the min-heap according to symbols and their probabilities 
    uint8_t alph[],
    float prob[], 
    uint32_t size);

/***** Huffman Encoding *****/
node_t* buildHuffmanEncTree(        // Construct the huffman tree from the min-heap, by adding the appropriate internal nodes
    uint8_t alph[],                        
    float prob[], 
    uint32_t size);
void printCodesBitified(            // Generate the huffman code for every character in the tree, recusively, and store it in a table
    node_t* root, 
    uint8_t* buff, 
    uint32_t bit_count, 
    huffCode_t* asciiToHuffman);
void generatehuffmanCodesBitified(  // Uses the huffman tree to populate a table that maps each symbol to its huffman encoding in one memory access (Don't have to traverse tree anymore in order to encode)
    node_t* root,
    huffCode_t* asciiToHuffman);
void generateEncodedFileBitified(   // Generates the huffman encoding of the given sample and stores it in a file
    const char* sample_filename,   
    const char* encoded_filename, 
    huffCode_t* asciiToHuffman);

/***** LUT *****/
void lutPopulate(                     // 
    node_t* root, 
    uint8_t* buff, 
    uint32_t bit_count, 
    uint32_t longest_code_exp,
    lut all_luts[MAX_TABLES][MAX_ROWS_PER_TABLE], 
    uint32_t* lut_num, 
    uint32_t* row_count);
void lutCreation(                     // 
    node_t* root, 
    lut all_luts[MAX_TABLES][MAX_ROWS_PER_TABLE],
    uint32_t longest_code_exp);

/***** Huffman Decoding *****/
void treeDecodingBitByBit(                 //
    char* huf_filename, 
    char* decompressed_filename, 
    node_t* root);
void lutDecoding(                          //
    const char* huf_filename, 
    const char* decompressedLUT_filename,  
    lut all_luts[MAX_TABLES][MAX_ROWS_PER_TABLE], 
    const int barrel_shifter);

/***** Misc Helpers *****/
void printArr(
    int arr[], 
    int n);
int symbolIndex(
    uint8_t* symbols, 
    int symbol_count, 
    uint8_t symbol);
int prologue(
    const char* original_filename, 
    uint8_t* symbols, 
    float* probabilities, 
    uint32_t* sample_size);
void printSymbolEncodingBitified(
    huffCode_t* asciiToHuffman);
int maxTreeDepth(node_t* root);
void freeHuffmanTree(node_t* node);  // Free the memory allocated for the huffman tree recursively 

void custom_fwrite(             // Wrapper for fwrite to avoid problems with fwrite's interal buffer, that is implementation dependent 
    const void* array, 
    uint32_t element_size, 
    uint32_t total_elements, 
    FILE* file);

#endif