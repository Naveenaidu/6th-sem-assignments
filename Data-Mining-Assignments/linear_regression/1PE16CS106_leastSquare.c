#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ROWS 50

// slope_ls -> slope for least square method
// constant_ls --> constant for least square method
double slope_ls,constant_ls;

// Used for storing the values from csv file
// index -> Tells the new x_value that this `date`. Each date is assigned a 
//          new value which will be treated as their x_values.
// close_value -> This is the y_value for the specific `date`
typedef struct{
	int date;
	int month;
	float close_value;
	int index;
} record_t;


void calculate_least_square(record_t records[MAX_ROWS], int );
int calculate_index_value(int , int);
void predict_value_least_square();

/*
count -> Used to count the number of rows in the csv file
*/
int main(void){

	FILE *fp;
	record_t records[MAX_ROWS];
	int count = 0;

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

	/*
   
   // Used for printing the values of csv file	

	for(int i=0; i < count; i++){
		printf("%d %d %.2f %d\n",records[i].date, records[i].month,
			    records[i].close_value, records[i].index);
	}
   
   */

	fclose(fp);

	calculate_least_square(records, count);
	predict_value_least_square();

	return 0;

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

// For a given data points we need to find the value of slope(m) and constant(b)
// which would give us the line which has could fit the points as efficiently as
// possible
// Check: https://www.mathsisfun.com/data/least-squares-regression.html
// summ_xy --> summation of xy(Product of x and y)
// summ_x --> summation of all the values of x
void calculate_least_square(record_t records[MAX_ROWS], int count){
	float summ_x = 0.0,summ_y = 0.0;
	float x_mean, y_mean;
	float covariance , variance;

	for(int i=0; i < count; i++){

		summ_x += records[i].index;
		summ_y += records[i].close_value;
	}

	x_mean = summ_x/count;
	y_mean = summ_y/count;

	for(int i=0; i < count; i++){

		covariance += (records[i].index - x_mean) * (records[i].close_value - y_mean);
		variance += pow((records[i].index - x_mean),2);
	}
	
	slope_ls = covariance / variance;
	constant_ls = y_mean - (slope_ls*x_mean);

	printf("slope is : %f \nconstant is : %f \n", slope_ls, constant_ls);

}

void predict_value_least_square(){
	int opt,month,day,index;
	double predicted_value = 0.0;

	printf("---------- PREDICTING VALUES USING LEAST SQUARE METHOD ---------\n");
	
	while(1){
		
		printf("\nEnter the month\n");
		scanf("%d",&month);
		printf("Enter the day\n");
		scanf("%d", &day);

		day = calculate_index_value(month,day);

		predicted_value = slope_ls * day + constant_ls;

		printf("The predicted value is %f\n", predicted_value );

	}

}
	



