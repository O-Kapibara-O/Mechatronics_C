#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>
struct picture
{
		int width;
		int height;
		char name[100];
		int max_value;
		int** pixels;
};
struct listOfPictures
{
	struct picture * listOfPictures;
	int current;
	int size;
};
/*
char* getName()
{
	char name[50]="File_name";
	printf("Enter the name of file: ");	
	scanf("%s", name);
	return name;
}
*/
//-----------------------------------------------------------
int read_data(FILE* file);
void save_to_file_pgm(struct picture* argPicture);
int allocateMemory(struct picture* argPicture);
void read_PGM_file(struct picture* argPicture);
void release_memory_of_picture(struct picture* argPicture);

void salt_and_pepper(struct picture* argPicture, int percente);
void medianFilter(struct picture* argPicture);
void bubbleSort(int table[], int size);
void swap(double* number1, double* number2);
//-----------------------------------------------------------
int read_data(FILE* file)
{
	char bufor[21];
	int data;
	while (1)
	{
		if (fscanf(file, "%20s", bufor) == 1)
		{
			if (sscanf(bufor, "%d", &data) == 1)
			{
				return data;
			}
			if (bufor[0] != '#')
			{
				return -1;
			}
			fscanf(file, "%*[^\n]");
		}
		else
		{
			return -1;
		}
	}
}


void save_to_file_pgm(struct picture* argPicture)
{
	if (argPicture->pixels != NULL)
	{
		FILE* file;
		char name[50] = "File_name";
		printf("Enter the name of file: ");
		scanf("%s", name);
		file = fopen(name, "w");
		if (file != NULL)
		{
			fprintf(file, "P2 \n");
			fprintf(file, "%d %d \n", argPicture->width, argPicture->height);
			fprintf(file, "%d \n", argPicture->max_value);
			for (int i = 0; i < argPicture->height; i++)
			{
				for (int j = 0; j < argPicture->width; j++)
				{
					fprintf(file, "%d ", argPicture->pixels[i][j]);
				}
				fprintf(file, "\n");
			}
			fclose(file);
		}
		else
		{
			printf("Error with file - cant create this file \n");
		}
	}
	else
	{
		printf("Cant create file without data. \n");
	}
}

int allocateMemory(struct picture* argPicture)
{
	if (argPicture->pixels == NULL)
	{
		argPicture->pixels = (int**)calloc(argPicture->height, sizeof(int*));
		if (argPicture->pixels != NULL)
		{
			for (int i = 0; i < argPicture->height; i++)
			{
				argPicture->pixels[i] = (int*)calloc(argPicture->width, sizeof(int));
				if (argPicture->pixels[i] != NULL)
				{

				}
				else
				{
					printf("Error with allocation memory. \n"); 
					for (int x = 0; x <= i; x++)
					{
						free(argPicture->pixels[x]);
					}
					free(argPicture->pixels);
					argPicture->pixels = NULL;
					return -1;
				}
			}
		}
		else
		{
			printf("Error with allocation memory. \n");
			return -1;
		}
	}
	else
	{
		printf("This picture have already data. \n");
		return -1;
	}
}

void read_PGM_file(struct picture* argPicture)
{
	if (argPicture->pixels == NULL)
	{
		FILE* file;
		char name[50] = "File_name";
		printf("Enter the name of file: ");
		scanf("%s", name);

		int height, width, data, value;
		int read_data_checker;
		file = fopen(name, "r");
		if (file != NULL)
		{
			if (fgetc(file) == 'P' && fgetc(file) == '2')
			{
				argPicture->width = read_data(file);
				argPicture->height = read_data(file);
				argPicture->max_value = read_data(file);
				strcpy(argPicture->name, name);
				if (argPicture->width <= 0 || argPicture->height == -1 || argPicture->max_value == -1)
				{
					printf("This file is broken for pgm standard. \n");
					fclose(file);
					return;
				}
                if (allocateMemory(argPicture) <0)
				{
					fclose(file);
					return;
				}
				else
				{
					for (int i = 0; i < argPicture->height; i++)
					{
						for (int j = 0; j < argPicture->width; j++)
						{
							read_data_checker = read_data(file);
							if (read_data_checker == -1)
							{
								printf("This file is broken for pgm standard. \n");
								fclose(file);
								release_memory_of_picture(argPicture);
								return;
							}
							else
							{
								argPicture->pixels[i][j] = read_data_checker;
							}

						}
					}
				}
				
			}
			else
			{
				printf("Ohh noo, we opend wrong file !!! \n");
			}
			fclose(file);
		}
		else
		{
			printf("Error with file - this file propably does not exist \n");
		}
	}
	else
	{
		printf("Before you do this, you need to release already use memory. \n");
	}
}

