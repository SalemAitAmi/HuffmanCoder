// Includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "HuffmanHeader.h"



void printCodes(node_t* root, int arr[], int symbol_count, huffmanEncoding_t* HuffmanTable){
 
    // Assign 0 to left edge and recur
    if (root->leftChild) {
 
        arr[symbol_count] = 0;
        printCodes(root->leftChild, arr, symbol_count + 1, HuffmanTable);
    }
 
    // Assign 1 to right edge and recur
    if (root->rightChild) {
 
        arr[symbol_count] = 1;
        printCodes(root->rightChild, arr, symbol_count + 1, HuffmanTable);
    }
 
    // If this is a leaf node, then
    // it contains a symbol
    if (isLeaf(root)) {
        // Add the symbol and its encoding to a table
        HuffmanTable[(int) root->character].character = root->character;
        HuffmanTable[(int) root->character].bitValSize = symbol_count;
        // printf("%c:    %d\n", root->character, symbol_count);
        for(int y = 0; y < symbol_count; y++){
            HuffmanTable[(int) root->character].bitVal[y] = arr[y];
        }
    }
}

void generatehuffmanCodes(node_t* root, huffmanEncoding_t* HuffmanTable){
    // Print Huffman codes
    int arr[100];
    // Initialize indexes in table to matching ASCII symbol 
    for(int i = 0; i < MAX_SYMBOLS; i++){
        HuffmanTable[i].character = (char) i;
    }
 
    printCodes(root, arr, 0, HuffmanTable);  //added feature that creates Huffman Table
}


void printSymbolEncoding(char* symbols, int symbol_count,  huffmanEncoding_t* HuffmanTable){
    for(int i = 0; i < MAX_SYMBOLS; i++){
        // If a symbol exists in the sample text, then print it along with its Huffman encoding
        if(symbolIndex(symbols, symbol_count, HuffmanTable[i].character) != -1){
            printf("Symbol: %c    Encoding: ", HuffmanTable[i].character);
            for(int j = 0; j < HuffmanTable[i].bitValSize; j++){
                printf("%d",HuffmanTable[i].bitVal[j]);
            }
            printf("\n");
        }
    }

}

int generateEncodedText(char* filename, huffmanEncoding_t* HuffmanTable){
    // Sample file
    FILE* fpSample = fopen(filename, "r");
    if(fpSample == NULL){
        printf("Failed to open sample file!\n");
        exit(1);
    }
    // String manipulation for encoded filename
    char encodedFilename[64];
    strncpy(encodedFilename, strtok(filename, "."), 51);
    strncat(encodedFilename, "_encoded.txt", 13);

    // Encoded file
    FILE* fpEncoding = fopen(encodedFilename, "w");
    if(fpEncoding == NULL){
        printf("Failed to create encoding file!\n");
        exit(1);
    }
    // Buffers
    char sample[MAX_FILE_LENGTH];
    char encodedSample[MAX_ENCODING_LENGTH]; 
    int bitstoWrite = 0;
    int sample_size = fread(sample, sizeof(char), MAX_FILE_LENGTH, fpSample);

    for(int i = 0; i < sample_size; i++){
        for(int j = 0; j < HuffmanTable[(int) sample[i]].bitValSize; j++){
            encodedSample[bitstoWrite] = HuffmanTable[(int) sample[i]].bitVal[j];
            bitstoWrite++;
        }
    }

    int encodedSampleSize = fwrite(encodedSample, sizeof(char), bitstoWrite,  fpEncoding);
    
    fclose(fpSample);
    fclose(fpEncoding);

    return encodedSampleSize;
}

// Huffman decoding
void brute_decode_text(int* encodedText, huffmanEncoding_t* huffmanTable)
{
    // Get encoded Text Length
    printf("\n");
    int encoded_text_length = 0;
    for(int x = 0; encodedText[x] != -1; x++){
        printf("%d",encodedText[x]);
        encoded_text_length = x;
    }
    printf("\n test \n");    
    
    
    // Find longest code length  
    int code_length = 0;
    for(int j = 0; j < MAX_SYMBOLS; j++){
        if(code_length < huffmanTable[j].bitValSize){
            // add all the bit code values to array
            code_length = huffmanTable[j].bitValSize;
        }
    }

    // Test
    printf("longest code: %d", code_length);

    // For every symbol code pair in the table
    // check if the code exists in the code_length section
    //while(encodedText[0] != -1){
        for(int y = 0; y < MAX_SYMBOLS; y++){
            for(int i = 0; i < code_length; i++){
                if(huffmanTable[y].bitVal[i] != encodedText[i]){ // if no match
                    break;
                } else if(huffmanTable[y].bitValSize == i){ // if match char & length
                    printf("CHAR FOUND: %c", huffmanTable[y].character);
                    //char found so remove the code
                    for(int d = 0; (d+i) <= encoded_text_length; d++){
                        encodedText[d] = encodedText[d+i];
                    }
                } else { // if match char
                    //printf("match\n");
                }
            }    
        }
    //}
}

