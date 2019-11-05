#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define SUCCESS (0)
#define ERROR (-1)
#define BUFFER (1024)

struct Polinom;
typedef struct Polinom _POLI;
typedef struct Polinom* Pozicija;

struct Polinom {
	int koeficijent;
	int eksponent;
	Pozicija Next;
};

int noviElement(Pozicija*);
int citajIzDatoteke(Pozicija);
int ispisPolinoma(Pozicija);
int zbrojiPolinome(Pozicija, Pozicija, Pozicija);
int mnoziPolinome(Pozicija, Pozicija, Pozicija);

int main(int argc, char* argv[]) {
	int e = SUCCESS;
	struct Polinom P1, P2, S, P;

	P1.Next = NULL;
	P2.Next = NULL;
	S.Next = NULL;
	P.Next = NULL;
	
	printf("\n*** PRVI POLINOM ***\n");
	e = citajIzDatoteke(&P1);
	if (e) return e;
	printf("\nPolinom 1:\nP1(x)= ");
	ispisPolinoma(P1.Next);

	printf("\n\n*** DRUGI POLINOM ***\n");
	e = citajIzDatoteke(&P2);
	if (e) return e;
	printf("\nPolinom 2:\nP2(x)= ");
	ispisPolinoma(P2.Next);

	printf("\n\n\nZbroj(suma) polinoma:\nS(x)= ");
	zbrojiPolinome(P1.Next, P2.Next, &S);
	if (e) return e;
	ispisPolinoma(S.Next);

	printf("\n\n\nUmnozak polinoma:\nP(x)= ");
	mnoziPolinome(P1.Next, P2.Next, &P);
	if (e) return e;
	ispisPolinoma(P.Next);

	printf("\n\n");
	return e;
}

int noviElement(Pozicija* h) {
	Pozicija q = NULL;
	*h = (Pozicija)malloc(sizeof(_POLI));
	if (h == NULL)
		return ERROR;
	q = *h;
	q->Next = NULL;

	return SUCCESS;
}

int citajIzDatoteke(Pozicija P) {

	int e = SUCCESS;
	Pozicija q = NULL;
	Pozicija tmp = NULL;
	char* ime = NULL;
	FILE* fp = NULL;

	ime = (char*)malloc(sizeof(char) * BUFFER);
	if (ime == NULL) 
		return ERROR;
	memset(ime, '\0', BUFFER);

	printf("\nUnesite ime datoteke: ");
	scanf(" %s", ime);

	if (strchr(ime, '.') == NULL)
		strcat(ime, ".txt");

	fp = fopen(ime, "r");

	if (fp == NULL)
	{
		printf("\ndatoteka %s nije otvorena.\n", ime);
		return ERROR;
	}

	while (!feof(fp))
	{
		e = noviElement(&q);
		if (e){
			printf("\nMemorija nije alocirana.\n");
			break;
		}
		fscanf(fp, " %d %d", &q->koeficijent, &q->eksponent);

		//sortiranje eksponenata
		tmp = P;
		while (tmp->Next != NULL && tmp->Next->eksponent > q->eksponent)
			tmp = tmp->Next;

		//ako imamo 2 jednaka eksponenta, zbrojit æemo im koeficijente

		if (tmp->Next != NULL && tmp->Next->eksponent == q->eksponent){
			tmp->Next->koeficijent += q->koeficijent;
			free(q);
			if (tmp->Next->koeficijent == 0){
				q = tmp->Next;
				tmp->Next = q->Next;
				free(q);
			}
		}
		else{
			q->Next = tmp->Next;
			tmp->Next = q;
		}
	}
	fclose(fp);

	return e;
}

int ispisPolinoma(Pozicija P) {
	int prviEl = 1;
	while (P != NULL){
		if (prviEl){
			printf("%dx^%d", P->koeficijent, P->eksponent);
			prviEl = 0;
		}
		else{
			if (P->koeficijent > 0)
				printf(" +%dx^%d", P->koeficijent, P->eksponent);
			else
				printf(" %dx^%d", P->koeficijent, P->eksponent);
		}

		P = P->Next;
	}

	return SUCCESS;
}

int zbrojiPolinome(Pozicija P1, Pozicija P2, Pozicija S) {
	int e = SUCCESS;
	Pozicija q = NULL;
	Pozicija tmp = NULL;

	while (P1 != NULL && P2 != NULL) {
		e = noviElement(&q);
		if (e) {
			printf("Memorija nije alocirana.\n");
			break;
		}

		if (P1->eksponent < P2->eksponent) {
			q->eksponent = P2->eksponent;
			q->koeficijent = P2->koeficijent;
			P2 = P2->Next;
		}

		else if (P1->eksponent > P2->eksponent) {
			q->eksponent = P1->eksponent;
			q->koeficijent = P1->koeficijent;
			P1 = P1->Next;
		}

		else {
			q->eksponent = P1->eksponent;
			q->koeficijent = P1->koeficijent + P2->koeficijent;
			P1 = P1->Next;
			P2 = P2->Next;
		}
		q->Next = S->Next;
		S->Next = q;
		S = q;
	}

		if (P1 == NULL)
			tmp = P1;
		else tmp = P2;

		while (tmp != NULL) {
			e = noviElement(&q);
			if (e) {
				printf("Memorija nije alocirana.\n");
				break;
			}

			q->eksponent = tmp->eksponent;
			q->koeficijent = tmp->koeficijent;

			q->Next = S->Next;
			S->Next = q;
			S = q;
			tmp = tmp->Next;
		}
	
	return e;
}

int mnoziPolinome(Pozicija P1, Pozicija P2, Pozicija P) {
	int e = SUCCESS;
	Pozicija q = NULL;
	Pozicija tmp = NULL;
	Pozicija i = NULL;
	Pozicija j = NULL;

	i = P1;
	while (i != NULL){
		j = P2;
		while (j != NULL){
			e = noviElement(&q);
			if (e){
				printf("\nMemorija nije alocirana.\n");
				break;
			}

			q->eksponent = i->eksponent + j->eksponent;
			q->koeficijent = i->koeficijent * j->koeficijent;

			tmp = P;

			while (tmp->Next != NULL && tmp->Next->eksponent > q->eksponent)
				tmp = tmp->Next;

			if (tmp->Next != NULL && tmp->Next->eksponent == q->eksponent){
				tmp->Next->koeficijent += q->koeficijent;
				free(q);

				if (tmp->Next->koeficijent == 0){
					q = tmp->Next;
					tmp->Next = q->Next;

					free(q);
				}
			}
			else{
				q->Next = tmp->Next;
				tmp->Next = q;
			}

			j = j->Next;
		}
		i = i->Next;
	}

	return e;
}