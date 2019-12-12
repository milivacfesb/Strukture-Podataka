#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX_CH (128)
#define SUCC (0)
#define ERR (-1)

struct _student;
typedef struct _student _stud;
typedef struct _student* Pozicija;

struct _student
{
	char ime[MAX_CH];
	char prezime[MAX_CH];
	int god;
	Pozicija next;
};

int unesiPodatke(Pozicija);
int oslobodiProstorZaElement(Pozicija *);
int ubaciNaPocetak(Pozicija);
int ispisiListuElemenata(Pozicija);
int ubaciNaKraj(Pozicija);
Pozicija pretraziPoPrezimenu(Pozicija);
int brisiElementPoPrezimenu(Pozicija);
int ubaciIzaElementaPoPrezimenu(Pozicija);
int ubaciIspredElementaPoPrezimenu(Pozicija);
int spremiPodatkeUDatoteku(Pozicija);
int procitajPodatkeIzDatoteke(Pozicija);
int sortirajPoPrezimenu(Pozicija);


int main(int argc, char*argv[])
{
	_stud head;
	head.next = NULL;
	Pozicija trazeniStudent = NULL;

	int izbor = 0;

	while (izbor !=11)
	{
		printf("\nOdaberite:\n1 - Unos na pocetak liste\n2 - Ispis liste\n3 - Unos na kraj liste\n4 - Trazenje po prezimenu\n5 - Brisanje odredjenog elementa\n6 - Unesi iza odredjenog studenta\n7 - Unesi ispred odredjenog studenta\n8 - Slozi po abecednom redu prezimena\n9 - Spremi u datoteku\n10 - Ispisi iz datoteke\n11 - Izlaz\n\n");
		scanf("%d", &izbor);
		if (izbor == 1)
			ubaciNaPocetak(&head);
		else if (izbor == 2)
			ispisiListuElemenata(head.next);
		else if (izbor == 3)
			ubaciNaKraj(&head);
		else if (izbor == 4)
		{
			trazeniStudent = pretraziPoPrezimenu(&head);
			if (trazeniStudent == NULL)
				printf("Student nije pronaden.\n");
			else
				printf("Trazeni student: %s %s %d\n", trazeniStudent->ime, trazeniStudent->prezime, trazeniStudent->god);
		}
		else if (izbor == 5)
			brisiElementPoPrezimenu(&head);
		else if (izbor == 6)
			ubaciIzaElementaPoPrezimenu(&head);
		else if (izbor == 7)
			ubaciIspredElementaPoPrezimenu(&head);
		else if (izbor == 8)
			sortirajPoPrezimenu(&head);
		else if (izbor == 9)
			spremiPodatkeUDatoteku(head.next);
		else if (izbor == 10)
			procitajPodatkeIzDatoteke(&head);

	}
	return SUCC;
}

int oslobodiProstorZaElement(Pozicija *p)
{
	Pozicija q = NULL;
	*p = (Pozicija)malloc(sizeof(_stud));
	if (*p == NULL)
		return ERR;

	q = *p;
	q->next = NULL;
	return SUCC;
}

int unesiPodatke(Pozicija p)
{
	if (p == NULL) return ERR;

	printf("Unesite ime, prezime i godinu rodjenja studenta:\n");
	scanf(" %s %s %d", p->ime, p->prezime, &p->god);

	return SUCC;

}

int ubaciNaPocetak(Pozicija p)
{
	if (p == NULL)
		return ERR;

	int unos = ERR;
	Pozicija q = NULL;
	oslobodiProstorZaElement(&q);

	unos = unesiPodatke(q);
	if (unos)
		printf("Greska pri unosu podataka!\n");

	q->next = p->next;
	p->next = q;

	return SUCC;
}

int ubaciNaKraj(Pozicija p)
{
	Pozicija q = NULL;
	oslobodiProstorZaElement(&q);
	unesiPodatke(q);
	
	while (p->next != NULL)
		p = p->next;

	q->next = p->next;
	p->next = q;

	return SUCC;
}