void release_memory_of_picture(struct picture* argPicture)
{
	if (argPicture->pixels != NULL)
	{
		for (int i = 0; i < argPicture->height; i++)
		{
			free(argPicture->pixels[i]);
		}
		free(argPicture->pixels);
		argPicture->pixels = NULL;
	}
	else
	{
		printf("Memory already deleted. \n");
	}
}
void make_negative(struct picture* argPicture)
{
	if (argPicture->pixels != NULL)
	{
		for (int i = 0; i < argPicture->height; i++)
		{
			for (int j = 0; j < argPicture->width; j++)
			{
				argPicture->pixels[i][j] = argPicture->max_value - argPicture->pixels[i][j];
			}
		}
	}
	else
	{
		printf("This picture does not exist");
	}
}

int** allocate_2d_array(int height, int width)
{
	int** temporaryPixels = (int**)calloc(width , sizeof(int*));
	if (temporaryPixels != NULL)
	{
		for (int i = 0; i < width; i++)
		{
			temporaryPixels[i] = (int*)calloc(height , sizeof(int));
			if (temporaryPixels[i] != NULL)
			{

			}
			else
			{
				printf("Error with allocation memory. \n");
				for (int x = 0; x <= i; x++)
				{
					free(temporaryPixels[x]);
				}
				free(temporaryPixels);
				return NULL;
			}
		}
		return temporaryPixels;
	}
	else
	{
		printf("Error with allocation temporary memory \n");
	}
}

void rotate(struct picture* argPicture)
{
	if (argPicture->pixels != NULL)
	{
		int** temporaryPixels = allocate_2d_array(argPicture->height, argPicture->width);
		if(temporaryPixels !=NULL)
		{
			for (int i = 0; i < argPicture->height; i++)
			{
				for (int j = 0; j < argPicture->width; j++)
				{
					temporaryPixels[j][argPicture->height - i - 1] = argPicture->pixels[i][j];
				}
			}
			release_memory_of_picture(argPicture);
			int temp;
			temp = argPicture->height;
			argPicture->height = argPicture->width;
			argPicture->width = temp;
			temp = allocateMemory(argPicture);
			if (temp == -1)
			{
				printf("Error occurred, rotate fail !!! \n ");
				return;
			}
			for (int i = 0; i < argPicture->height; i++)
			{
				for (int j = 0; j < argPicture->width; j++)
				{
					argPicture->pixels[i][j] = temporaryPixels[i][j];
				}
			}
			for (int i = 0; i < argPicture->width; i++)
			{
				for (int j = 0; j < argPicture->height; j++)
				{
					free(temporaryPixels[j]);
				}
				free(temporaryPixels);
			}
		}
		else
		{
			printf("Error occurred, rotate fail !!! \n ");
		}
	}
	else
	{
		printf("This picture does not exist \n");
	}
	return 0;
}

