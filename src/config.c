#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>


#include <bartlby.h>


char * getConfigValue(char * key, char * fname) {
	FILE * fp;
	char  str[1024];

	char * tok;
	
	fp=fopen(fname, "r");
	if(!fp) 
		return NULL;
	
	while(fgets(str,1024,fp) != NULL) {
		str[strlen(str)-1]='\0';
		tok=strtok(str, "=");
		if(tok != NULL) {
				if(strcmp(tok, key) == 0) {
						tok=strtok(NULL, "=");
						if(tok == NULL) {
								return NULL;
						}
						tok[strlen(tok)-1]='\0';
						return strdup(tok);
						
				} else {
					continue;
				}
					
		}
			
	}
	
	
	fclose(fp);
	
	
	return NULL;
}

