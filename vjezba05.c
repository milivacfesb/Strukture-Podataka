#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ERROR (-1)
#define SUCCESS (0)
#define BUFFER (1024)

struct Cvor;
typedef struct Cvor _cvor;
typedef struct Cvor* Pozicija;

struct Cvor{
	int element;
	Pozicija Next;
};

int noviElement(Pozicija*);
int citajIzDatoteke(Pozicija);
int ispis(Pozicija);
int unija(Pozicija, Pozicija, Pozicija);
int presjek(Pozicija, Pozicija, Pozicija);

int main(int argc, char* argv[]){
	int e = SUCCESS;
	struct Cvor L1, L2, U, P;

	L1.Next=NULL;
	L2.Next=NULL;
	U.Next=NULL;
	P.Next=NULL;

	printf("\n***Prva lista***\n");
	e=citajIzDatoteke(&L1);
	if(e) return ERROR;
	ispis(L1.Next);

	printf("\n***Druga lista***\n");
	e=citajIzDatoteke(&L2);
	if(e) return ERROR;
	ispis(L2.Next);

	printf("\n\n\nUnija:\n ");
	e = unija(L1.Next, L2.Next, &U);
	if (e) return e;
	ispis(U.Next);

	printf("\n\n\nPresjek:\n ");
	e = presjek(L1.Next, L2.Next, &P);
	if (e) return e;
	ispis(P.Next);

	printf("\n\n");
	return e;
}


int noviElement(Pozicija *h){
	Pozicija q=NULL;
	*h=(Pozicija)malloc(sizeof(_cvor));
	if(h==NULL) return ERROR;
	q=*h;
	q->Next=NULL;

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
		fscanf(fp, " %d", &q->element);

		tmp = P;
		while (tmp->Next != NULL && tmp->Next->element >= q->element)
			tmp = tmp->Next;

		q->Next = tmp->Next;
		tmp->Next = q;
	}
		
	fclose(fp);

	return e;
}	

int ispis(Pozicija P){
	if(P==NULL) return ERROR;
	while(P!=NULL){
		printf(" %d", P->element);
		P=P->Next;
	}

	return SUCCESS;
}

int unija(Pozicija L1, Pozicija L2, Pozicija U){
	int e=SUCCESS;
	Pozicija q = NULL;
	Pozicija tmp = NULL;
	
	//setamo po listama istovremeno (dok s jednom listom ne dodjemo do kraja)
	while(L1 != NULL && L2 != NULL)
	{
		e = noviElement(&q);
		if (e)
		{
			printf("\nMemorija nije alocirana.\n");
			break;
		}
		else
		{
			//ako je element od prve liste veci, prvo njega saljemo u uniju 
			//i idemo na slj.element od prve liste
			if (L1->element > L2->element)
			{
				q->element = L1->element;
				L1 = L1->Next;
			}
			//ako je element od druge liste veci, prvo njega saljemo u uniju 
			//i idemo na slj.element od druge liste
			else if (L1->element < L2->element)
			{
				q->element = L2->element;
				L2 = L2->Next;
			}
			//ako su radi o istom elementu nije ga potrebno 
			//slati dva puta u uniju, pa ga saljemo samo iz prve liste
			//i idemo na slj element od obje liste
			else
			{
				q->element = L1->element;
				L1 = L1->Next;
				L2 = L2->Next;
			}

			q->Next = U->Next;
			U->Next = q;
			U = q;
			
		}
	}
	//provjeravamo s kojom smo listom dosli do kraja i stavljamo drugu u tmp listu
	if (L1!=NULL)
		tmp = L1;
	else
		tmp = L2;

	//ubacujemo u uniju preostale elemente iz liste koja je ostala
	while (tmp)
	{
		e = noviElement(&q);
		if (e)
		{
			printf("\nMemorija nije alocirana.\n");
			break;
		}

		q->element= tmp->element;

		
		q->Next = U->Next;
		U->Next = q;
		U = q;
		
		tmp = tmp->Next;
	}

	return SUCCESS;
}

int presjek(Pozicija L1, Pozicija L2, Pozicija P)
{
	int e = SUCCESS;
	Pozicija q = NULL;
	
	while (L1!=NULL && L2!=NULL)
	{
		//ako elementi nisu jednaki, ne zanimaju nas pa
		//idemo dalje i s jednom i s drugom listom
		if (L1->element > L2->element)
			L1 = L1->Next;
		else if (L1->element < L2->element)
			L2 = L2->Next;
		//ako su elementi jednaki, tek onda ih ubacujemo u presjek
		//opet je dovoljno prebaciti samo jednoga (npr iz prve liste)
		else
		{
			e = noviElement(&q);
			if (e)
			{
				printf("\nMemorija nije alocirana.\n");
				break;
			}

			q->element = L1->element;

			q->Next = P->Next;
			P->Next = q;
			P = q;

			L1 = L1->Next;
			L2 = L2->Next;
		}
	}
	//u ovom slucaju nam ne treba tmp lista, jer kad dodjemo do kraja jedne liste
	//to znaci da vise nema elemenata koji bi trebali uci u presjek

	return SUCCESS;
}
