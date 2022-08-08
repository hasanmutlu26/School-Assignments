/*Hasan Mutlu - 1801042673 - CSE 108 Lab 10*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NAMESIZE 30
#define SUBJECTSIZE 50

/*Book struct*/
typedef struct Book{
	char title[NAMESIZE];
	char author[NAMESIZE];
	char subject[SUBJECTSIZE];
	int year;
	int id;
}Book;

/*Functions.*/
void menu();
void printMenu();
void printSubmenu();

void addNewBook(Book* bookList, int* num);
void listBooks(Book* bookList, int num);
void searchByName(Book* bookList, int num);

/*menu function is called from main*/
void main(){
	menu();
}

void menu(){
	Book* bookList;		/*List of books.*/
	int selection=0;	/*An integer for menu commands.*/
	int num = 0;		/*Number of books added.*/

	/*Menu loop.*/
	while(1){
		/*Menu is printed and user selection is obtained.*/
		printMenu();
		scanf("%d", &selection);

		if(selection == 3)
			break;

		/*If selection is 1, a new Book block is created and addNewBook function is called.*/
		if(selection == 1){
			bookList = (Book*)calloc(1,sizeof(Book));
			addNewBook(bookList, &num);
		}

		/*If selection is 2, Listbooks function is called.*/
		if(selection == 2)
			listBooks(bookList, num);
	}
}

/*This function simply gets strings from user and saves them into last created Book struct.*/
/*Functions in pdf didn't work for me again so i used %s*/
void addNewBook(Book* bookList, int* num){
	printf("\n*********************\n");
	printf("Please enter book title = ");
	scanf("%s", bookList[*num].title);

	printf("Please enter book author = ");
	scanf("%s", bookList[*num].author);

	printf("Please enter book subject = ");
	scanf("%s", bookList[*num].subject);

	printf("Please enter book year = ");
	scanf("%d", &bookList[*num].year);

	bookList[*num].id = *num;
	printf("%s*****\n", bookList[*num].title);
	
	/*num is increased as a new book is added.*/
	(*num)++;
}

/*This function is a submenu for list books selection.*/
void listBooks(Book* bookList, int num){
	int selection=0;

	/*In loop, submenu is printed and user selection is obtained.*/
	while(1){
		printSubmenu();
		scanf("%d", &selection);
		if(selection == 6)
			break;

		/*If selection is 1, searchByName function is called for getByTitle.*/
		if(selection == 1)
			searchByName(bookList, num);
	}
}

/*This function searches for book title.*/
void searchByName(Book* bookList, int num){
	char buffer[NAMESIZE];
	printf("Enter book title = ");
	/*User enters a title for search.*/
	scanf("%s", buffer);

	/*All list is covered and if book is found, its information is printed.*/
	for(int i= 0; i<num; i++){
		if(!strcmp(bookList[i].title,buffer)){
			printf("Book Found\n");
			printf("Title => %s\n", bookList[i].title);
			printf("Author => %s\n", bookList[i].author);
			printf("Subject => %s\n", bookList[i].subject);
			printf("Year => %d\n", bookList[i].year);
		}
		else
			printf("Book couldn't found.\n");
	}
}

/*Next two functions simply prints the menus.*/
void printMenu(){
	printf("\n*********************\n");
	printf("MENU\n");
	printf("\t1. Add New Book\n");
	printf("\t2. List Books\n");
	printf("\t3.EXIT\n");
	printf("Choose = ");
}

void printSubmenu(){
	printf("\n*********************\n");
	printf("SUBMENU\n");
	printf("\t1. Get by Title\n");
	printf("\t2. Get by Author\n");
	printf("\t3. Get by Subject\n");
	printf("\t4. Sorted List by Year\n");
	printf("\t5. List all Books\n");
	printf("\t6. EXIT SUBMENU\n");
	printf("Choose = ");
}

