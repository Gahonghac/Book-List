#include <stdio.h>
#include <String.h>
#include <ctype.h>
#define Max 100
#define MaxTitle 60


struct book{
	char id[8];
	char title[61];
	int publishYear;
	int quantity;
};

int isEmpty(int spt){
	return (spt==0);
}

int isFull(int spt){
	return spt==Max;
}

void printMenu()
{
	int choice;
	printf("\n\n\n=============***** MENU BOOK LIST *****===================");
	printf("\n| 1. Add a new book into the book list.         Press: 1 |");
	printf("\n| 2. Show all book in book list.                Press: 2 |");
	printf("\n| 3. Search a book by book title.               Press: 3 |");
	printf("\n| 4. Update an existing book.                   Press: 4 |");
	printf("\n| 5. Remove a book.                             Press: 5 |");
	printf("\n| 6. Save the list of the book to file.         Press: 6 |");
	printf("\n| 7. Read from file.                            Press: 7 |");
	printf("\n| 8. Exit the program.                          Press: 8 |");
	printf("\n==========================================================\n");
}

char* lTrim(char s[]){
	int i=0;
	while(s[i]==' ') i++;
	if(i>0) strcpy(&s[0], &s[i]);
	
	return s;
}

char* rTrim(char s[]){
	int i=strlen(s)-1;
	while(s[i]==' ') i--;
	s[i+1] = '\0';
	return s;
}

char* trim(char s[]){
	lTrim(rTrim(s));
	char* str = strstr(s, "  ");
	while(str!=NULL){
		strcpy(str, str+1);
		str = strstr(s, "  ");
	}
	
	return s;
}

char* nameStr (char s[])
{
	trim(s);
	strlwr(s);
	int L = strlen(s);
	int i;
	for (i = 0; i < L; i++)
		if (i == 0 || (i > 0 && s[i-1] == ' '))
			s[i] = toupper(s[i]);
	return s;
}

int getPositiveInteger(char announce[], char error[]){
	int check, number;
	char ch;

	fflush(stdin);	
	printf(announce);
	check = scanf("%d%c", &number, &ch);
	
	while(number<=0 || ch!='\n' || check!=2){
		fflush(stdin);
		printf(error);
		printf(announce);
		check = scanf("%d%c", &number, &ch);	
	}	
	
	return number;
}

int getLimitInteger(char announce[], char error[], int firstLimit, int endLimit){
	int check, number;
	char ch;
	
	if(firstLimit>endLimit){
		int temp = firstLimit;
		firstLimit = endLimit;
		endLimit = temp;
	}
	fflush(stdin);	
	printf(announce);
	check = scanf("%d%c", &number, &ch);
	
	while(number<0 || ch!='\n' || check!=2 || number>endLimit || number<firstLimit){
		fflush(stdin);
		printf(error);
		printf(announce);
		check = scanf("%d%c", &number, &ch);	
	}	
	
	return number;	
}

char* getString(char announce[], char temp[]){
	fflush(stdin);
	printf(announce);
	scanf("%[^\n]", temp);	
	trim(temp);
	return temp;      
}

int getOption(char announce[], char error[]){
	char option[Max];
	printf(announce);
	fflush(stdin);
	scanf("%[^\n]", option);
	trim(option);
	strlwr(option);
	
	while(strlen(option)!=1 || (strcmp(option, "y")!=0 && strcmp(option, "n")!=0) ) {
		printf(error);
		printf(announce);
		fflush(stdin);
		scanf("%[^\n]", option);		
	}
	
	if(strcmp(option, "y")==0) return 1;
	return 0;
}
//============================================================================================

int checkExistId(struct book bookList[], int spt, char temp[]){
	for(int i=0; i<spt; i++){
		if(strcmp(bookList[i].id, temp) == 0)
			return i;
	}
	return -1;
}

