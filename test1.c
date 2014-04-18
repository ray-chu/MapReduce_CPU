#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct MapInputList {
	char* filename;
	struct MapInputList* next;
}MapInputList;

typedef struct MapReduceSpec{
	MapInputList* map_input_list;
}MapReduceSpec;

void add_input(char *path,MapReduceSpec* spec){
	MapInputList* plist;
	plist=(MapInputList*)malloc(sizeof(MapInputList));
	spec->map_input_list=plist;
	struct dirent* entry = NULL;
	DIR *pDir;
	pDir=opendir(path);
	while((entry=readdir(pDir))!=NULL){
		if(entry->d_type==DT_REG){
			plist->filename=(char*)malloc(strlen(path)+strlen(entry->d_name));
			strcpy(plist->filename,path);
		       	strcat(plist->filename,entry->d_name);
			plist->next=(MapInputList*)malloc(sizeof(MapInputList));
			plist=plist->next;
		}
	}
}

int main(int argc, char **argv){
	MapInputList* list_entry;
	MapReduceSpec spec;
	add_input(argv[1],&spec);
	list_entry=spec.map_input_list;
	while(list_entry->filename!=NULL){
		printf("%s \n",list_entry->filename);
		list_entry=list_entry->next;
	}
}
