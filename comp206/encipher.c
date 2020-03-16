  #include <stdio.h>
#include <stdlib.h>
#include <string.h>

//introducing variables used in main
int getFileSize(char*);
void encrypt(char[], int);
void writeToFile(char[], char[]);

int main(int argc, char* argv[]){
	char *filename = argv[1]; //getting the file name from first argument
//the size of the file is found and an array of the characters of the input file is created named text
	int size = getFileSize(filename);
	char text[size+1];
	text[size] = '\0';
	int c;
	FILE *file;
	file = fopen(argv[1],"r");
	if (file){
		int i = 0;
		while ((c = getc(file)) != EOF){
			text[i] =  c;
			i++;
		}
	}	
	int key = atoi(argv[2]); //second input is the key
	encrypt(text, key);
	writeToFile(text, filename);
}

//encryption method, left shift key amount 
void encrypt(char text[], int key){
	for (int i=0; i < strlen(text); i++){
		if (text[i] == ' '){
			text[i] = text[i];
		} 
		else if ((text[i]>=65)&&(text[i]<=90)){ //for upper case letters
			if (text[i]-key < 65){
				text[i] = text[i] - key + 26;
			}
			else{
				text[i] = (text[i] - key);
			}
		}
		else if((text[i]>=97)&&(text[i]<=122)){ //for lower case letters
			if (text[i]-key < 97){
				text[i] = text[i] - key + 26;
			}
			else{
				text[i] = (text[i] - key);
			}
		}
	
	}
}

//returns the size of the input file. If file doesn't exist, returns -1
int getFileSize(char filename[]) {
	FILE *file;
	file = fopen(filename, "r");
	int count = 0;
	if (file) {
		while (1) {
			char c = getc(file);
			if (c == EOF){
				break;
			}
			else{
				count = count + 1;
			}
		}
	}
	else {
		return -1;
	}
	return count;	
}

//method that writes an array to a file
void writeToFile(char text[], char filename[]){
	FILE *file;
	file = fopen (filename, "w");
	fputs (text, file);
	fputs ("\0", file);
	fclose(file);
}

