#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct MapInputList {
	char* filename;
	struct MapInputList* next;
}MapInputList;

typedef struct MapReduceSpec{
	MapInputList* map_input_list;
	unsigned int input_size;
	enum InputFormat{TextInputFormat,KeyValueInputFormat,SequenceFileInputFormat} input_format;
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
			plist->filename=(char*)malloc(strlen(path)+strlen(entry->d_name)+1);
			strcpy(plist->filename,path);
		       	strcat(plist->filename,entry->d_name);
			plist->next=(MapInputList*)malloc(sizeof(MapInputList));
			/* printf("%s\n",entry->d_name);	 */
			/* printf("%s\n",plist->filename);	 */
			plist=plist->next;
		}
	}
}

void map_input_split(MapReduceSpec* spec){
	MapInputList* list_entry;
	size_t buffer_size=(size_t)256*1024*1024;
	/* unsigned int num_data_block_allow=(unsigned)256/spec.input_size; */
	/* size_t input_size=(size_t)spec.input_size*1024*1024; */
	/* unsigned int data_block_filled=0; */
	size_t buffer_used=0;
	char* buffer;
	buffer=(char*)malloc(buffer_size);
	if (buffer == NULL) {fputs ("Memory error\n",stderr); exit (2);}

	struct stat st;
	FILE* pFile;
	list_entry=spec->map_input_list;
	while(list_entry->filename!=NULL){
		stat(list_entry->filename, &st);
		if(buffer_used+st.st_size<=buffer_size){
			pFile=fopen(list_entry->filename,"rb");
//			printf("%u, %u\n",st.st_size,buffer_used);		
			if (pFile==NULL) {fputs ("File error\n",stderr); exit (1);}
			size_t result;
			result = fread (buffer+buffer_used,1,st.st_size,pFile);
			buffer_used=buffer_used+st.st_size;
			if (result != st.st_size) {fputs ("Reading error\n",stderr); exit (3);}
		}
		else
			printf("Buffer full!!\n");
		list_entry=list_entry->next;
		fclose(pFile);
	}
	printf("%s \n",buffer);
	free(buffer);
}

int main(int argc, char **argv){
	MapReduceSpec spec;
	add_input(argv[1],&spec);
	//printf("\n\n");
	map_input_split(&spec);
}