int ispisiListuElemenata(Pozicija p)
{
	if (p == NULL)
		printf("Greska! Lista nema clanova!\n");
	else
	{
		printf("LISTA:\n");
		while (p != NULL)
		{
			printf(" %s %s %d\n", p->ime, p->prezime, p->god);
			p = p->next;
		}
	}
	return SUCC;
}

Pozicija pretraziPoPrezimenu(Pozicija p)
{
	char prezime[MAX_CH] = "";
	printf("Unesite prezime studenta:\n");
	scanf(" %s", &prezime);

	while (strcmp(p->prezime, prezime) != 0)
		p = p->next;

	return p;
}

int brisiElementPoPrezimenu(Pozicija p)
{
	Pozicija q = NULL;
	q = pretraziPoPrezimenu(p);

	if (q == NULL)
		printf("Taj student nije pronaden.\n");
	
	else
	{
		while (p->next != q)
			p = p->next;
		p->next = q->next;
		printf("Iz liste se brise student: %s %s %d\n", q->ime, q->prezime, q->god);

	}

	return SUCC;
}

int ubaciIzaElementaPoPrezimenu(Pozicija p)
{
	Pozicija q = NULL;
	Pozicija r = NULL;
	
	q = pretraziPoPrezimenu(p);
	oslobodiProstorZaElement(&r);
	unesiPodatke(r);
	r->next = q->next;
	q->next = r;

	return SUCC;
}

int ubaciIspredElementaPoPrezimenu(Pozicija p)
{
	Pozicija q = NULL;
	Pozicija r = NULL;

	q = pretraziPoPrezimenu(p);
	oslobodiProstorZaElement(&r);
	unesiPodatke(r);

	while (p->next != q)
		p = p->next;
	r->next = q;
	p->next = r;
	return SUCC;
}

int spremiPodatkeUDatoteku(Pozicija p)
{
	char nazivDatoteke[MAX_CH] = "";
	printf("Unesite naziv datoteke u koji zelite spremiti vasu listu: ");
	scanf(" %s", &nazivDatoteke);
	FILE *fp;
	fp = fopen(nazivDatoteke, "w");
	while (p != NULL)
	{
		fprintf(fp, "%s %s %d ", p->ime, p->prezime, p->god);
		p = p->next;
	}
	

	fclose(fp);
	return SUCC;
}

int procitajPodatkeIzDatoteke(Pozicija p)
{
	char ime[MAX_CH] = "";
	char prezime[MAX_CH] = "";
	int god = 0;
	char nazivDatoteke[MAX_CH] = "";
	printf("Unesite naziv datoteke iz koje zelite procitati studente: ");
	scanf(" %s", &nazivDatoteke);
	FILE *fp;
	fp = fopen(nazivDatoteke, "r");
	printf("U toj datoteci nalaze se studenti: \n");
	do
	{
		fscanf(fp, "%s %s %d ", &ime, &prezime, &god);
		printf("%s %s %d\n", ime, prezime, god);
	} while (!feof(fp));

	printf("\n\n");
	fclose(fp);
	return SUCC;
}

int sortirajPoPrezimenu(Pozicija p)
{
	printf("Sortiram sve studente po abecednom redu!");
	Pozicija q, prevq, temp, end;
	end = NULL;
	while (p->next != end)
	{
		prevq = p;
		q = p->next;
		while (q->next != end)
		{
			if (q->prezime[0] >= q->next->prezime[0])
			{
				temp = q->next;
				prevq->next = temp;
				q->next = temp->next;
				temp->next = q;

				q = temp;
			}
			prevq = q;
			q = q->next;
		}
		end = q;
	}

	printf("Nova lista izgleda ovako:\n");
	ispisiListuElemenata(p->next);
	return SUCC;
}
