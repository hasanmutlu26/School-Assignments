/*Hasan Mutlu - 1801042673 - CSE 108 Lab 9*/
#include <stdio.h>
#include <string.h>

/*Maximum sizes for some arrays.*/
#define STUDENT_NAME 20
#define STUDENT_LIST 20
#define GAMES_LIST 20
#define GAMES_NAME 30
#define PLATFORMS_LIST 20

/*Structs are declared according to PDF*/
typedef struct line{
	float x1;
	float y1;
	float x2;
	float y2;
	float x3;
	float y3;
	float slope;
}line;

typedef struct grades{
	float midterm;
	float final;
	float homework;
}grades;

typedef struct student{
	char name[STUDENT_NAME];
	int id;
	grades studentGrades;
}student;

typedef struct games{
	char gameName[GAMES_NAME];
	char platforms[PLATFORMS_LIST];
	float score;
}games;

/*Every part is one function.*/
void part1();
void part2();
void part3();

/*Parts are called from main*/
void main(){
	part1();
	part2();
	part3();
}


void part1(){
	printf("PART 1\n\n");
	line myLine;
	float b;

	/*The coordinates are obtained.*/
	printf("Enter cordinates:\n");
	scanf("%f", &myLine.x1);
	scanf("%f", &myLine.y1);
	scanf("%f", &myLine.x2);
	scanf("%f", &myLine.y2);
	scanf("%f", &myLine.x3);

	/*Slope is calculated.*/
	myLine.slope = (myLine.y2 - myLine.y1)/(myLine.x2 - myLine.x1);
	/*The value of b in formula is calculated.*/
	b = myLine.y1 - (myLine.x1 * myLine.slope);
	/*y3 is calculated.*/
	myLine.y3 = (myLine.x3 * myLine.slope) - b;

	/*Result is printed.*/
	printf("y3 = %f\n", myLine.y3);
	printf("Slope = %f\n", myLine.slope);
}


void part2(){
	printf("\n\nPART 2\n\n");
	/*Student array and average list array are created.*/
	student studentArray[STUDENT_LIST];
	float averageList[STUDENT_LIST];
	int num_of_students, search;

	/*Number of students is obtained from user.*/
	printf("Enter number of students: ");
	scanf("%d", &num_of_students);

	/*Student's information is obtained from user.*/
	for(int i=0; i<num_of_students; i++){
		printf("Enter %d. student's name: ", i+1);
		scanf("%s", studentArray[i].name);
		printf("Enter %d. student's id: ", i+1);
		scanf("%d", &studentArray[i].id);
		printf("Enter %d. student's grades: ",i+1);
		scanf("%f %f %f", 	&studentArray[i].studentGrades.midterm, 
							&studentArray[i].studentGrades.final, 
							&studentArray[i].studentGrades.homework);
		/*Average is calculated and saved into corresponding index number cell of array.*/
		averageList[i] = (studentArray[i].studentGrades.midterm 
						+ studentArray[i].studentGrades.final 
						+ studentArray[i].studentGrades.homework) / 3;
		printf("Average grade of this student is %f\n\n", averageList[i]);
	}

	/*In a while loop, id search is done. -1 is needed to stop loop.*/
	while(1){
		printf("Enter an id number to print student's information.\n");
		printf("Enter -1 to stop: ");
		scanf("%d", &search);

		/*If user enters -1, loop is broken.*/
		if(search == -1)
			break;

		/*In a loop, whole array is covered.*/
		for(int i = 0; i < num_of_students; i++){
			/*If id is found, student info is printed and loop is broken.*/
			if(studentArray[i].id == search){
				printf("Student name: %s\n", studentArray[i].name);
				printf("Student id: %d\n", studentArray[i].id);
				printf("Student grades:\n");
				printf("Midterm: %.0f\n", studentArray[i].studentGrades.midterm);
				printf("Final: %.0f\n", studentArray[i].studentGrades.final);
				printf("Homework: %.0f\n", studentArray[i].studentGrades.homework);
				printf("Average: %.0f\n\n", averageList[i]);
				break;
			}
			/*If i get to num_of_students - 1 without the loop is broken, the student doesn't exist.
			Error message is printed.*/ 
			else if(i == num_of_students - 1){
				printf("There isn't any student with this id number.\n\n");
			}
		}
	}
}

/*In this part, fflush(stdin) and scanf("%[^\n]%*c", string) functions didn't work for me.*/
void part3(){
	printf("\n\nPART 3\n\n");
	games gamesArray[GAMES_LIST];
	int num_of_games, i;
	float sum = 0.0;

	/*Just like part 2, num of games and informations are obtained.*/
	printf("Enter number of games: ");
	scanf("%d", &num_of_games);
	fflush(stdin);

	for(i= 0; i< num_of_games; i++){
		printf("Enter %d. game's name: ", i+1);
		scanf("%s", gamesArray[i].gameName);
		printf("Enter %d. game's platforms: ", i+1);
		scanf("%s", gamesArray[i].platforms);
		printf("Enter %d. game's score: ", i+1);
		scanf("%f", &gamesArray[i].score);
		printf("\n");
	}

	/*Scores of games are summed.*/
	for(i=0;i<num_of_games;i++)
		sum += gamesArray[i].score;

	/*Average is assigned into sum variable and printed.*/
	sum = sum / (float)num_of_games;
	printf("Average score of the games: %f\n", sum);

	/*Because the function given in PDF didn't work for me, I could only search given platform names in PDF.*/
	/*In a loop, all array is covered and if strstr function returns something, it means that the game is on that platform.
	It is printed.*/
	printf("PC games:\n");
	for (i = 0; i < num_of_games; ++i){
		if(strstr(gamesArray[i].platforms,"PC"))
			printf("%s\n", gamesArray[i].gameName);
	}
	printf("\n");

	printf("PS4 games:\n");
	for (i = 0; i < num_of_games; ++i){
		if(strstr(gamesArray[i].platforms,"PS4"))
			printf("%s\n", gamesArray[i].gameName);
	}
	printf("\n");

	printf("mobile games:\n");
	for (i = 0; i < num_of_games; ++i){
		if(strstr(gamesArray[i].platforms,"mobile"))
			printf("%s\n", gamesArray[i].gameName);
	}




}