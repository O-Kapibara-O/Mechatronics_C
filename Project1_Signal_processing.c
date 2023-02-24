
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <math.h>
//Mo¿liwe terminy:
// Poniedzia³ek 9.30-15.00
// Wtorek 11.30-15.00
// Œroda 9.00-15.00
// 
// (b/abs(a)x^2+1) +C
//-----------------------------------------------------------------------------------------------------------
void enter_field(double* x_min, double* x_max);
void enter_parameters_of_function(double parameters[]);
void generate(double* parameters, double x_min, double x_max, double* result, int size);
void generate_singnal_noise(double* results, int size, double* results_with_noises, int percentage, double amplitude);
void write_data_to_csv_file(double table[], char *fileName,double x_min, double x_max, int size);
double* dynamicAllocationMemory(int size);
void read_csv_file(double* myTable, char* fileName, int sizePar, double* xmin, double* xmax);
void bubbleSort(double table[], int size);
void mediaFilter(double* myTable, int size,double* tableWithOutNoises);
//-----------------------------------------------------------------------------------------------------------
void generate(double* parameters, double x_min, double x_max, double* result, int size)
{	
	// Parametr A = parameters, parametr B = parameters+1, parametr C = parameters+2
	double a = *parameters;
	double b = *(parameters + 1);
	double c = *(parameters + 2);
	double x;
	double delta = (x_max - x_min) / ((double)size - 1);
	for (int i = 0; i < size; i++)
	{
		x = x_min + delta * i;
		{
			result[i] = (b / (fabs(a) * (x * x) + 1) + c);
		}
	}
}

void enter_parameters_of_function(double parameters[])
{
	for (int i = 0; i < 3; i++)
	{
		printf("Enter %d paramer: ", i + 1);
		scanf("%lf", parameters + i);
	}
}

void enter_2_numbers(double *x_min, double *x_max)
{
	printf("Enter x_min: ");
	scanf("%lf", x_min);
	printf("Enter x_max: ");
	scanf("%lf", x_max);
}

void swap(double* number1, double* number2)
{
	double temp;
	temp = *number1;
	*number1 = *number2;
	*number2 = temp;
}

void enter_field(double* min, double* max)
{
	do
	{
		enter_2_numbers(min, max);
		if (*max - *min == 0)
		{
			printf("Numbers can't be the same, enter numbers one more time: \n");
		}
		else if (*min > *max)
		{
			swap(min, max);
		}
	} while (*max - *min == 0);
}

void write_data_to_csv_file(double table[], char* fileName, double x_min, double x_max, int size)
{
	FILE* file = fopen(fileName, "w");
	if (file != NULL)
	{
		setlocale(LC_ALL, "polish_poland");
		fprintf(file, "x;y \n");
		double delta = (x_max - x_min) / ((double)size - 1);
		for (int i = 0; i < size; i++)
		{
			fprintf(file, "%lf;%lf \n", x_min + delta * i, table[i]);
		}
		fclose(file);
	}
	else
	{
		printf("Error with file \n");
	}
}

void generate_singnal_noise(double* results, int size, double* results_with_noises,int percentage, double amplitude)
{
	int chance_to_noise;
	int value_of_amplitude;
	
	for (int i = 0; i < size; i++)
	{
		chance_to_noise = rand() % 100;
		int amplitude_x1000 = (int)(amplitude * 1000 *2);
		value_of_amplitude = rand() % (amplitude_x1000+1) -(amplitude*1000);

		if (chance_to_noise < percentage)
		{
			results_with_noises[i] = results[i] + (double)value_of_amplitude/1000;
		}
		else
		{
			results_with_noises[i] = results[i];
		}
	}
}

double* dynamicAllocationMemory(int size)
{
	return (double*)calloc(size, sizeof(double));
}

int numberOfLines(char* filename)
{
	int size = 0;
	char mystring[100];
	FILE* file;
	file = fopen(filename, "r");
	if (file != NULL)
	{
		fgets(mystring, 100, file);
		while (fgets(mystring, 100, file))
		{
			size++;
		}
		fclose(file);
	}
	else
	{
		return -1;
	}
	return size;
}

void read_csv_file(double* myTable, char* fileName, int sizePar, double* xmin, double* xmax)
{
	FILE* file;
	int ile;
	//double* tempTab = dynamicAllocationMemory(sizePar);
	//double tab[2];
	file = fopen(fileName, "r");
	if (file != NULL)
	{
		char mystring[100];
		char s1[100];
		char s2[100];
		fgets(mystring, 100, file);
		int size = -1;
		while (fgets(mystring, 100, file))
		{
			size++;
			int i = 0, j = 0, k = 0;
			int flag = 0;
			while (mystring[i] != '\n')
			{
				if (mystring[i] == ',')
				{
					mystring[i] = '.';
				}
				if (mystring[i] == ';')
				{
					flag = 1;
					i++;
					s1[j] = '\0';
					continue;
				}
				if (flag == 0)
				{
					s1[j] = mystring[i];
					j++;
				}
				else
				{
					s2[k] = mystring[i];
					k++;
				}
				i++;
			}
			s2[k] = '\0';
			ile = size;
			myTable[size] = atof(s2); // y
			//tempTab[size] = atof(s1); // x
			//*xmin = tempTab[0];
			//*xmax = tempTab[size];
		}
		fclose(file);
		//free(tempTab);
	}
	else
	{
		printf("Error with file - this file propably does not exist \n");
	}
}
int enterTheSize()
{
	int size =-1;
	while (size < 2)
	{
		printf("Enter the size of table: ");
		scanf("%d", &size);
	}
	return size;
}
void mediaFilter(double* table, int size, double* tableWithOutNoises)
{
	double window[5];

	tableWithOutNoises[0] = table[0];
	tableWithOutNoises[1] = table[1];
	tableWithOutNoises[size-1] = table[size - 1];
	tableWithOutNoises[size - 2] = table[size - 2];
	for (int i = 2; i < size-2; i++)
	{
		window[0] = table[i-2];
		window[1] = table[i-1];
		window[2] = table[i];
		window[3] = table[i+1];
		window[4] = table[i+2];
		bubbleSort(window,5);
		tableWithOutNoises[i] = window[3];
	}
}