void tree_decodingRaw(char* filename, node_t* root, int size){
    FILE* fpEncoded = fopen(filename, "r");
    if(fpEncoded == NULL){
        printf("Failed to open encoded file!\n");
        exit(1);
    }
    
    node_t* cur = root;
    char encoded[size];
    char decodedFilename[64];
    strncpy(decodedFilename, strtok(filename, "_"), 50);
    strncat(decodedFilename, "_decoded.txt", 14);
    char decoded[size/8];
    int index = 0;

    FILE* fpDecoded = fopen(decodedFilename, "w");
    if(fpDecoded == NULL){
        printf("Failed to create decoded file!\n");
        exit(1);
    }

    fread(encoded, sizeof(char), size, fpEncoded);
    for(int i = 0; i < size;){
        if(!isLeaf(cur)){
            if(encoded[i] == (char) 0){
                cur = cur->leftChild;
            }
            else if(encoded[i] == (char) 1){
                cur = cur->rightChild;
            }
            i++;
        }
        else{
            decoded[index] = cur->character;
            index++;
            // printf("%c", cur->character);
            cur = root;
        }
    }
    decoded[index] = cur->character;
    index++;
    
    fwrite(decoded, sizeof(char), index, fpDecoded);
    fclose(fpEncoded);
    fclose(fpDecoded);
}

void tree_decodingV1(int* encoded, node_t* root, char* decoded){
    node_t* cur = root;
    int i = 0;

    while (*encoded != -1){
        if(!isLeaf(cur)){
            if(*encoded == 0){
                cur = cur->leftChild;
            }
            else if(*encoded == 1){
                cur = cur->rightChild;
            }
            encoded++;
        }
        else{
            // store decoded symbol
            decoded[i] = cur->character;
            i++;
            // Can be done up to a depth of m-1, where m = length of smallest code
            if(*encoded == 0){
                cur = root->leftChild;
                encoded++;
                if(*encoded == 0){
                    cur = root->leftChild;
                    
                }
                else{
                    cur = root->rightChild; 
                }
                encoded++;
            }
            else{
               cur = root->rightChild; 
               encoded++;
               if(*encoded == 0){
                    cur = root->leftChild;
                    
                }
                else{
                    cur = root->rightChild; 
                }
                encoded++;
            }
        }
    }
    decoded[i] = cur->character;
    i++;
}

int prologue(char* filename, char* symbols, float* probabilities, int* size){
    /* Retrieves sample text; Determines symbols and their probabilities; returns symbol_count */
    FILE* fp = fopen(filename, "r");
    char cur_symbol;
    int sample_size = 0;
    int symbol_count = 0;
    int frequencies[MAX_SYMBOLS];
    char sample[4096]; // Never NULL-terminated

    // Get the frequencies for every unique symbol in the sample text
    while(fread(sample, sizeof(char), 4096, fp)){
        for (int i = 0; i < 4096; i++){
            cur_symbol = sample[i];
            if (symbol_count == MAX_SYMBOLS){
                printf("Symbol count exceeded!\nAbort to avoid overflow!\n");
                exit(1);
            }
            else if(symbolIndex(symbols, symbol_count, cur_symbol) == -1){
                symbols[symbol_count] = cur_symbol;
                frequencies[symbol_count] = 1;
                symbol_count++;
            }
            else{
                frequencies[symbolIndex(symbols, symbol_count, cur_symbol)]  += 1;
            }
            sample_size++;
        }
    }
    *size = sample_size;
    float inv_sample_size = sample_size;
    // Precomputed for efficiency 
    inv_sample_size = 1/inv_sample_size;
    for (int i = 0; i < symbol_count; i++){
        probabilities[i] = frequencies[i]*inv_sample_size;
    }

    printf("Sample Size: %d bits\n", sample_size*8);
    printf("Symbol Count: %d\n", symbol_count);

    fclose(fp);
    return symbol_count;
}

int main(){
    // Encoding
    char filename[] = "sample3.txt";
    char symbols[MAX_SYMBOLS];
    float probabilities[MAX_SYMBOLS];
    int sample_size = 0;
    int symbol_count = prologue(filename, symbols, probabilities, &sample_size);
    huffmanEncoding_t HuffmanTable[MAX_SYMBOLS];

    node_t* huffmanRoot = buildHuffmanEncTree(symbols, probabilities, symbol_count);
    
    generatehuffmanCodes(huffmanRoot, HuffmanTable);
    
    int encodedSampleSize = generateEncodedText(filename, HuffmanTable);
    printf("Encoded Sample Size: %d bits\n", encodedSampleSize);
    
    printSymbolEncoding(symbols, symbol_count, HuffmanTable);

    // Decoding 
    clock_t start, end;
    char encfilename[] = "sample3_encoded.txt";

    start = clock();
    tree_decodingRaw(encfilename, huffmanRoot, encodedSampleSize);
    end = clock();
    printf("Time Elapsed Raw: %ld\n", end - start);


    return 0;
}