int checkInputId(struct book bookList[], int spt, char id[]){ //Id: se12345
	if(strlen(id)!=7) return 0; //is not enough length
	for(int i=0; i<2; i++)
		if(id[i]<'A' ||  (id[i]>'Z' && id[i]<'a') || id[i]>'z')
			return 0; //wrong format!
	for(int i=2; i<7; i++)
		if(id[i]<'0' || id[i]>'9') 
			return 0; //wrong format!
	if(checkExistId(bookList, spt, id) != -1) 
		return -1; //Id is existed! 
	return 1;
}

int getId(struct book bookList[], int spt, char id[]){
	int option=1;
	
	getString("\nInput Id Book (AA-DDDDD):  ( A: any alphabet || D: any digit ): ", id);
	int check = checkInputId(bookList, spt, id);
	
	while(check!=1 && option){
		if(check == 0 || check==-1){
			if(check ==0)
				printf("\n!!! Wrong format Id! Just input (AA-DDDDD):  ( A: any alphabet || D: any digit )\n");	
			if(check == -1)
				printf("\n!!! Error! This Id is existed!\n");			
			option = getOption("\nY/y to continue or N/n to cancle: ","\n!!!Just input Y/y or N/n\n"); 
			if(option == 0) return 0; // Get id That bai
		}
		
		getString("\nInput Id Book (AA-DDDDD):  ( A: any alphabet || D: any digit ):  ", id);
		check = checkInputId(bookList, spt, id);
	}
	return 1;//get Id thanh cong
}

int checkTile(char title[]){
	if(strlen(title) > MaxTitle) return -1; //over 60 word 
	for(int i=0; i<strlen(title); i++)
		if((title[i]<'0' && title[i]!=' ') || (title[i]<'A'&&title[i]>'9') ||  (title[i]>'Z' && title[i]<'a') || title[i]>'z') 
			return 0; // especial words in title
	return 1;
}

void getTitle(struct book bookList[], int spt,char title[]){
	strcpy(title, nameStr(getString("\nInput Title Book: ", title)));
	int check = checkTile(title);
	while(check!=1){
		if(check == -1)
			printf("\n!!! Wrong Title! This title is over 60 word!\n");
		if(check == 0)
			printf("\n!!! Wrong Title! Title book only contains a-z, A-Z, 0-9!\n");
		strcpy(title, nameStr(getString("\nInput Title Book: ", title)));
		check = checkTile(title);
	}
}

void addNewBook(struct book bookList[], int* spt){
	if(isFull(*spt)){
		printf("\n=>The Book List is full!\n");
		return;
	}
	
	int check;
	char idBook[7+1], titleBook[Max+1];
	int publishYearBook, quantityBook;
	//getId 
	check = getId(bookList, (*spt), idBook);
	if(check == 0){
		printf("\n=>Adding book fairly!\n");
		return;
	}
	//get title	
	getTitle(bookList, (*spt), titleBook);
	//get publish year	
	publishYearBook = getLimitInteger("\nInput Published Year Book: ",
									"\n!!! Wrong input! Just input a positive integer from 1450 to 2022!\n"
									,1450, 2022);
	//get quantity
	quantityBook = getPositiveInteger("\nInput Quantity Book: ",
									"\n!!! Wrong input! Just input a positive integer!\n");
	
	int option = getOption("\nY/y to adding Book or N/n to cancle: ","\n!!!Just input Y/y or N/n\n"); 
	
	if(option){
		strcpy(bookList[(*spt)].id, idBook);
		strcpy(bookList[(*spt)].title, titleBook);
		bookList[(*spt)].publishYear = publishYearBook;
		bookList[(*spt)].quantity = quantityBook;
		(*spt)++;
		printf("\n=> Adding a new book successfully!\n");		
	}
	else
		printf("\n=> Adding a new book is cancled!\n");
}
         