void medianFilter(struct picture* argPicture)
{
	if (argPicture->pixels != NULL)
	{
		int window[9];

		for (int i = 0; i < argPicture->height; i++)
		{
			for (int j = 0; j < argPicture->width; j++)
			{
				if (i == 0 && j==0)
				{
					//up-Left corner
					window[0] = argPicture->pixels[i][j];
					window[1] = argPicture->pixels[i][j];
					window[2] = argPicture->pixels[i][j+1];
					window[3] = argPicture->pixels[i][j];
					window[4] = argPicture->pixels[i][j];
					window[5] = argPicture->pixels[i][j + 1];
					window[6] = argPicture->pixels[i + 1][j];
					window[7] = argPicture->pixels[i + 1][j];
					window[8] = argPicture->pixels[i + 1][j + 1];
					bubbleSort(window, 9);
					argPicture->pixels[i][j] = window[4];
				}
				//up row
				else if(i==0 && (j>0 && j< (argPicture->width) - 1))
				{
					window[0] = argPicture->pixels[i][j - 1];
					window[1] = argPicture->pixels[i][j];
					window[2] = argPicture->pixels[i][j + 1];
					window[3] = argPicture->pixels[i][j - 1];
					window[4] = argPicture->pixels[i][j];
					window[5] = argPicture->pixels[i][j + 1];
					window[6] = argPicture->pixels[i + 1][j -1];
					window[7] = argPicture->pixels[i + 1][j];
					window[8] = argPicture->pixels[i + 1][j + 1];
					bubbleSort(window, 9);
					argPicture->pixels[i][j] = window[4];
				}
				//up-right corner
				else if(i==0 && (j==(argPicture->width)-1))
				{
					window[0] = argPicture->pixels[i][j - 1];
					window[1] = argPicture->pixels[i][j];
					window[2] = argPicture->pixels[i][j];
					window[3] = argPicture->pixels[i][j -1];
					window[4] = argPicture->pixels[i][j];
					window[5] = argPicture->pixels[i][j];
					window[6] = argPicture->pixels[i + 1][j - 1];
					window[7] = argPicture->pixels[i + 1][j];
					window[8] = argPicture->pixels[i + 1][j];
					bubbleSort(window, 9);
					argPicture->pixels[i][j] = window[4];
				}
				// right column
				else if((j ==(argPicture->width) -1) && (i >0 && i<(argPicture->height)-1))
				{
					window[0] = argPicture->pixels[i - 1][j - 1];
					window[1] = argPicture->pixels[i - 1][j];
					window[2] = argPicture->pixels[i - 1][j];
					window[3] = argPicture->pixels[i][j - 1];
					window[4] = argPicture->pixels[i][j];
					window[5] = argPicture->pixels[i][j];
					window[6] = argPicture->pixels[i + 1][j - 1];
					window[7] = argPicture->pixels[i + 1][j];
					window[8] = argPicture->pixels[i + 1][j];
					bubbleSort(window, 9);
					argPicture->pixels[i][j] = window[4];
				}
				//bottom right corner
				else if (i==(argPicture->height)-1 && j==(argPicture->width)-1)
				{
					window[0] = argPicture->pixels[i - 1][j - 1];
					window[1] = argPicture->pixels[i - 1][j];
					window[2] = argPicture->pixels[i - 1][j];
					window[3] = argPicture->pixels[i][j - 1];
					window[4] = argPicture->pixels[i][j];
					window[5] = argPicture->pixels[i][j];
					window[6] = argPicture->pixels[i][j - 1];
					window[7] = argPicture->pixels[i][j];
					window[8] = argPicture->pixels[i][j];
					bubbleSort(window, 9);
					argPicture->pixels[i][j] = window[4];
				}
				//bottom row
				else if (i == (argPicture->height) -1 &&(j>0 && j<(argPicture->width)-1))
				{
					window[0] = argPicture->pixels[i - 1][j - 1];
					window[1] = argPicture->pixels[i - 1][j];
					window[2] = argPicture->pixels[i - 1][j + 1];
					window[3] = argPicture->pixels[i][j - 1];
					window[4] = argPicture->pixels[i][j];
					window[5] = argPicture->pixels[i][j + 1];
					window[6] = argPicture->pixels[i][j - 1];
					window[7] = argPicture->pixels[i][j];
					window[8] = argPicture->pixels[i][j + 1	];
					bubbleSort(window, 9);
					argPicture->pixels[i][j] = window[4];
				}
				//bottom left corner
				else if (i==(argPicture->height)-1 && j==0 )
				{
					window[0] = argPicture->pixels[i - 1][j];
					window[1] = argPicture->pixels[i - 1][j];
					window[2] = argPicture->pixels[i - 1][j + 1];
					window[3] = argPicture->pixels[i][j];
					window[4] = argPicture->pixels[i][j];
					window[5] = argPicture->pixels[i][j + 1];
					window[6] = argPicture->pixels[i][j];
					window[7] = argPicture->pixels[i][j];
					window[8] = argPicture->pixels[i][j + 1];
					bubbleSort(window, 9);
					argPicture->pixels[i][j] = window[4];
				}
				//left collumn
				else if (j == 0 && (i > 0 && i < (argPicture->height) - 1))
				{
					window[0] = argPicture->pixels[i - 1][j];
					window[1] = argPicture->pixels[i - 1][j];
					window[2] = argPicture->pixels[i - 1][j + 1];
					window[3] = argPicture->pixels[i][j];
					window[4] = argPicture->pixels[i][j];
					window[5] = argPicture->pixels[i][j + 1];
					window[6] = argPicture->pixels[i - 1][j];
					window[7] = argPicture->pixels[i - 1][j];
					window[8] = argPicture->pixels[i - 1][j + 1];
					bubbleSort(window, 9);
					argPicture->pixels[i][j] = window[4];
				}
				// mid
				else
				{
					window[0] = argPicture->pixels[i - 1][j - 1];
					window[1] = argPicture->pixels[i - 1][j];
					window[2] = argPicture->pixels[i - 1][j + 1];
					window[3] = argPicture->pixels[i][j - 1];
					window[4] = argPicture->pixels[i][j];
					window[5] = argPicture->pixels[i][j + 1];
					window[6] = argPicture->pixels[i + 1][j - 1];
					window[7] = argPicture->pixels[i + 1][j];
					window[8] = argPicture->pixels[i + 1][j + 1];
					bubbleSort(window, 9);
					argPicture->pixels[i][j] = window[4];
				}
			}
		}
	}
	else
	{
		printf("This picture does not exist \n");
	}
}
// filter
void bubbleSort(int table[], int size)
{
	int i, j;
	for (i = 0; i < size - 1; i++)
		for (j = 0; j < size - i - 1; j++)
			if (table[j] > table[j + 1])
				swap(&table[j], &table[j + 1]);
}
void swap(double* number1, double* number2)
{
	double temp;
	temp = *number1;
	*number1 = *number2;
	*number2 = temp;
}

