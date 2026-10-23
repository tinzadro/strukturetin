#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct _person* position;

typedef struct _person {
	char name[50];
	char lastname[50];
	int birthyear;
	position next;
} person;

int insertAfter(position newperson, position p) {
	newperson->next = p->next;
	p->next = newperson;
	return 0;
}

int insertEnd(char* name, char* lastname, int birthyear, position head) {
	position newperson = NULL;
	newperson = (position)malloc(sizeof(person));
	if (newperson == NULL) {
		perror("Greska pri alokaciji memorije\n");
		return -1;
	}
	strcpy(newperson->name, name);
	strcpy(newperson->lastname, lastname);
	newperson->birthyear = birthyear;
	newperson->next = NULL;
	position current = head;
	while (current->next != NULL) {
		current = current->next;
	}
	insertAfter(newperson, current);
	return 0;
}

int ispis(position p) {
	p = p->next;
	while (p != NULL) {
		printf("%s %s %d\n", p->name, p->lastname, p->birthyear);
		p = p->next;
	}
	return 0;
}

// a) dodavanje novog elementa iza odredenog elementa
int insertAfterLastname(char* trazeno, char* name, char* lastname, int birthyear, position p) {
	position q = p->next;

	while (q != NULL) {
		if (strcmp(q->lastname, trazeno) == 0) {
			position newperson = (position)malloc(sizeof(person));
			if (newperson == NULL) return -1;

			strcpy(newperson->name, name);
			strcpy(newperson->lastname, lastname);
			newperson->birthyear = birthyear;

			insertAfter(newperson, q);
			return 0;
		}
		q = q->next;
	}
	return 1;
}

//b) dodavanje novog elementa ispred odredenog elementa
int insertBeforeLastname(char* trazeno, char* name, char* lastname, int birthyear, position p) {
	while (p->next != NULL) {
		if (strcmp(p->next->lastname, trazeno) == 0) {
			position newperson = (position)malloc(sizeof(person));
			if (newperson == NULL) return -1;

			strcpy(newperson->name, name);
			strcpy(newperson->lastname, lastname);
			newperson->birthyear = birthyear;

			insertAfter(newperson, p);

			return 0;
		}
		p = p->next;
	}
	return 1;
}

//c) sortiranje po prezimenu
int sort(position p) {
	position i = NULL;
	position q = NULL;

	for (i = p->next; i != NULL; i = i->next) {
		for (q = p->next; q->next != NULL; q=q->next) {
			if (strcmp(q->lastname, q->next->lastname) > 0) {
				person temp = *q;
				strcpy(q->name, q->next->name);
				strcpy(q->lastname, q->next->lastname);
				q->birthyear = q->next->birthyear;

				strcpy(q->next->name, temp.name);
				strcpy(q->next->lastname, temp.lastname);
				q->next->birthyear = temp.birthyear;
			}
		}
	}
	return 0;
}

//d) upis u datoteku
int writeFile(char* file, position p) {
	FILE* fp = fopen(file, "w");
	if (fp == NULL) {
		perror("Greska pri otvaranju datoteke\n");
		return -1;
	}
	p = p->next;
	while (p != NULL) {
		fprintf(fp, "%s %s %d\n", p->name, p->lastname, p->birthyear);
		p = p->next;
	}
	fclose(fp);
	return 0;
}

//e) citanje iz datoteke
int read(char* file, position p) {
	FILE* fp = fopen(file, "r");
	if (fp == NULL) return -1;
	char name[50];
	char lastname[50];
	int birthyear;
	while (fscanf(fp, "%49s %49s %d", name, lastname, &birthyear) == 3) {
		if (insertEnd(name, lastname, birthyear, p) != 0) {
			fclose(fp);
			return 1;
		}
		}
	fclose(fp);
	return 0;
	}

int deleteAll(position p) {
	position curr = p->next;
	while (curr != NULL) {
		position next = curr->next;
		free(curr);
		curr = next;
	}
	p->next = NULL;
	return 0;
}

int main() {
	person head = { .name = "", .lastname = "", .birthyear = 0, .next = NULL };
	position p = &head;

	insertEnd("Ivo", "Ivic", 2003, p);
	insertEnd("Ana", "Anic", 2002, p);
	insertEnd("Marko", "Maric", 2000, p);

	printf("Pocetna lista: \n");
	ispis(p);

	//dodavanje iza Ive
	insertAfterLastname("Ivic", "Petra", "Peric", 2001, p);

	//dodavanje ispred Marka
	insertBeforeLastname("Maric", "Iva", "Ivanic", 1999, p);

	printf("Nakon dodavanja: \n");
	ispis(p);


	//sortiranje po prezimenu
	sort(p);
	printf("Sortirano: \n");
	ispis(p);

	//spremanje u datoteku
	if (writeFile("dat.txt", p) == 0) {
		printf("\nLista spremljena u datoteku.\n");
		ispis(p);
	}

	deleteAll(p);
}