int handleId(struct book bookList[], int spt, char id[]){                     
	int option=1, check;
	
	do{
		strcpy(id, getString("\nInput Id Book to Search (AA-DDDDD):  ( A: any alphabet || D: any digit ): ", id));
		check = checkInputId(bookList, spt, id); 
		if(check == 1 || check ==0){
			if(check == 1)	
				printf("\n!!! Error! This Id is not existed!\n");
			if(check == 0)
				printf("\n!!! Wrong format Id! Just input (AA-DDDDD):  ( A: any alphabet || D: any digit ): \n");
			option = getOption("\nY/y to continue or N/n to go to Menu: ","\n!!!Just input Y/y or N/n\n"); 
		}			
	}while(option && (check == 1 || check ==0));

	return option; 
}

void searchBookById(struct book bookList[], int spt){
	if(isEmpty(spt)){
		printf("\n!!! The Book List is empty!!!\n");
		return ;
	}
	
	char id[Max];
    int check = handleId(bookList, spt, id);
    if(!check){
    	printf("\nSearch a Book by Id is cancled!\n");
    	return;
	}
	int pos = checkExistId(bookList, spt, id);
	printf("\n===================================");
	printf("\n=>The Book Id %s:\n\n", bookList[pos].id);
	printf("-Title Book: %s\n-Quantity Book: %d\n", 
					bookList[pos].title, bookList[pos].quantity);
	printf("===================================\n\n");
}

int checkUpdateTitleBook( char title[]){
	printf("\nInput updated Title Book: ");
	fflush(stdin);
	gets(title);
	nameStr(title);
	if(strcmp(title, "") == 0) return 0; // khong nhap gi ca
	int check = checkTile(title);
	while(check!=1){
		if(check == -1)
			printf("\n!!! Wrong Title! This title is over 60 word!\n");
		if(check == 0)
			printf("\n!!! Wrong Title! Title book only contains a-z, A-Z, 0-9!\n");
		printf("\nInput updated Title Book: ");
		
		fflush(stdin); 	gets(title);
		nameStr(title);
		
		if(strcmp(title, "") == 0) return 0; //khong thay doi Title book
		check = checkTile(title);
	}	
	return 1; // xuat hien 1 Title moi
}

int getNumber(char number[]){
	int num = 0;
	for(int i=0; i<strlen(number); i++)
		if(number[i]<'0'|| number[i]>'9') 
			return -1;
		else
			num = num*10 + number[i] - '0';
				
	return num;
}
int checkUpdateNumber(char announce[], char error[], char option[]){
	char number[Max];
	printf(announce);
	gets(number);
	trim(number);
	if(strcmp(number, "")==0) return 0; //khong thay doi number
	
	int num = getNumber(number);
	while((num==-1) || 
	((num<1450||num>2022)&&strcmp(option,"publishYear")==0) || (num<=0&&strcmp(option,"quantity")==0) ){
		printf(error);
		printf(announce);
		gets(number);
		trim(number);
		num = getNumber(number);
		if(strcmp(number, "")==0) return 0;	//khong thay doi number
	}
	return num; //xuat hien 1 number moi		
}

void updateBook(struct book bookList[], int spt){  
	if(isEmpty(spt)){
		printf("\n!!! The Book List is empty!!!\n");
		return ;
	}
	
	char id[7+1], title[MaxTitle+1];
    int publishYear, quantity;
	int check = handleId(bookList, spt, id);	
	if(!check){
    	printf("\nSearch a Book by Id is cancled!\n");
    	return;
	}
	
	int pos = checkExistId(bookList, spt, id);
	
	//Title Book
	check = checkUpdateTitleBook(title);
	if(check == 0) strcpy(title, bookList[pos].title);
	
	//Publish Year Book
	check =	checkUpdateNumber("\nInput Updated Publish Year Book: ", 
			"\n!!!Wrong input! Publish Year just from 1450 to 2022!\n", "publishYear");  
	if(check == 0)
		publishYear = bookList[pos].publishYear;
	else 
		publishYear = check;
		
	//Quantity Book
	check =	checkUpdateNumber("\nInput Updated Quantity Book: ", 
			"\n!!!Wrong input! Quantity is only a positive integer!\n", "quantity");   
	if(check == 0)
		quantity = bookList[pos].quantity;
	else 
		quantity = check;
	
	if(getOption("\nDO you want to update now? Y/y = yes || N/n = no\nYour choice: ", 
	"\n!!!Wrong input! Just input Y/y(yes) or N/n(no)!\n")){
		strcpy(bookList[pos].title, title);
		bookList[pos].publishYear = publishYear;
		bookList[pos].quantity = quantity;
		printf("\n=> Updating Book successfully!\n");
	}	
	else{
		printf("\n=> Updating Book is cancled!\n");
	}	                                                  
}    

