#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printLinkToRoom(const char*);

//header of room, including greeting, room image, NSEW buttons, and submit button, and hidden resources
void printHeader(){
	printf( "Content-type: text/html\n\n");
	printf( "<!doctype html>\n");
	printf( "<html>\n");
	//styling format
	printf("<style>\n h1 { \n	color: green;\n}\niframe{\n	boarder:none;\n	outiline: none;\n}\n</style>\n");
	//greeting
	printf( "<h1><center>Welcome!</center></h1>\n");
	//Room image
	printf("<body>\n		<center><img src=\"../room.jpg\" width=\"900px\" height=\"500px\"/>\n");
	//reading the player's inventory
	FILE * file;
	int mannaI; 
	int goldI;
	file = fopen("inventory.csv","r");
	fscanf(file, "%d,%d",&mannaI, &goldI);
        fclose(file);
	//formatting and adding the NSEW buttons and submit button and hidden inventory  of player 
	printf("<section>\n");
	printf("	<form action='%s' method='get'>\n ", "http://cs.mcgill.ca/~yhe54/cgi-bin/transporter.py\n");
	printLinkToRoom("North");
	printf("<table><tr>\n");
	printf("	<td><form action='%s' method='get'>\n ", "http://cs.mcgill.ca/~sli144/cgi-bin/transporter.py\n");
	printLinkToRoom("West");
	printf("	</td>\n");

	printf( "		<td><form action=\"./room.cgi\"  method=\"get\">\n");
	printf("	<input type=\"text\" name=\"command\" placeholder=\"Enter your command\"/>\n		<input type=\"submit\" text=\"asdf\"/>\n");
	printf("	<input type=\"hidden\" name=\"inventory\" value=\"%d,%d\">\n", mannaI, goldI); 
	printf("	</form></td>\n");
	printf("	<td><form action='%s' method='get'>\n ", "http://cs.mcgill.ca/~nladd/cgi-bin/transporter.py\n");
	printLinkToRoom("East");
	printf("	</td>\n </table></tr>\n");

	printf("	<form action='%s' method='get'>\n ", "http://cs.mcgill.ca/~smonfo1/cgi-bin/transporter.py\n");
	printLinkToRoom("South");
}

//ending the html region
void printFooter() {
	printf("</section>\n</body>\n");	
	printf("</html>\n");
}

//method for printing direction buttons
void printLinkToRoom(const char* direction) {
	FILE * file;
	int mannaI;
	int goldI;
	file = fopen("inventory.csv","r");
	fscanf(file, "%d,%d", &mannaI, &goldI);
	fclose(file);
	printf("	<input type='submit' value='%s'/>\n",direction); 
	printf("	<input type='hidden' name='inventory' value='%d,%d'/>\n",mannaI, goldI);
	printf("	<input type='hidden' name='URL' value='http://cs.mcgill.ca/~mshimi2/cgi-bin/room.cgi'/>\n");
	printf("</form>");
}

//sending player to the game page
void printGame(){
	printf("Location: ./game.py \n\n");
}


void printMain(){
	printHeader();
	printFooter();
}

//method for DROP n command
void drop(int n){
	
	int mannaR;
	int goldR;
	int mannaI;
	int goldI;
	FILE * file;
	printHeader();
	if(n<0){
		printf("Error: Cannot drop negative gold");
		printFooter();
		return;
	}
	
	//reding contents of inventory
	file = fopen("inventory.csv", "r");
	fscanf(file, "%d , %d" , &mannaI, &goldI); 
	fclose(file);
	if(goldI < n){
		printf("Transaction failed: You do not have enough gold");
		printFooter();
		return;
	}
	else{
		//succesful input, gold is taken from player and manna added to player
		goldI -= n;
		mannaI += n/2;
	}	
	//writing contents to inventory 
	file = fopen("inventory.csv", "w");
	fprintf(file,"%d, %d", mannaI, goldI);
	fclose(file);
	//Reading contents of resources
	file = fopen("resources.csv", "r");
	if (file == NULL) {
		printf("ggg");
		return;
	}
	fscanf(file, "%d, %d", &mannaR , &goldR);
	fclose(file);
	//n gold added to room 
	goldR += n;
	//writing contents to resources
	file = fopen("resources.csv", "w");
	if(file == NULL){
		printf("hhh");
		return;
	}
	//rewrite resources to room 
	fprintf(file, "%d, %d, 1", mannaR, goldR);
	fclose(file);
	printf("Success! %d manna gained.\n" , n/2); 
	printFooter();
}

