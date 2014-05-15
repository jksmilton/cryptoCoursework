#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "qThree.h"
#include <string.h>

int main(int argc, char* argv[])
{
	char* content = ReadFile("secret.hex");
	printf("%s\n", content);
	char* key = GetKey(content);
	printf("Key = %s\n", key);

	char* intermediateForm = XOR(content, key);
	char* crnt = intermediateForm;
	while(*crnt != '\0')
	{
		printf("%c", *crnt);
		crnt++;
	}

}

//Stack overflow
char* ReadFile(char *filename)
{
   char *buffer = NULL;
   int string_size,read_size;
   FILE *handler = fopen(filename,"r");

   if (handler)
   {
       //seek the last byte of the file
       fseek(handler,0,SEEK_END);
       //offset from the first to the last byte, or in other words, filesize
       string_size = ftell (handler);
       //go back to the start of the file
       rewind(handler);

       //allocate a string that can hold it all
       buffer = (char*) malloc (sizeof(char) * (string_size + 1) );
       //read it all in one operation
       read_size = fread(buffer,sizeof(char),string_size,handler);
       //fread doesnt set it so put a \0 in the last position
       //and buffer is now officialy a string
       buffer[string_size+1] = '\0';

       if (string_size != read_size) {
           //something went wrong, throw away the memory and set
           //the buffer to NULL
           free(buffer);
           buffer = NULL;
        }
    }

    return buffer;
}

char* GetKey(char* content)
{
	char* crib = "Hg";

	char* key = (char*) malloc(3 * sizeof(char));

	key[0] = crib[0] ^ content[0];
	key[1] = crib[1] ^ content[1];
	key[2] = '\0';
	return key;
}

char* XOR(char* content, char* key)
{
	int length = strlen(content);
	char* newText = (char*) malloc(length * sizeof(char));
	int tmp;

	for(int i = 0; i < length; i++)
	{
		tmp = (i % 2);
		newText[i] = content[i] ^ key[tmp];
	}
	return newText;
}