#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>

#define ERROR (-1)
#define SUCCESS (0)
#define BUFFER (1024)

struct Stog;
typedef struct Stog _stog;
typedef struct Stog* Pozicija;
struct Stog{
	int el;
	Pozicija Next;
};

int Push(int, Pozicija);
int noviElement(Pozicija*);
int citajIzDatoteke(char *);
int zbroji(Pozicija);
int pomnozi(Pozicija);
int podjeli(Pozicija);
int oduzmi(Pozicija);
int ispisiStog(Pozicija);


int main (int argc, char* argv[]){
	char *buffer;
	int i = 0;
	_stog s;
	s.Next = NULL;

	buffer = (char*)malloc(BUFFER * sizeof(char));
	citajIzDatoteke(&buffer);
	
	while(buffer[i] != NULL)
	{
		if (buffer[i] >= '0' && buffer[i] <= '9')
			Push(buffer[i] - '0', &s);
		else if (buffer[i] == '+')
			zbroji(&s);
		else if (buffer[i] == '-')
			oduzmi(&s);
		else if (buffer[i] == '*')
			pomnozi(&s);
		else if (buffer[i] == '/')
			podjeli(&s);
		else
			printf("Znak nije prepoznat");
		
		i++;
	}
	printf("\n---------------------------------------------------------------\nIspis stoga:\n");
	ispisiStog(s.Next);

	free(buffer);
	getchar();
}

int ispisiStog(Pozicija p)
{
	while (p != NULL)
	{
		printf("%d\n", p->el);
		p = p->Next;
	}

}

int noviElement(Pozicija *h) {
	Pozicija q = NULL;

	*h = (Pozicija)malloc(sizeof(_stog));
	if (*h == NULL) return ERROR;

	q = *h;
	q->Next = NULL;
	
	return SUCCESS;
}

int citajIzDatoteke(char *buff[BUFFER]){

	FILE *fp;
	fp = fopen("postfix.txt","r");
	while(!feof(fp))
	{
		fscanf(fp,"%s", *buff);
	}
	return SUCCESS;
}


int Push(int x, Pozicija P) {
	int e = SUCCESS;
	Pozicija q = NULL;
	e = noviElement(&q);
	if (e) return ERROR;

	q->el = x;
	while (P->Next != NULL)
		P = P->Next;
	P->Next = q;

	q->Next = NULL;
	printf("%d\t", q->el);
	return SUCCESS;
}


int zbroji(Pozicija P)
{
	while (P->Next->Next != NULL)
		P = P->Next;
	P->el = P->el + P->Next->el;
	P->Next = NULL;

	return SUCCESS;
}

int oduzmi(Pozicija P)
{
	while (P->Next->Next != NULL)
		P = P->Next;

	P->el = P->el - P->Next->el;
	P->Next = NULL;

	return SUCCESS;
}
int pomnozi(Pozicija P)
{

	while (P->Next->Next != NULL)
		P = P->Next;
	P->el = P->el * P->Next->el;
	P->Next = NULL;

	return SUCCESS;
}
int podjeli(Pozicija P)
{
	while (P->Next->Next != NULL)
		P = P->Next;
	P->el = P->el / P->Next->el;
	P->Next = NULL;

	return SUCCESS;
}