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
struct Cvor {
	int el;
	Pozicija R;
	Pozicija L;
};

Pozicija unesi(Pozicija, int);
int ispisi(Pozicija);
Pozicija traziNajmanji(Pozicija);
Pozicija traziNajveci(Pozicija);
Pozicija brisi(Pozicija, int);
Pozicija trazi(Pozicija, int);

int main(int argc, char* argv[]) {
	Pozicija root = NULL;
	int izbor = 0;
	int br = 0;
	Pozicija a = NULL;


	while (izbor!=5) {
		printf("1 - dodavanje novog elementa\n2 - ispis\n3 - brisanje elementa\n4 - trazenje odredjenog elementa\n5 - izlaz\n");
		printf("\nOdabir: ");
		scanf(" %d", &izbor);

		if (izbor==1) {
			printf("\nUnesite broj za unosenje: ");
			scanf(" %d", &br);

			root = unesi(root, br);
		}
		else if (izbor==2) {
			if (root == NULL) {
				printf("\nStablo je prazno.\n");
				return ERROR;
			}
			else {
				ispisi(root);
				printf("\n");
			}
		}
		else if (izbor==3) {
			if (root == NULL) {
				printf("\nStablo je prazno\n");
				return ERROR;
			}
			else {
				printf("\nUnesite element za brisanje: ");
				scanf(" %d", &br);

				root = brisi(root, br);
			}
		}
		else if (izbor==4) {
			if (root == NULL) {
				printf("\nStablo je prazno\n");
				return ERROR;
			}
			else {
				printf("\nUnesite element koji zelite pronaci: ");
				scanf(" %d", &br);
				a = trazi(root, br);
				if (a != NULL)
					printf("\nElement %d je pronadjen na adresi 0x%xh\n", a->el, (unsigned int)a);
				else printf("\nElement nije pronadjen.\n");
			}
		}
		else if (izbor==5) {
			printf("\nIzlaz iz programa.\n");
		}
		else printf("\nOdaberite nesto iz izbornika!\n");
	}

	
	return SUCCESS;
}

Pozicija unesi(Pozicija P, int br){
	//ako je stablo prazno, alociramo memoriju za novi element i stavljamo ga u stablo
	if (P == NULL){
		P = (Pozicija)malloc(sizeof(struct Cvor));
		if (P == NULL){
			printf("\nGreska kod alokacije memorije.\n");
			return P;
		}

		P->el = br;
		P->L = NULL;
		P->R = NULL;
	}
	//ako vec imamo element u stablu, provjeravamo je li onaj kojeg unosimo veci ili manji
	//ako je manji postat ce lijevo dijete, a ako je veci desno dijete
	else if (P->el < br)
		P->R = unesi(P->R, br);
	else if (P->el > br)
		P->L = unesi(P->L, br);
	else
		printf("\nElement vec postoji.\n");
	return P;
}

int ispisi(Pozicija P) {
	if (P != NULL) {
		printf(" %d", P->el);
		ispisi(P->L);
		ispisi(P->R);
	}

	return SUCCESS;
}

Pozicija traziNajmanji(Pozicija P){
	if (P != NULL && P->L != NULL){
		return traziNajmanji(P->L);
	}

	return P;
}


Pozicija brisi(Pozicija P, int br){
	Pozicija tmp;
	if (P == NULL){
		printf("\nElement ne postoji.\n");
	}
	else if (br < P->el)
		P->L = brisi(P->L, br);
	else if (br > P->el)
		P->R = brisi(P->R, br);
	else{
		if (P->L != NULL && P->R != NULL){
			// ako je P taj kojeg moramo brisati, umjesto njega mora ici ili najmanji s desnog podstabla ili najveci s lijevog
			tmp = traziNajmanji(P->R);
			P->el = tmp->el;
			P->R = brisi(P->R, tmp->el);
		}
		else{
			//ako P nema djece s lijeve ili desne strane
			tmp = P;
			if (P->L == NULL)
				P = P->R;
			else
				P = P->L;
			free(tmp);
		}
	}

	return P;
}

Pozicija trazi(Pozicija P, int br){
	if (P == NULL)
		return P;

	//ako je trazeni broj manji od P, trazimo ga s lijeve strane
	if (P->el > br)
		return trazi(P->L, br);
	//ako je trazeni broj veci od P, trazimo ga s desne strane
	else if (P->el < br)
		return trazi(P->R, br);
	else
		return P;
}
