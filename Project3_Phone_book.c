#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
//#include<stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>

struct Contact {
	char name[31];
	char surname[51];
	char group[31];
	char phoneNumber[13];
};
struct Node {
	struct Contact contact;
	struct Node* next;
	struct Node* previous;
};
void findInContact(struct Node* head, char* userString);
int add_to_the_end(struct Node** head, struct Contact contact) {
	struct Node* temporary = NULL;
	struct Node* current = NULL;
	temporary = (struct Node*)calloc(1, sizeof(struct Node));
	if (temporary != NULL) {
		temporary->contact = contact;
		if (*head == NULL) {
			*head = temporary;
		}
		else {
			for (current = *head; current->next != NULL; current = current->next);
				temporary->previous = current;
				current->next = temporary;
		}
		return 0; /// all fine code
	}
	return -1; // error with allocation code
}

int display(struct Node *head) {
	int position = 1;
		while (head != NULL) {
			printf("\t Contact:%d\nName:%s\nSurname:%s\nGroup:%s\nPhone number:%s\n", position++, head->contact.name, head->contact.surname, head->contact.group, head->contact.phoneNumber);
			head = head->next;
		}
		return position-1;
}
void release(struct Node** head){
	while (*head !=NULL)
	{
		struct Node* temporary = *head;
		*head = (*head)->next;
		free(temporary);
	}
}
int write_from_csv_file(char* fileName, struct Node** head) {
	FILE* file;
	struct Contact csvContact;
	if ((file = fopen(fileName, "rt")) != NULL) {
		while (fscanf(file, "%[^;];%[^;];%[^;];%[^;];\n", csvContact.name, csvContact.surname, csvContact.group, csvContact.phoneNumber) == 4) {
			add_to_the_end(head, csvContact);
		}
		fclose(file);
		return 0;
	}
	return -1;
}
void swap(struct Node** head)
{
	struct Node* temporary = *head;
	if (temporary != NULL && temporary->next != NULL) {
		//1
		if (temporary->previous != NULL){
			temporary->previous->next = temporary->next;
		}
		else {
			*head = temporary->next;
		}
		temporary->next->previous = temporary->previous;
		//2
		temporary->previous = temporary->next;
		temporary->next = temporary->previous->next;
		//3
		temporary->previous->next = temporary;
		if (temporary->next != NULL) {
			temporary->next->previous = temporary;
		}
	}
}
void sort(struct Node** head, int field, int direction)
{
	struct Node* temporary;
	int offset;
	int variable = 1;
	switch (field) {
	case 0:offset = offsetof(struct Node, contact.name);
		break;
	case 1:offset = offsetof(struct Node, contact.surname);
		break;
	default:
		offset = offsetof(struct Node, contact.group);
		break;
	}
	while (variable) {
		variable = 0;
		temporary = *head;
		while (temporary != NULL && temporary->next != NULL) {
			if (((strcoll((char*)temporary + offset, (char*)temporary->next + offset)) * direction) > 0) {
				variable = 1;
				swap(&temporary);
			}
			temporary = temporary->next;
		}
	}
}
struct Contact getContact(){
	struct Contact temporaryContact;

	printf("Enter name:");
	scanf("%30s", temporaryContact.name);

	printf("Enter surname:");
	scanf("%30s", temporaryContact.surname);

	printf("Enter group:");
	scanf("%30s", temporaryContact.group);

	printf("Enter phone number:");
	scanf("%30s", temporaryContact.phoneNumber);

