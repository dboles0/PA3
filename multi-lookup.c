#include "multi-lookup.h"
// open file in main and pass the pointer to producer 
// to start this process. it then will give it a thread/file name 
// it will read the file then proccess it to the queue
// this will complete then exit the main program

void * runner(void *param){
	//set up local variables 
	long * tid = param;
	long t; 
	long numprint = 3;

	// print hello param times
	for(t=0; t<numprint; t++){
		printf("Hello World! thread[%ld]", *tid);
		printf("this is printout %ld of %ld\n", t+1, numprint);
		// sleep for 1 to 2 seconds
		usleep((rand()%100)*10000+1000000);
	}
	pthread_exit(0);
}

/*
// get the file names in a spicific directory
void get_file_names(char * update_file_names[], int * a_num_files, int * a_buffer_size){

	struct dirent *dir; 
	DIR *d = opendir("input/");
	int num_files = 0, i = 0, buffer_size = 0;
		
	buffer_size = *a_buffer_size;


	while((dir = readdir(d)) != NULL){
		if(!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..")){}
		else {	
			num_files++;
		}
	}
	rewinddir(d);
	
	// crate temp arry to campture all 
	*a_num_files = num_files;

	if(d){

		while((dir = readdir(d)) != NULL){


			// skip (.) and (..) 
			if(!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..")){}
			else {	
				update_file_names[i] = (char*) malloc (strlen(dir->d_name)+1); 
				strncpy (update_file_names[i], dir->d_name, strlen(dir->d_name));
				i++;
			}
		}
		*a_buffer_size = buffer_size;
		closedir(d);

	}
	else{	
		printf("erro - directory open");
	}
}
*/
