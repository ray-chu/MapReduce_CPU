#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct MapInputList {
	char* filename;
	struct MapInputList* next;
} MapInputList;

class MapReduceSpecification{

public:
	MapInputList* map_input_list;
	void add_input(char *path){
		MapInputList* plist;
		map_input_list=plist;
		struct dirent* entry = NULL;
		DIR *pDir;
		pDir=opendir(path);
		while((entry=readdir(pDir))!=NULL){
			if(entry->d_type==DT_REG){
				plist=(MapInputList*)malloc(sizeof(MapInputList));
				plist->filename=entry->d_name;
				plist->next=NULL;
				plist=plist->next;
			}
		}
	}
};

int main(int argc, char **argv){
	MapInputList* list_entry;
	MapReduceSpecification spec;
	list_entry=spec.map_input_list;
	while(list_entry->filename!=NULL){
		printf("%s \n",list_entry->filename);
		list_entry=list_entry->next;
	}
}
