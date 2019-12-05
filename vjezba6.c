#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define ERROR (-1)
#define SUCCESS (0)
#define BUFFER (1024)

struct Cvor;
typedef struct Cvor _Cvor;
typedef struct Cvor* Pozicija;

struct Cvor {
	int el;
	Pozicija Next;
};

int noviElement(Pozicija*);
int slucajniBroj(int, int);
int pushStog(Pozicija, int);
int pushRed(Pozicija, int);
int pop(Pozicija);
int ispis(Pozicija);

int main(int argc, char* argv[]) {
	int x = 0;
	char* odabir = NULL;
	struct Cvor S, R;
	S.Next = NULL;
	R.Next = NULL;

	srand((unsigned)time(NULL));

	odabir = (char*)malloc(sizeof(char)*BUFFER);
	if (odabir == NULL) return ERROR;
	memset(odabir, '\0', BUFFER);

	while (strcmp(odabir, "izlaz") != 0) {
		memset(odabir, 0, BUFFER);
		printf("*** Menu ***\n1 - Push u stog\n2 - Push u red\n3 - Pop iz stoga\n4 - Pop iz reda\nUnesite 'izlaz' da biste izasli iz izbornika.\n");
		printf("Odabir: ");
		scanf(" %s", odabir);

		if (strcmp(odabir, "1") == 0) {
			printf("\nOdabrali ste - push u stog.\n");
			if(x==0)
				x = slucajniBroj(10, 100);
			pushStog(&S, x);
			x = 0;
			ispis(S.Next);
		}

		else if (strcmp(odabir, "2") == 0) {
			printf("\nOdabrali ste - push u red.\n");
			if (x == 0)
				x = slucajniBroj(10, 100);
			pushRed(&R, x);
			x = 0;
			ispis(R.Next);
		}
		else if (strcmp(odabir, "3") == 0) {
			printf("\nOdabrali ste - pop iz stoga.\n");
			pop(&S);
			ispis(S.Next);
		}

		else if (strcmp(odabir, "4") == 0) {
			printf("\nOdabrali ste - pop iz reda.\n");
			pop(&R);
			ispis(R.Next);
		}

		else printf("\n Pogresan unos.\n");

	}

	return SUCCESS;
}

int noviElement(Pozicija *h) {
	Pozicija q = NULL;

	*h = (Pozicija)malloc(sizeof(_Cvor));
	if (*h == NULL) return ERROR;

	q = *h;
	q->Next = NULL;
}

int slucajniBroj(int min, int max) {
	return rand() % (max - min) + min;
}

int pushStog(Pozicija P, int x){
	int e = SUCCESS;
	Pozicija q = NULL;

	e = noviElement(&q);
	if (e) return ERROR;

	q->el = x;

	q->Next = P->Next;
	P->Next = q;

	return SUCCESS;
}

int pushRed(Pozicija P, int x) {
	int e = SUCCESS;
	Pozicija q = NULL;

	while (P->Next != NULL) {
		P = P->Next;
	}

	e = noviElement(&q);
	if (e) return ERROR;

	q->el = x;

	q->Next = P->Next;
	p->Next = q;

	return SUCCESS;
}

int Pop(Pozicija P)
{
	Pozicija tmp = NULL;

	tmp = P->Next;

	if (tmp == NULL) return ERROR;

	P->Next = tmp->Next;
	printf("\r\nBrise se : %d\r\n", tmp->el);
	free(tmp);

	return SUCCESS;
}