void removeBookById(struct book bookList[],int spt, int pos){
	for(int j=pos; j<spt-1; j++){
		strcpy(bookList[j].id, bookList[j+1].id);	
		strcpy(bookList[j].title, bookList[j+1].title);	
		bookList[j].publishYear = bookList[j+1].publishYear;
		bookList[j].quantity = bookList[j+1].quantity;		
	}
}

void removeBook(struct book bookList[], int* spt){
	if(isEmpty(*spt)){
		printf("\n!!! The Book List is empty!!!\n");
		return ;
	}	
	
	char id[7+1];
                         
	int option, check;
	
	do{
		strcpy(id, getString("\nInput Id Book to search (AA-DDDDD):  ( A: any alphabet || D: any digit ): : ", id));
		check = checkInputId(bookList, *spt, id); 
		if(check == 1 || check ==0){
			if(check == 1)	
				printf("\n!!! Error! This Id is not existed!\n");
			if(check == 0)
				printf("\n!!! Wrong format Id! Just input (AA-DDDDD):  ( A: any alphabet || D: any digit )! \n");
			option = getOption("\nY/y to continue or N/n to go to Menu: ","\n!!!Just input Y/y or N/n\n"); 
		}			
	}while(option && (check==1 || check ==0));
	int pos = checkExistId(bookList, *spt, id);	
	removeBookById(bookList, *spt, pos);
	(*spt)--;	
	printf("\n=> Removing book succesfully!\n");
}      

void printBook(struct book bookList[], int pos){
	printf("\n============================================\n");
	printf("- Id Book: %s\n",bookList[pos].id);	
	printf("- Title Book: %s\n",bookList[pos].title);	
	printf("- Publish Year Book: %d\n",bookList[pos].publishYear);	
	printf("- Quantity Book: %d\n",bookList[pos].quantity);
	printf("============================================\n");		
}

void printAllBook(struct book bookList[], int spt){
	if(isEmpty(spt)){
		printf("\n!!! The Book List is empty!!!\n");
		return ;
	}
	
	printf("\n******* All Book *******\n");
	for(int i=0; i<spt; i++)
	printBook(bookList, i);
	printf("\n\n");
}                             

int existFile(char* fileName){
	int existed = 0;
	FILE * f = fopen(fileName, "r");
	if (f != NULL){
		existed = 1;
		fclose(f);
	}
	return existed;
}

int getData(struct book bookList[], int *spt, char line[]){
	int num;
	lTrim(rTrim(line));	
	
	char* token = strtok(line, "-"); 
	int check = checkInputId(bookList, *spt, token);
	if(check == 1)	
		strcpy(bookList[*spt].id, token);
	else {
		if(check ==0)
			printf("\n!!! Error! (A/Some) Id Book in file is wrong format Id! Format ID (AA-DDDDD):  ( A: any alphabet || D: any digit )!  \n");	
		if(check == -1)
			printf("\n!!! Error! (A/Some) Id Book in file is existed!\n");	
		return 0;	
	}
	
	token = strtok(NULL, "-");	
	check = checkTile(token);
	if(check == 1) 	strcpy(bookList[*spt].title, token);
	else{
		if(check == -1)
			printf("\n\n!!! Error! (A/Some) Title Book in file is over 60 word!\n");
		if(check == 0)
			printf("\n!!! Error! (A/Some) Title Book in file is wrong\n");	
		return 0;
	}
	
	token = strtok(NULL, "-");
	num = getNumber(token);		
	if(num<=2022 && num>=1450)bookList[*spt].publishYear = num;	
	else{
		printf("\n!!! Error! Publish Year Book in file is wrong\n");
		return 0;		
	}
	
	token = strtok(NULL, "-");
	num = getNumber(token);	
	if(num>0)bookList[*spt].quantity = num;	
	else{
		printf("\n!!! Error! Quantity Book in file is wrong\n");
		return 0;		
	} 
	(*spt)++;
	
	return 1;
}