	return temporaryContact;
}
char* getFileName()
{
	static char fileName[50];
	printf("Enter file name:");
	scanf("%s", fileName);
	return fileName;
}
void print_Sort_by_menu() {
	printf("Sort by:\n");
	printf("1.Name.\n");
	printf("2.Surname.\n");
	printf("3.Group.\n");
	printf("---------------------\n");
	printf("Select option:");
}
void printfSingleContact(struct Node* head, int position)
{
	printf("\t Contact:%d\nName:%s\nSurname:%s\nGroup:%s\nPhone number:%s\n", position++, head->contact.name, head->contact.surname, head->contact.group, head->contact.phoneNumber);
}
void find(struct Node* head){
	char userInputString[51]="";
	int position = 0;
	int singleChar;
	while(position < 50) {
		
		printf("\nEnterd world:%s \n", userInputString);
		printf("Enter the character:");
		singleChar = getch();
		system("cls");
		printf("\n");
		if (singleChar <= ' ') {
			break;
		}
		userInputString[position++] = singleChar;
		userInputString[position] = 0;
		
		findInContact(head, userInputString);
	}
}
char* changeToLowerString(char* stringOutput, char* stringInput)
{
	char* position = stringOutput;
	while (*stringInput != 0)
	{
		*position = tolower(*stringInput);
		position++;
		stringInput++;
	}
	*position = 0;
	return stringOutput;
}
void findInContact(struct Node* head, char *userString)
{
	int position = 1;
	char lowerUserInput[50];
	char lowerContact[51];
	strcpy(lowerUserInput, changeToLowerString(lowerUserInput, userString));
	while (head != NULL) {
		if (strstr(changeToLowerString(lowerContact, head->contact.name), lowerUserInput) != NULL ||
			strstr(changeToLowerString(lowerContact, head->contact.surname), lowerUserInput) != NULL ||
			strstr(changeToLowerString(lowerContact, head->contact.group), lowerUserInput) != NULL)
		{
			printfSingleContact(head, position++);
		}
		head = head->next;
	}
}
void deleteContact(struct Node** node)
{
	int userInput;
	int numberOfContacts = display(*node);
	if (*node == NULL) {
		printf("Empty list of contact");
		return;
	}
	printf("Which contact want to delete?:");
	if (scanf("%d", &userInput) != 1) {
		printf("Enter valid input!!! \n");
		getchar();
	}
	else {
		if (userInput >= 1 && userInput <= numberOfContacts)
		{
			struct Node* temporary = *node;
			if (userInput == 1)
			{
				
				*node = (*node)->next;
				free(temporary);
			}
			else if (userInput == numberOfContacts) {
				for (int i = 1; i <= userInput; i++)
				{
					temporary = temporary->next;
				}
				//temporary->previous->next = NULL;
				free(temporary);
			}
			else {
				for (int i = 1; i <= userInput; i++)
				{
					temporary = (temporary)->next;
				}
				temporary->previous->next = temporary->next;
				temporary->next->previous = temporary->previous;
				free(temporary);
			}
		}
		else {
			printf("Wrong option. \n");
		}
	}
	struct Node* temporary = *node;
	*node = (*node)->next;
	free(temporary);
}
int main()
{
	struct Node* head = NULL;
	
	int userInput;
		do{
			printf("\t***MENU***\n");
			printf("---------------------\n");
			printf("1.Add element to the end.\n");
			printf("2.Display list.\n");
			printf("3.Release memory.\n");
			printf("4.Get elements from csv file.\n");
			printf("5.Sort.\n");
			printf("6.Find the contact.\n");
			printf("7.Delete the contact.\n");
			printf("10.Exit.\n");
			printf("---------------------\n");
			printf("Select option:");
			if (scanf("%d", &userInput) != 1){
				printf("Enter valid input!!! \n");
				getchar();
			}
			else {
				switch (userInput){
				case 1:
					add_to_the_end(&head, getContact()); //<<--- dodaj co zwraca ta funkcja
					break;
				case 2:
					display(head);
					break;
				case 3:
					release(&head);
					break;
				case 4:
					write_from_csv_file(getFileName(), &head);
					break;
				case 5:
					//--------------------------------------------------
					printf("1.Sort ascending.\n");
					printf("2.Sort descending.\n");
					printf("---------------------\n");
					printf("Select option:");
					if (scanf("%d", &userInput) != 1) {
						printf("Enter valid input!!! \n");
						getchar();
					}
					else {
						switch (userInput){
							//--------------------------------------------------
						case 1:
							print_Sort_by_menu();
							if (scanf("%d", &userInput) != 1) {
								printf("Enter valid input!!! \n");
								getchar();
							}
							else {
								switch (userInput){
									//--------------------------------------------------
								case 1:
									sort(&head, 0, 1);
									break;
								case 2:
									sort(&head, 1, 1);
									break;
								case 3:
									sort(&head, 2, 1);
									break;
								default:
									printf("No options.\n");
									break;
									//--------------------------------------------------
								}
							}
							break;
							//--------------------------------------------------
						case 2:
							print_Sort_by_menu();
							if (scanf("%d", &userInput) != 1) {
								printf("Enter valid input!!! \n");
								getchar();
							}
							else {
								switch (userInput) {
								case 1:
									sort(&head, 0, -1);
									break;
								case 2:
									sort(&head, 1, -1);
									break;
								case 3:
									sort(&head, 2, -1);
									break;
								default:
									printf("No options.\n");
									break;
								}
							}
							break;
						default:
							printf("No options.\n");
							break;
						}//--------------------------------------------------
					}
					break;
					//--------------------------------------------------
				case 6:
					
					find(head);
					break;
				case 7:
					deleteContact(&head);
					break;
				case 10:
					return;
				default:
					printf("No options.\n");
					break;
				}
				
			}
		} while (userInput != 0);
	return 0;
}