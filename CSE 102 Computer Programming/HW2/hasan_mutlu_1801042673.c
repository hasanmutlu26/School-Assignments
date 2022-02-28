#include <stdio.h>
/*pre-deﬁned variables*/
#define p1 0.5
#define p2 20

int main(){
    /*File pointers for both files.*/
    FILE *fp1, *fp2;
    /*n, a and b are the variables defined in the PDF.
    sum and i variables will be used to calculate the average.
    sum is sum of numbers in a chunk and i is amount of numbers in a chunk.*/
    float n, sum = 0.0, a, b, i=2;

    /*files are opened.*/
	fp1 = fopen ("input.txt", "r+");
	fp2 = fopen ("output.txt", "w+");

	/*If files are not opened, an error message is shown.*/
    if (fp1 == NULL) {
         printf("File opening error!");
    }
    if (fp2 == NULL){
        printf("File opening error!");
    }

    /*The very first number in the file is read manually.*/
    fscanf(fp1, "%f", &n);
    /*n is added to sum variable to find average in next commands.*/
    sum += n;
    /*Since a is the current average,
    when the second number will be added,
    a is equal to the first number, which is now n.*/
    a = n;

    /*First number in the file written manually.*/
    fprintf(fp2, "%.4f ", n);

    /*Rest of the document will be written with this while loop until it reaches end of file.*/
    while (fscanf(fp1, "%f", &n) != EOF){
        /*After the next number is read, n is added to the sum.*/
    	sum += n;
    	/*b, the new average is calculated.*/
    	b = sum / i;

		/*If the condition stated in PDF satisfied, the number is added to the current chunk.*/
    	if(!(b > a*(1+p1) || b < a*(1-p1) || a > n*p2 || a < n/p2)){
            /*The number is written in the current chunk.*/
    		fprintf(fp2, "%.4f ", n);
            /*New average of the chunk is assigned to 'a' variable.*/
    		a = b;
    		/*i is incremented by one as the amount of numbers in the chunk is incremented.*/
			i++;
		}

		/*If the condition is not satisfied, it means that the current chunk ends here. */
		else{
            /*Current chunk's average is written.*/
			fprintf(fp2, "Average: %.4f \n", a);
            /*First number of the new chunk is written.*/
			fprintf(fp2, "%.4f ", n);
			/*sum, i and a variables are restored to calculate new chunk's average.*/
			sum = 0.0;
			i = 2;
			sum += n;
			a = n;
		}
	}
	/*After the file pointer reaches the end of file, the average of the last chunk is written manually.*/
    fprintf(fp2, "Average: %.4f \n", a);

    /*Files are closed.*/
	fclose(fp1);
	fclose(fp2);

    return 0;
}