//REFRESH command method
void refresh(){
	printMain();
}

//EXIT method 
void exitRoom(){

	printf( "Content-type: text/html\n\n");
	printf( "<!doctype html>\n");
	printf("<html>\n");
	printf("<style>\n");
	printf("h1 { \n 	color: blue; \n }");
	printf("</style>\n");
	printf("<center><h1>You have left the room. See you again soon!</h1></center>\n");
	printf("</html>");

}

//player's inventory is added to the room's inventory, and player's inventory set to 1, 0 
void dumpInventory(){
	FILE* file;
	int mannaI;
	int goldI;
	int mannaR;
	int goldR;
	//read what's in the inventory
	file = fopen("inventory.csv", "r");
	fscanf(file,"%d, %d", &mannaI, &goldI);
	fclose(file);
	//read what's in the resources
	file = fopen("resources.csv" , "r");
	fscanf(file,"%d, %d", &mannaR, &goldR);
	fclose(file);
	//move inventory to resources and reset inventory
	mannaR += mannaI;
	goldR += goldI;
	mannaI = 1;
	goldI = 0;
	//print content to resourvces
	file = fopen("resources.csv" , "w");
	fprintf(file, "%d, %d, 0" , mannaR, goldR);
	fclose(file);
	//print content to inventory
	file = fopen("inventory.csv", "w");
	fprintf(file, "%d, %d", mannaI, goldI);
	fclose(file);
}

int main(int argc, char* argv[]){
	//getting environment string
	char* parameters = getenv("QUERY_STRING");
	char command[64];
	int mannaI;
	int goldI;
	int n = 0;
	//scanning parameters for n of DROP if exists  
	sscanf(parameters,"command=DROP+%d",&n);
	if(parameters != NULL && parameters[0] != '\0'){
		if(sscanf(parameters,"command=%s", command ) != 1) {
				}
	}
	else{
		//if starting game (entering ~/room.cgi), we write inventory to 1,0 and room as occupied
		int mannaR;
		int goldR;
		FILE* file;
		//reading the resources
		file = fopen("resources.csv" ,"r");
		fscanf(file, "%d,%d", &mannaR, &goldR);
		fclose(file);
		//putting back resources and making room occupied
		file = fopen("resources.csv","w");
		fprintf(file, "%d,%d,1", mannaR, goldR);
		fclose(file);

		file = fopen("inventory.csv","w");
		fprintf(file, "1,0");
		fclose(file);

	}
	//play the game
	if(strncmp(command,"PLAY", 4) == 0){
		printGame(command);
	}
	//drop n gold for n/2 manna 
	else if(strncmp(command,"DROP",4) == 0){
		drop(n);
	}
	//refresh page with resources in url 
	else if(strncmp(command,"REFRESH", 7) == 0 ){
		refresh();
	}
	//exit, give room inventory, and leave room empty 
	else if(strncmp(command,"EXIT", 4) == 0){
		exitRoom();
		dumpInventory();
	}
	else if (strncmp(command,"asdfghjkloiuytre", 10) != 0){
		printHeader();
		//printing command for player's knowledge 
		printf("Valid commands: 'PLAY', 'DROP n' , 'REFRESH', and 'EXIT'");
		printFooter();
	}
	else {
		printMain();
		
	}
	return 1;
}

