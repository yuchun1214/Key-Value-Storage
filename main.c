#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "data.h"
#include "database.h"
#include "bloom_filter.h"
#include "btree.h"

int main(int argc, const char * argv[]){
	
	// basicFunction((char*)argv[1]);
	FILE * file = fopen(argv[1], "r");
	assert(file != NULL);
	unsigned char c_mode[100];
	unsigned long long int key1, key2;
	unsigned char * value = (unsigned char *)malloc(sizeof(char)*129);
	Data *data;

	B_tree * tree = createTree(MAX_TREE_SIZE);
	
	Data * gData = createData(0, NULL);
	
	unsigned int dataAmount = 0;

	while(fscanf(file, "%s", c_mode) != EOF){

		if(c_mode[0] == 'P'){ // PUT
			fscanf(file, "%llu", &key1);
			value = (unsigned char *)malloc(sizeof(char)*129); 
			fscanf(file, "%s", value);
			data = createData(key1, value);
			if(!putData(tree, data)){
				printf("OUTPUT\n");
				outputTree(0, tree); // should pass for metadata engin
				tree = createTree(MAX_TREE_SIZE);
				putData(tree, data);
			}
			++dataAmount;
		}else if(c_mode[0] == 'G'){ // GET
			fscanf(file, "%llu", &key1);
			gData->key = key1;
			gData = getData(tree, gData);
			if(gData->value == NULL){
				printf("EMPTY\n");
			}else{
				printf("%s\n", gData->value);
			}
			gData->value = NULL;
		}else{ // SCAN
			fscanf(file, "%llu %llu", &key1, &key2);
			for(unsigned long long int i = key1; i <= key2; ++i){
				gData->key = i;
				gData = getData(tree, gData);
				if(gData->value == NULL){
					printf("EMPTY\n");
				}else{
					printf("%s\n", gData->value);
				}
				gData->value = NULL;
			}
		}
	}	
	Output(tree);
	return 0;
}
