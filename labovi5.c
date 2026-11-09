#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct stack* position;

typedef struct stack {
	int number;
	position next;
} stack;

//dodavanje broja na vrh stoga
int push(int number, position head) { //head je prvi
	position new = (position)malloc(sizeof(stack));
	if (new == NULL) return -1;

	new->number = number; //broj u novi element
	new->next = head->next; //dodaje na pocetak liste
	head->next = new;
	//novi elementi uvijek dolaze na pocetak

	return 0;
}

//micanje broja sa pocetka
int pop(position head, int* number) { //broj se brise s vrha liste ali se ne gubi jer se sacuva preko pokazivaca number
	if (head->next == NULL) return -1; //ako nema nijedan broj na stogu

	position temp = head->next; // pokazivac na prvi element stoga
	*number = temp->number; // sprema broj prije brisanja
	head->next = temp->next; // prvi element je sad onaj koji je prvi pronadjen
	free(temp); // brise prosli prvi element iz memorije

	return 0;
}

int racun(char* file, position head, int* rez) { //funkcija za racunanje
	FILE* fp = fopen(file, "r");
	if (fp == NULL) return -1;
	char c;
	int a, b, r; //a i b brojevi, r rezultat

	while (fscanf(fp, " %c", &c) == 1) { //cita jedan po jedan znak iz datoteke
		if (isdigit((unsigned char)c)) { //isdigit provjerava je li procitani znak znamenka
			if (push(c - '0', head) == 1) { //c-'0' pretvara ASCII znak u brojcanu vrijednost; ASCII vrijednosti su 0=48, 1=49 itd
				fclose(fp);
				return -1;
			}
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/') { //provjera je li znak matematicki operator
			if (pop(head, &b) == -1 || pop(head, &a) == -1) { // pop skida dva broja s vrha stoga (a i b), ako u stogu nema dovoljno elemenata, vraca -1 i ulazi u blok za gresku (kolegica objasnila)
				fclose(fp); // zatvara datoteku ako je doslo do greske
				return -1;
			}
			switch (c) { 
			case '+':
				r = a + b;
				break;
			case '-':
				r = a - b;
				break;
			case '*':
				r = a * b;
				break;
			case '/':
				if (b == 0) { //provjera dijeljenja s nulom
					fclose(fp);
					return -1;
				}
				r = a/b;
				break;
			}

			if (push(r, head) == -1) { //vracanje rezultata na vrh stoga
				fclose(fp);
				return -1;
			}
		}

	}
	pop(head, rez); // pop skida konacni rezultat s vrha stoga i sprema ga u rez
	fclose(fp);
	return 0;
}

//brisanje elemenata
void deleteAll(position head) {
	int broj; //varijabla jer pop treba neku adresu u koju ce poslat obrisane elemente
	while (head->next != NULL) { //petlja se vrti dok ima elemenata
		pop(head, &broj);
	}
}

int main() {
	stack head = { .number = 0, .next = NULL };
	int rez;

	if (racun("dat.txt", &head, &rez) == 0) {
		printf("Rezultat: %d\n", rez);
	}
	else printf("greska");

	deleteAll(&head);

	return 0;
}