void bubbleSort(double table[], int size)
{
	int i, j;
	for (i = 0; i < size - 1; i++)
		for (j = 0; j < size - i - 1; j++)
			if (table[j] > table[j + 1])
				swap(&table[j], &table[j + 1]);
}
int main()
{
	srand(time(NULL));
	int size;
	int userInput = 10;
	double* myTable = NULL;
	double* tableWithNoise = NULL;
	double* tableWithOutNoise = NULL;
	double parameter[3];
	double xmax;
	double xmin;
	char fileName[50]="FILE_NAME.CSV";
	while (userInput != 0)
	{
		printf("\n---------------------------\n");
		printf("1.Generate function \n");
		//printf("2.Read data from csv file \n");
		if (myTable != NULL)
		{
			printf("3.Make some noise in signal \n");
			printf("4.Write data to csv file \n");
			if (tableWithNoise != NULL)
			{
				printf("5.Denoise of signals \n");
			}
		}
		printf("0.Exit \n\n");
		printf("---------------------------\n\n");
		printf("Enter the number: ");
		scanf("%d", &userInput);


		switch (userInput)
		{
		case 1:
			if (myTable != NULL)
			{
				free(myTable);
				myTable = NULL;
			}
			size = enterTheSize();
			myTable = dynamicAllocationMemory(size);
			enter_parameters_of_function(parameter);
			enter_field(&xmin, &xmax);
			generate(parameter, xmin, xmax, myTable, size);
			break;
			
		case 2:
			if (myTable != NULL)
			{
				free(myTable);
				myTable = NULL;
			}
			printf("Enter name of file: ");
			scanf("%s", fileName);
			if (numberOfLines(fileName) == -1)
			{
				printf("Error with file - this file propably does not exist \n");
			}
			else
			{
				size = numberOfLines(fileName);
				myTable = dynamicAllocationMemory(size);
				read_csv_file(myTable, fileName, size, &xmin, &xmax);
			}
			break;
		case 3:
			if (tableWithNoise != NULL)
			{
				free(tableWithNoise);
				tableWithNoise = NULL;
			}
			int percentage = -3;
			double amplitude;
			while (percentage < 1 || percentage >=100)
			{
				printf("Enter percentage (1 - 99) :");
				scanf("%d", &percentage);
			}
			printf("Enter apmlitude :");
			scanf("%lf", &amplitude);
			amplitude = abs(amplitude);

			tableWithNoise = dynamicAllocationMemory(size); 
			generate_singnal_noise(myTable, size, tableWithNoise, percentage, amplitude);
			break;
		case 4:

			if (myTable != NULL)
			{
				printf("1.Save signal in csv file \n");
			}
			if (tableWithNoise != NULL)
			{
				printf("2.Save signal with noises in csv file \n");
			}
			if (tableWithNoise != NULL)
			{
				printf("3.Save signal without noises in csv file \n");
			}
			int userInput2;
			printf("Enter the number: ");
			scanf("%d", &userInput2);
			if (userInput2 == 1)
			{
				printf("Enter name of file: ");
				scanf("%s", fileName);
				write_data_to_csv_file(myTable, fileName, xmin, xmax, size);
			}
			if (userInput2 == 2)
			{
				printf("Enter name of file: ");
				scanf("%s", fileName);
				write_data_to_csv_file(tableWithNoise, fileName, xmin, xmax, size);
			}
			if (userInput2 == 3)
			{
				printf("Enter name of file: ");
				scanf("%s", fileName);
				write_data_to_csv_file(tableWithOutNoise, fileName, xmin, xmax, size);
			}
			else
			{
				printf("No options \n");
			}
			break;
		case 5:
			if (tableWithOutNoise != NULL)
			{
				free(tableWithOutNoise);
				tableWithOutNoise = NULL;
			}
			tableWithOutNoise = dynamicAllocationMemory(size);
			mediaFilter(tableWithNoise, size, tableWithOutNoise);
			break;
		case 0:
			free(myTable);
			free(tableWithNoise);
			free(tableWithOutNoise);
			exit(1);
			break;
		default:
			printf("No options !!! \n");
			break;
		}
	}
}
