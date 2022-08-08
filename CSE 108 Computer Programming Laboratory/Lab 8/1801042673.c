/*Hasan Mutlu - 1801042673 - Lab 8*/

#include <stdio.h>
#include <math.h>

/*Matrix struct definition.*/
typedef struct matrix{
	double arr[3][3];
	double determinant;
}matrix;

void print_matrix(matrix initial_matrix);
void inverse_matrix(matrix* initial_matrix, matrix* inverted_matrix);
void determinant_of_matrix(matrix* initial_matrix);

/*Vector struct definition.*/
typedef struct vector{
	double x;
	double y;
	double z;
}vector;

double find_orthogonal(vector vec_1, vector vec_2, vector* output_vec);

/*polynomial struct definition.*/
typedef struct polynomial{
	double deg3;
	double deg2;
	double deg1;
	double deg0;
}polynomial;

int main(){
	/*PART 1 */

	/*Initial matrix and inverted matrix are created.*/
	matrix initial_matrix, inverted_matrix;

	/*Initial matrix's elements are obtained by the user.*/
	printf("Enter matrix elements:\n");
	for(int i = 0; i< 3; i++){
		for(int j=0; j<3; j++){
			scanf("%lf", &initial_matrix.arr[i][j]);
		}
	}

	/*Initial matrix is printed.*/
	printf("\nInitial matrix:\n");
	print_matrix(initial_matrix);

	/*Inverse_matrix functions is called.*/
	inverse_matrix(&initial_matrix, &inverted_matrix);
	
	/*If product of determinants of initial and inverted matrices is 1.0, then it was inverted. 
	Inverted matrix is printed.*/
	if(inverted_matrix.determinant*initial_matrix.determinant == 1.0){
		printf("inverted matrix:\n");
		print_matrix(inverted_matrix);
	}
	/*Else, the matrix is not invertible.*/
	else{
		printf("The matrix is not invertible.\n\n\n");
	}

	/*PART 2*/
	/*Vectors are created.*/
	vector vec1, vec2, output_vec;

	/*Two vector's elements are obtained from the user.*/
	printf("Enter first vector's elements:\n");
	scanf("%lf%lf%lf", &vec1.x, &vec1.y, &vec1.z);
	printf("Enter second vector's elements:\n");
	scanf("%lf%lf%lf", &vec2.x, &vec2.y, &vec2.z);

	/*find orthogonal function is called and the angle is printed.*/
	printf("\nAngle: %lf\n", find_orthogonal(vec1,vec2,&output_vec));

	/*The output vector is printed.*/
	printf("Output vector:\nx=%lf	y=%lf	z=%lf\n",output_vec.x, output_vec.y, output_vec.z);


	/*PART 3 does not exist.*/
	printf("\nPART 3 does not exist.\n");
	return 0;
}

/*Print matrix function simply prints all elements of the matrix in a nested loop.*/
void print_matrix(matrix initial_matrix){
	for(int i = 0; i< 3; i++){
		for(int j=0; j<3; j++){
			printf("%.4lf    ",initial_matrix.arr[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
}


void inverse_matrix(matrix* initial_matrix, matrix* inverted_matrix){
	/*Determinant of initial_matrix is calculated with function.*/
	determinant_of_matrix(initial_matrix);

	/*inverted_matrix's determinant is set to 0 for preventing errors.*/
	inverted_matrix->determinant = 0;

	/*If initial_matrix is not 0, it is invertible. Inversion iperation is done according to the formula.*/
	if(initial_matrix->determinant != 0){
		inverted_matrix->arr[0][0] = (1/initial_matrix->determinant)*(initial_matrix->arr[1][1]*initial_matrix->arr[2][2] - initial_matrix->arr[1][2]*initial_matrix->arr[2][1]);
		inverted_matrix->arr[1][0] = (1/initial_matrix->determinant)*(initial_matrix->arr[1][0]*initial_matrix->arr[2][2] - initial_matrix->arr[1][2]*initial_matrix->arr[2][0]);
		inverted_matrix->arr[2][0] = (1/initial_matrix->determinant)*(initial_matrix->arr[1][0]*initial_matrix->arr[2][1] - initial_matrix->arr[1][1]*initial_matrix->arr[2][0]);
		inverted_matrix->arr[0][1] = (1/initial_matrix->determinant)*(initial_matrix->arr[0][1]*initial_matrix->arr[2][2] - initial_matrix->arr[0][2]*initial_matrix->arr[2][1]);
		inverted_matrix->arr[1][1] = (1/initial_matrix->determinant)*(initial_matrix->arr[0][0]*initial_matrix->arr[2][2] - initial_matrix->arr[0][2]*initial_matrix->arr[2][0]);
		inverted_matrix->arr[2][1] = (1/initial_matrix->determinant)*(initial_matrix->arr[0][0]*initial_matrix->arr[2][1] - initial_matrix->arr[0][1]*initial_matrix->arr[2][0]);
		inverted_matrix->arr[0][2] = (1/initial_matrix->determinant)*(initial_matrix->arr[0][1]*initial_matrix->arr[1][2] - initial_matrix->arr[0][2]*initial_matrix->arr[1][1]);
		inverted_matrix->arr[1][2] = (1/initial_matrix->determinant)*(initial_matrix->arr[0][0]*initial_matrix->arr[1][2] - initial_matrix->arr[0][2]*initial_matrix->arr[1][0]);
		inverted_matrix->arr[2][2] = (1/initial_matrix->determinant)*(initial_matrix->arr[0][0]*initial_matrix->arr[1][1] - initial_matrix->arr[0][1]*initial_matrix->arr[1][0]);
		
		/*After inverse matrix is created, it's determinant is calculated.*/
		determinant_of_matrix(inverted_matrix);
	}
}


void determinant_of_matrix(matrix* initial_matrix){
	double det;
	/*The determinant is calculated according to the formula.*/
	det = initial_matrix->arr[0][0]*(initial_matrix->arr[1][1]*initial_matrix->arr[2][2] - initial_matrix->arr[1][2]*initial_matrix->arr[2][1])
		- initial_matrix->arr[0][1]*(initial_matrix->arr[1][0]*initial_matrix->arr[2][2] - initial_matrix->arr[1][2]*initial_matrix->arr[2][0])
		+ initial_matrix->arr[0][2]*(initial_matrix->arr[1][0]*initial_matrix->arr[2][1] - initial_matrix->arr[1][1]*initial_matrix->arr[2][0]);
	/*The result is assigned to matrix's determinant cell.*/
	initial_matrix->determinant = det;
}

double find_orthogonal(vector vec_1, vector vec_2, vector* output_vec){
	double angle;
	double A, B, dotProduct;

	/*‖A‖, ‖B‖ and A.B dot product are calculated.*/
	A = sqrt(vec_1.x*vec_1.x + vec_1.y*vec_1.y + vec_1.z*vec_1.z);
	B = sqrt(vec_2.x*vec_2.x + vec_2.y*vec_2.y + vec_2.z*vec_2.z);
	dotProduct = vec_1.x*vec_2.x + vec_1.y*vec_2.y + vec_1.z*vec_2.z;

	/*The angle between is calculated.*/
	angle = acos(dotProduct/(A*B));

	/*Cross product of vector1 and vector2 is calculated.*/
	output_vec->x = (vec_1.y*vec_2.z) - (vec_1.z*vec_2.y);
	output_vec->y = (vec_1.x*vec_2.z) - (vec_1.z*vec_2.x);
	output_vec->z = (vec_1.x*vec_2.y) - (vec_1.y*vec_2.x);
	
	/*Angle is returned.*/
	return angle;
}