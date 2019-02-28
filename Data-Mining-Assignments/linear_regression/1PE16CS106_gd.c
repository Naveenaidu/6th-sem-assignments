/*
For reference -  https://towardsdatascience.com/linear-regression-using-gradient-descent-97a6c8700931
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_ROWS 50
#define NUM_ITER 3000000
#define LEARNING_RATE 0.00001

double slope_gd = 0.0 , constant_gd = 0.0;

typedef struct{
	int date;
	int month;
	int index;
	float close_value;
	double predicted_close_value;
} record_t;

int calculate_index_value(int, int);
void calculate_gradient_descent( record_t records[MAX_ROWS], int);
void predict_value_gradient_descent();

/*
count -> Used to count the number of rows in the csv file
*/
int main(void){

	FILE *fp;
	record_t records[MAX_ROWS];
	int count = 0 ;

	fp = fopen("data.csv", "r");
	if (fp == NULL){
		fprintf(stderr, "Error Reading the file \n");
	}

	// This stores the scanned values into the structure objects
	// and also finds the count of the number of rows and finds the x_value
	// for each date
	while (fscanf(fp, "%d %d %f", &records[count].date, 
		   &records[count].month, &records[count].close_value) == 3){

		records[count].index = calculate_index_value(records[count].month, 
			                                         records[count].date);	
		count++; 
	}



	fclose(fp);

	calculate_gradient_descent(records, count);
	predict_value_gradient_descent();


}

// Used to calculate the new x_value given to the records
int calculate_index_value(int month, int date){

	int index;

	// If the month is january, subtract the date from 24
	if( month == 1){
		index = date - 24;
	}

	// If the month is Feb, add 7 to the date. This is done because 
	// according to the data set we have, we have only 7 days in Jan.
	if( month == 2){
		index = date + 7;
	}

	if (month == 3){
		index = date + 35;
	}

	return(index);

}

void calculate_gradient_descent( record_t records[MAX_ROWS], int count){

	double summ_y_diff = 0.0;
	double summ_x_into_y_diff = 0.0;
	double differentaion_by_slope;
	double differentaion_by_constant;

	for(int i=0; i<NUM_ITER; i++){

		summ_x_into_y_diff = 0.0;
		summ_y_diff = 0.0;

		for (int j = 0; j<count; j++){

			records[j].predicted_close_value = 0;

			// The current predicted value of Y
			records[j].predicted_close_value = (slope_gd * records[j].index) + constant_gd;
			summ_x_into_y_diff += (records[j].index * (records[j].close_value - records[j].predicted_close_value));
			summ_y_diff += (records[j].close_value - records[j].predicted_close_value);
		}

		differentaion_by_slope = ((double)-2/count) * summ_x_into_y_diff; //Derivative wrt m
		differentaion_by_constant = ((double)-2/count) * summ_y_diff; // Derivative wrt C

		/*
		printf("\n The value of summ_y_diff is  %.5f \n", summ_y_diff);
		printf(" The value of summ_x_into_y_diff  %.5f \n", summ_x_into_y_diff);
		printf(" The value of differentaion_by_slope  %.5f \n", differentaion_by_slope);
		printf(" The value of differentaion_by_constant  %.5f \n", differentaion_by_constant);
        */

		// Update the values of slope and constant
		slope_gd = slope_gd - (LEARNING_RATE * differentaion_by_slope);
		constant_gd = constant_gd - (LEARNING_RATE * differentaion_by_constant);

		/*		
		printf(" The value of Slope is  %.5f \n", slope_gd);
		printf(" The value of Constant is %.5f\n", constant_gd);
		*/

	}

	printf(" The values of slope and constant after Gradient descent is \n");
	printf(" The value of Slope is  %.5f \n", slope_gd);
	printf(" The value of Constant is %.5f\n", constant_gd);


}

void predict_value_gradient_descent(){
	int opt,month,day,index;
	double predicted_value = 0.0;

	printf("\n ---------- PREDICTING VALUES USING GRADIENT descent METHOD ---------\n");
	
	while(1){
		
		printf("\nEnter the month\n");
		scanf("%d",&month);
		printf("Enter the day\n");
		scanf("%d", &day);

		day = calculate_index_value(month,day);

		predicted_value = slope_gd * day + constant_gd;

		printf("The predicted value is %.10f\n", predicted_value );

	}

}