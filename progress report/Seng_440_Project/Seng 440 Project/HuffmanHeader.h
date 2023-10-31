// Macros
#define MAX_FILE_LENGTH 10000
#define MAX_ENCODING_LENGTH 25000
#define MAX_SYMBOLS 128 // ASCII
#define MAX_CHAR_CODE_LENGTH 16 


// Structs
typedef struct node_t {
    char character;
    float probability;
    struct node_t *leftChild, *rightChild;
} node_t;

typedef struct {
    node_t **treeArray;
    int numNodes;
    int maxNodes; 
} minHeap_t;

typedef struct {
    char character;
    int bitVal[MAX_CHAR_CODE_LENGTH];
    int bitValSize;
} huffmanEncoding_t;

// Functions 

/** Min Heap**/
node_t* allocateNewNode(char character, float probability){
    // Dynamically allocate memory for a new node 
    node_t* temp = (node_t*)malloc(sizeof(node_t));
    temp->leftChild = NULL;
    temp->rightChild = NULL;
    temp->character = character;
    temp->probability = probability;
    return temp;
}

minHeap_t* createMinHeap(int maxNodes){
    // Create minHeap object and allocate necessary memory  
    minHeap_t* minHeap = (minHeap_t*)malloc(sizeof(minHeap_t));
 
    minHeap->numNodes = 0;
    minHeap->maxNodes = maxNodes;
 
    minHeap->treeArray = (node_t**)malloc(minHeap->maxNodes * sizeof(node_t*));
    return minHeap;
}

void swapNodes(node_t** n1, node_t** n2){
    // Swap two nodes 
    node_t* temp = *n1;
    *n1 = *n2;
    *n2 = temp;
}

void heapify(minHeap_t* minHeap, int i){
    // Standard min-heapify  
    int minI = i;
    int leftI = 2*i+1;
    int rightI = 2*i+2;

    if (leftI < minHeap->numNodes && minHeap->treeArray[leftI]->probability < minHeap->treeArray[minI]->probability){
        minI = leftI;
    }
    
    if (rightI < minHeap->numNodes && minHeap->treeArray[rightI]->probability < minHeap->treeArray[minI]->probability){
        minI = rightI;
    }
 
    if (minI != i) {
        // Recurse 
        swapNodes(&minHeap->treeArray[minI],&minHeap->treeArray[i]);
        heapify(minHeap, minI);
    }
}

node_t* popMin(minHeap_t* minHeap){
    // Pop the min and heapify before returning
    node_t* temp = minHeap->treeArray[0];
    minHeap->treeArray[0] = minHeap->treeArray[minHeap->numNodes - 1];
 
    minHeap->numNodes--;
    heapify(minHeap, 0);
 
    return temp;
}

void insertMinHeap(minHeap_t* minHeap, node_t* newNode){
    // Insert a new node into the heap
    minHeap->numNodes++;
    int i = minHeap->numNodes - 1;
 
    while (i && newNode->probability < minHeap->treeArray[(i - 1) / 2]->probability) {
 
        minHeap->treeArray[i] = minHeap->treeArray[(i - 1) / 2];
        i = (i - 1) / 2;
    }
 
    minHeap->treeArray[i] = newNode;
}

void buildMinHeap(minHeap_t* minHeap){
    // Build heap
    int n = minHeap->numNodes - 1;
    int i;
 
    for (i = (n - 1) / 2; i >= 0; --i)
        heapify(minHeap, i);
}

int isLeaf(node_t* node){
    // Returns 1 if a node is a leaf; returns 0 otherwise
    return !(node->leftChild) && !(node->rightChild);
}

minHeap_t* initMinHeap(char alph[], float prob[], int size){
    // NOTE: If there are n symbols in our alphabet, 
    // then there are n-1 internal nodes for Huffman Coding, for a total of 2n - 1 nodes for a tree
    int numLeaves = size;
    minHeap_t* minHeap = createMinHeap(2*numLeaves - 1);
 
    for (int i = 0; i < size; i++){
        minHeap->treeArray[i] = allocateNewNode(alph[i], prob[i]);
    }
 
    minHeap->numNodes = numLeaves;
    buildMinHeap(minHeap);
 
    return minHeap;
}

/** Huffman Encoding**/

node_t* buildHuffmanEncTree(char alph[], float prob[], int size){
    // Construct a Huffman Coding Tree for the provided alphabet 
    node_t *left, *right, *top;
 
    // Create, initialize, and build the min heap 
    minHeap_t* minHeap = initMinHeap(alph, prob, size);
 
    // Iterate while size of heap doesn't become 1
    while (!(minHeap->numNodes == 1)) {
 
        // Extract the two minimum probability nodes
        left = popMin(minHeap);
        right = popMin(minHeap);

        // Create new node: Internal nodes identified by '~', set child pointers 
        top = allocateNewNode('~', left->probability + right->probability);

        top->leftChild = left;
        top->rightChild = right;
 
        insertMinHeap(minHeap, top);
    }
 
    // Pop last node
    return popMin(minHeap);
}


/** Helper **/
void printArr(int arr[], int n){
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", arr[i]);
 
    printf("\n");
}

int symbolIndex(char* symbols, int symbol_count, char symbol){
    // If a symbol exists in symbols, return its index
    // Otherwise return -1
    for (int i = 0; i < symbol_count; i++){
            if (symbols[i] == symbol){
                return i;
            }
        }

    return -1;
}