void readFile(struct book bookList[], int *spt){
	
	char fileName[Max];
	printf("***Program for reading a file***\n");
	printf("Input a file name: ");
	gets(fileName);
	
	if(existFile(fileName)!=1) {
		printf("\n!!! Error! The file ""%s"" dose not exist!\n", fileName);
		return;
	}
	
	if(!getOption("\nThe file is existed. Do you want to read it? (Y/y or N/n): ", "\nJust input Y/s or N/n\n")){
		printf("\n=> Reading to file is cancled!!!\n");
		return;		
	}
	
	char line[105];
	
	FILE* f = fopen(fileName, "r");

	while(fscanf(f, "%[^\n]", line)!=EOF){ 
		fgetc(f);  

		if(getData(bookList, spt, line) == 0){
			printf("!!! Error! Data in file is wrong!\n");
			return;			
		}
	}
	
	fclose(f);
	printf("\n=> Reading file successfully!\n");
}

void saveToFile(struct book bookList[], int spt) 
{	
	if(isEmpty(spt)){
		printf("\n!!! The Book List is empty!!!\n");
		return;
	}
	
	char fileName[Max];
	printf("***Program for writing a file***\n");
	printf("Input a file name: ");
	gets(fileName);
	
	if(existFile(fileName)!=1) {
		printf("\n!!! This file name is not existed!!!\n");
		return;
	}
	
	if(!getOption("\nThe file is existed. Do you want to override it? (Y/y or N/n): ", "\nJust input Y/s or N/n\n")){
		printf("\n=>Saving to file is cancled!!!\n");
		return;		
	}
		
	char line[Max+1];
	int length=0;
	FILE* f = fopen(fileName, "w");
	
	for(int i = 0; i<spt; i++){
		fprintf(f,"%s-", bookList[i].id);
		fprintf(f,"%s-", bookList[i].title);
		fprintf(f,"%d-", bookList[i].publishYear);
		fprintf(f,"%d\n", bookList[i].quantity);
	}
	
	fclose(f);
	printf("\n=> This list has been saved successfully!\n");
}

void printEnd(){
	printf("\n\n\n===================================");
	printf("\n\n   << THANKS FOR YOUR USING >>\n\n");
	printf("       [ Have a nice day ]\n");
	printf("===================================\n\n");	
}

void process(struct book bookList[], int *spt){
	int check;	

	do{
		printMenu();
		check = getLimitInteger("\nInput your choice (from 1 to 8): ", 
		"\n!!! Wrong input! Just input a integer from 1 to 8!\n", 1, 8);

		switch(check){
			case 1:
				addNewBook(bookList, spt);
				break;
			case 2:
				printAllBook(bookList, *spt);
				break;
			case 3:
				searchBookById(bookList, *spt);
				break;
			case 4:
				updateBook(bookList, *spt);
				break;
			case 5:
				removeBook(bookList, spt);
				break;
			case 6:
				saveToFile(bookList, *spt);
				break;
			case 7:
				readFile(bookList, spt);
				break;
			case 8:
				printEnd();
				break;
		}
	}while(check != 8);
	
}

int main(){
//	struct book bookList[Max+1];
//	int spt=0;  
//	process(bookList, &spt);
	char s[100];
	printf("Aaaaaaaaaaaaa:");
	scanf("%[abcd]", s);
	puts(s);
	
// 	char S1[100] ="abcdefgh";
// 	char S2[100] = "abcdefdh";
//	 printf("%d", strcmp(S1, S2));

	return 0;
}
	