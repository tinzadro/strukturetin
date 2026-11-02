#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct poly* position;

typedef struct poly {
	int coeff;
	int pow;
	position next;
} polynom;

int insertBegin(position newpoly, position q){
	newpoly->next = q->next;
	q->next = newpoly;
	return 0;
}

//sortirani unos
int sort(int coeff, int pow, position head) {
	position q = head;

	if (coeff == 0) return 0;

	position newpoly = (position)malloc(sizeof(polynom));
	if (newpoly == NULL) return -1;
	newpoly->coeff = coeff;
	newpoly->pow = pow;
	newpoly->next = NULL;

	while (q->next != NULL && q->next->pow > newpoly->pow) {
		q = q->next;
	}

	if (q->next != NULL && q->next->pow == newpoly->pow) {
		q->next->coeff += newpoly->coeff;

		if (q->next->coeff == 0) {
			position temp = q->next;
			q->next = temp->next;
			free(temp);
		}
		free(newpoly);
		return 0;
	}
	insertBegin(newpoly, q);
	return 0;
}

int readFile(FILE* fp, position head) {
	int n, coeff, pow;
	if (fscanf(fp, "%d", &n) != 1 || n < 0) return -1;
	
	for (int i = 0; i < n; i++) {
		if (fscanf(fp, "%d %d", &coeff, &pow) != 2) return -1;
		if (sort(coeff, pow, head) != 0) return -1;
	}
	return 0;
}

int ispis(position head) {
	position q = head->next;
	if (q == NULL) {
		printf("0\n");
		return 0;
	}
	while (q != NULL) {
		printf("(%d)x^%d", q->coeff, q->pow);
		if (q->next != NULL) printf(" + ");
		q = q->next;
	}
	printf("\n");
	return 0;
}

int zbrajanje(position first, position second, position result) {
	position q = first->next;
	while (q != NULL) {
		if (sort(q->coeff, q->pow, result) != 0) return -1;
		q = q->next;
	}

	q = second->next;
	while (q != NULL) {
		if (sort(q->coeff, q->pow, result) != 0) return -1;
		q = q->next;
	}
	return 0;
}

int mnozenje(position first, position second, position result) {
	position p = first->next;
	while (p != NULL) {
		position q = second->next;

		while (q != NULL) {
			int coeff = p->coeff * q->coeff;
			int pow = p->pow + q->pow;
			if (sort(coeff, pow, result) != 0) return -1;

			q = q->next;
		}
		p = p->next;
	}
	return 0;
}

int deleteAll(position head) {
	position q = head->next;
	while (q != NULL) {
		position temp = q;
		q = q->next;
		free(temp);
	}
	head->next = NULL;
	return 0;
}

int main() {
	polynom head = { 0,0,NULL };
	polynom headFirst = head;
	polynom headSecond = head;
	position q = &headFirst;

	polynom headSum = { 0,0,NULL };
	polynom headProduct = { 0,0,NULL };

	FILE* fp = fopen("data.txt", "r");

	if (fp == NULL) {
		printf("greska pri otvaranju datoteke\n");
		return 1;
	}

	if (readFile(fp, &headFirst) != 0 || readFile(fp, &headSecond) != 0) {
		printf("greska pri ucitavanju\n");
		fclose(fp);
		deleteAll(&headFirst);
		deleteAll(&headSecond);
		return 1;
	}
	fclose(fp);

	printf("prvi polinom: ");
	ispis(&headFirst);

	printf("drugi polinom: ");
	ispis(&headSecond);

	if (zbrajanje(&headFirst, &headSecond, &headSum) == 0) {
		printf("\nzbroj: ");
		ispis(&headSum);
	}
	else printf("greska kod zbrajanja");

	if (mnozenje(&headFirst, &headSecond, &headProduct) == 0) {
		printf("\numnozak: ");
		ispis(&headProduct);
	}
	else printf("greska pri mnozenju");

	deleteAll(&headFirst);
	deleteAll(&headSecond);
	deleteAll(&headSum);
	deleteAll(&headProduct);

	return 0;
}