void salt_and_pepper(struct picture* argPicture,int chance_to_noice)
{
	if (argPicture->pixels != NULL)
	{
		int percente;
		int up_or_down;
		for (int i = 0; i < argPicture->height; i++)
		{
			for (int j = 0; j < argPicture->width; j++)
			{
				percente = rand() % 101;
				up_or_down = rand() % 101;
				if (percente <= chance_to_noice)
				{
					if (up_or_down <= 50)
					{
						argPicture->pixels[i][j] = 0;
					}
					else
					{
						argPicture->pixels[i][j] = argPicture->max_value;
					}
				}
			}
		}
	}
	else
	{
		printf("This picture does not exist \n");
	}
}
void histogram(struct picture* argPicture)
{
	if (argPicture->pixels != NULL)
	{
		int* histogram_tab = (int*)calloc(argPicture->max_value + 1, sizeof(int));
		if (histogram != NULL)
		{
			for (int i = 0; i < argPicture->height; i++)
			{
				for (int j = 0; j < argPicture->width; j++)
				{
					histogram_tab[argPicture->pixels[i][j]]++;
				}
			}
			setlocale(LC_ALL, "polish_poland");
			char file_name[50] = "File_name";
			printf("Enter name of file:");
			scanf("%s", file_name);
			FILE* file;
			file = fopen(file_name, "w");
			if (file != NULL)
			{

				for (int i = 0; i < argPicture->max_value; i++) {

					fprintf(file, "%d; %d\n", i, histogram_tab[i]);
				}
				fclose(file);
			}
			else
			{
				printf("Error with file \n");
				return;
			}
		}
		
	}
	else
	{
		printf("This picture does not exist \n");
	}
	
}
void printDatabase(struct listOfPictures list)
{
	for (int i = 0; i < list.size; i++)
	{
		printf("%d . %s", i+1, (*(list.listOfPictures +i)).name);
		if (i == list.current)
		{
			printf("\t<--- YOU ARE HERE \n");
		}
		printf("\n");
	}
}
/*
void saveInDatabase(struct listOfPictures *database)
{
	
	database->size++;
	struct picture* temp; 
	temp = (struct picture *)realloc(database->listOfPictures, database->size * sizeof(struct picture));
	if (temp != NULL)
	{
		database->listOfPictures = temp;
	}
	else
	{
		printf("Error with realloc memorry \n");
	}
}
*/
void zwolnijdb(struct listOfPictures *db)
{
	db->current = db->size - 1;
	for (int i = 0; i < db->size; i++)
	{
		release_memory_of_picture(db->listOfPictures + db->current);
		db->current = db->current - 1;
	}
	
}
int main()
{	
	srand(time(NULL));
	struct listOfPictures database;
	struct picture* tymczasowy;

	database.size = 1;
	database.current = database.size - 1;
	database.listOfPictures = (struct picture*)calloc(database.size, sizeof(struct picture));
	if (database.listOfPictures == NULL)
	{
		printf("Memory error, cant work anymore! \n");
		return 0;
	}
	int userInput = -1;
	int scanf_flag_error;

	while (userInput !=0)
	{
		printf("\n-----MENU-----\n");
		printf("1.Load the picture.\n");
		printf("2.Save the picture.\n");
		printf("3.Release the picture's memory.\n");
		printf("4.Rotate the picture 90*k degrees.\n");
		printf("5.Add noise.\n");
		printf("6.Make negative.\n");
		printf("7.Filter.\n");
		printf("8.Save to database\n");
		printf("9.Show database\n");
		printf("10.Histogram \n");
		printf("11.Select an image from the database:\n");
		printf("0.Exit \n");
		printf("--------------\n");
		
		printf("Enter the number: ");
		scanf_flag_error = scanf("%d", &userInput);
		if (scanf_flag_error != 1)
		{
			printf("Enter valid input!!! \n");
			char catch_white_space = getchar();
		}
		else
		{
			switch (userInput)
			{
			case 0:
				return 0;
				break;
			case 1:
				read_PGM_file(database.listOfPictures + database.current); // -------------- raczej current tutaj do +databasa
				break;
			case 2:
				save_to_file_pgm(database.listOfPictures + database.current);
				break;
			case 3:
				release_memory_of_picture(database.listOfPictures + database.current);
				break;
			case 4:
				
				printf("Enter the number of turns :");
				scanf_flag_error = scanf("%d", &userInput);
				if (scanf_flag_error != 1)
				{
					printf("Enter valid input!!! \n");
					char catch_white_space = getchar();
				}
				else
				{
					if (userInput < 0)
					{
						userInput = userInput * (-1);
					}
					for (int i = 0; i < userInput%3; i++)
					{
						rotate(database.listOfPictures + database.current);
					}
				}
				break;
			case 5:
				salt_and_pepper(database.listOfPictures + database.current,5);
				break;
			case 6:
				make_negative(database.listOfPictures + database.current);
				break;
			case 7:
				medianFilter(database.listOfPictures + database.current);
				break;
			case 8:
				if(database.current == database.size -1)
				{
					database.size++;
					tymczasowy = (struct picture*)realloc(database.listOfPictures, database.size * sizeof(struct picture));
					database.listOfPictures = tymczasowy;

					database.current = database.size - 1;
				}
				else
				{
					printf("Wanna add new element, chose last index in select option \n");
				}
				
				break;
			case 9:
				printDatabase(database);
				break;
			case 10:
				histogram(database.listOfPictures + database.current);
				break;

			case 11:
				printDatabase(database);
				printf("Select the picture:");
				scanf_flag_error = scanf("%d", &userInput);
				if (scanf_flag_error != 1)
				{
					printf("Enter valid input!!! \n");
					char catch_white_space = getchar();
				}
				else
				{
					if (userInput >= 1 && userInput <= database.size - 1)
					{
						
						database.current = userInput-1;
					}
					else
					{
						printf("Wrong index, does not exist \n");
					}
				}
				break;
			default:
				printf("No options. \n");
				break;
			}
		}
	}
	return 0;
}

