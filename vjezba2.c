#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CH (128)
#define SUCC (0)
#define ERR (-1)

struct _student;
typedef struct _student _stud;
typedef struct _student* Pozicija;

struct _student {
	char ime[MAX_CH];
	char prezime[MAX_CH];
	int god;
	Pozicija next;
};

int unesiPodatke(Pozicija);
int ubaciNaPocetak(Pozicija);
int ispisiListu(Pozicija);
int ubaciNaKraj(Pozicija);
Pozicija pronadiPoPrezimenu(Pozicija);
int brisiElement(Pozicija);

int main(int argc, char* argv[]) {
	_stud head;
	Pozicija trazeniStudent = NULL;
	head.next = NULL;
	int izbor = 0;
	while (izbor != 6) {
		printf("\nOdaberite:\n1 - Unos na pocetak liste\n2 - Ispis liste\n3 - Unos na kraj liste\n4 - Trazenje po prezimenu\n5 - Brisanje odredjenog elementa\n6 - izlaz\n\n");
		scanf("%d", &izbor);
		if (izbor == 1) {
			ubaciNaPocetak(&head);
		}
		else if (izbor == 2) {
			ispisiListu(head.next);
		}
		else if (izbor == 3) {
			ubaciNaKraj(&head);
		}
		else if (izbor == 4) {
			trazeniStudent = pronadiPoPrezimenu(head.next);
			if (trazeniStudent == NULL)
				printf("Student nije pronaden.\n");
			else
				printf("Trazeni student: %s %s %d\n", trazeniStudent->ime, trazeniStudent->prezime, trazeniStudent->god);
		}
		else if (izbor == 5) {
			brisiElement(&head);
			printf("Lista nakon brisanja elementa:\n");
			ispisiListu(head.next);
		}
		else if (izbor == 6) {
			printf("Izasli ste iz izbornika.\n");
		}
		else printf("Pogresan izbor.\n");
	}
	return SUCC;
}

int unesiPodatke(Pozicija P) {
	if (P == NULL) return ERR;

	printf("Unesite ime, prezime i godinu rodjenja studenta:\n");
	scanf(" %s %s %d", P->ime, P->prezime, &P->god);

	return SUCC;
}

int ubaciNaPocetak(Pozicija P) {
	if (P == NULL) return ERR;

	int unos = ERR;
	Pozicija q = NULL;
	q = (Pozicija)malloc(sizeof(_stud));
	unos = unesiPodatke(q);
	if (unos) {
		printf("Greska pri unosu podataka!\n");
	}
	q->next = P->next;
	P->next = q;

	return SUCC;
}

int ispisiListu(Pozicija P) {
	if (P == NULL) printf("Greska! Lista nema clanova!\n");
	else {
		printf("LISTA:\n");
		while (P != NULL) {
			printf("%s %s %d\n", P->ime, P->prezime, P->god);
			P = P->next;
		}
	}
	return 0;
}

int ubaciNaKraj(Pozicija P) {
	if (P == NULL) return ERR;

	int unos = ERR;
	Pozicija q=NULL;
	q = (Pozicija)malloc(sizeof(_stud));
	unos = unesiPodatke(q);
	if (unos) {
		printf("Greska pri unosu podataka!\n");
	}
	
	while (P->next != NULL)
		P = P->next;
	q->next = P->next;
	P->next = q;

	return SUCC;
}

Pozicija pronadiPoPrezimenu(Pozicija P) {
	char* pr = NULL;
	pr = (char*)malloc(MAX_CH * sizeof(char));

	printf("Unesite prezime studenta:\n");
	scanf(" %s", pr);

	while (strcmp(P->prezime, pr) != 0) {
		P = P->next;
	}

	return P;
}

int brisiElement(Pozicija P) {
	Pozicija q = NULL;
	// Brisanje elementa po prezimenu
	char* pr = NULL;
	pr = (char*)malloc(MAX_CH * sizeof(char));
	printf("Unesite prezime studenta kojeg zelite izbrisati iz liste:\n");
	scanf(" %s", pr);

	while (strcmp(P->next->prezime, pr) != 0) {
		P = P->next;
	}
	if (P->next == NULL) {
		P = NULL;
	}
	if (P == NULL) printf("Student s tim prezimenom ne postoji.\n");
	q = P->next;
	if (q != NULL)
	{
		P->next = q->next;
		printf("Iz liste se brise: %s %s %d\n", q->ime, q->prezime, q->god);
	}
	return SUCC;
}