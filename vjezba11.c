#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define ERROR (-1)
#define SUCCESS (0)
//velicina hash tablice je 11
#define VEL (11)
#define BUFFER (1024)

struct student;
typedef struct student* Pozicija;
typedef struct student _stud;
struct student {
	int mat_br;
	char* ime;
	char* prezime;
	Pozicija Next;
};

struct hash;
typedef struct hash* hPozicija;
typedef struct hash _hash;
struct hash {
	int hashVrijednost;
	Pozicija stablo;
};

int racunajHash(Pozicija, int*);
Pozicija unesiListu(Pozicija, Pozicija);
int unesiHash(Pozicija, hPozicija);
int stvoriCvor(Pozicija*);
int citajIzDatoteke(char*, hPozicija);
int ispisListe(Pozicija);
int ispisHashTablice(hPozicija);

int main(int argc, char* argv[]) {
	int provjera = SUCCESS;
	int i = 0;
	char* imeD = NULL;
	hPozicija tablica = NULL;

	imeD = (char*)malloc(sizeof(char) * BUFFER);
	if (imeD == NULL) {
		printf("\nGreska kod alokacije memorije (ime datoteke).\n");
		return ERROR;
	}
	memset(imeD, '\0', BUFFER);

	printf("Unesite naziv datoteke: ");
	scanf(" %s", imeD);

	//alociranje memorije za hash tablicu
	tablica = (hPozicija)malloc(sizeof(_hash) * VEL);
	if (tablica == NULL){
		printf("\nGreska pri alokaciji memorije (hash tablica).\n");
		return ERROR;
	}

	//postavljanje svih 11 "stabala" u hash tablici na NULL
	while (i < VEL){
		tablica[i].stablo = NULL;
		i++;
	}

	//citanje studenata iz datoteke i ispis stvorene hash tablice
	provjera = citajIzDatoteke(imeD, tablica);
	if (provjera==SUCCESS){
		provjera = ispisHashTablice(tablica); 
		if (provjera) {
			printf("Greška kod ispisa hash tablice.\n");
			return ERROR;
		}
	}

	free(imeD);
	return SUCCESS;
}

int racunajHash(Pozicija q, int* index){
	int provjera = SUCCESS;
	int buff_L = 0;
	int i = 0;
	int zbroj = 0;

	if (q == NULL || q->prezime == NULL){
		return ERROR;
	}
	buff_L = strlen(q->prezime);

	//racunamo zbroj ASCII vrijednosti prvih 5 slova prezimena
	while (i < buff_L && i < 5)
	{
		zbroj += (int)q->prezime[i];
		i++;
	}

	//ostatak cjelobrojnog dijeljenja zbroja s velicinom hash tablice daje hash vrijednost
	*index = zbroj % VEL;

	return provjera;
}

Pozicija unesiListu(Pozicija P, Pozicija q)
{
	Pozicija tmp = P;
	if (tmp == NULL) return q;

	if (tmp->Next == NULL && strcmp(tmp->prezime, q->prezime) < 0){
		q->Next = tmp;
		return q;
	}

	while (tmp != NULL && strcmp(tmp->prezime, q->prezime) < 0){
		tmp = tmp->Next;
	}

	if (tmp != NULL && strcmp(tmp->prezime, q->prezime) == 0){
		while (tmp->Next != NULL && strcmp(tmp->prezime, q->prezime) == 0 && strcmp(tmp->ime, q->ime) < 0){
			tmp = tmp->Next;
		}

		if (tmp->Next != NULL && strcmp(tmp->prezime, q->prezime) == 0 && strcmp(tmp->ime, q->ime) == 0){
			while (tmp->Next != NULL && strcmp(tmp->prezime, q->prezime) == 0 && strcmp(tmp->ime, q->ime) == 0 && tmp->mat_br < q->mat_br){
				tmp = tmp->Next;
			}
		}
	}

	q->Next = tmp->Next;
	tmp->Next = q;

	return tmp;
}

int unesiHash(Pozicija q, hPozicija tablica){
	int provjera = SUCCESS;
	int i = 0;
	if (q == NULL || tablica == NULL) return ERROR;

	provjera = racunajHash(q, &i);
	if (provjera) {
		printf("\nGreska kod racunanja hash vrijednosti.\n");
		return provjera;
	}

	if (tablica[i].hashVrijednost != i){
		tablica[i].stablo = NULL;
	}

	//postavljanje hash vrijednosti tablice na odgovarajuæi index
	tablica[i].hashVrijednost = i;
	//spajanje studenata s odgovarajuæom hash vrijednošæu (smještanje studenata u odg.mjesta u hash tablici)
	tablica[i].stablo = unesiListu(tablica[i].stablo, q);

	if (tablica[i].stablo == NULL) {
		printf("\nGreska pri dodjeli studenata odgovarajucim hash vrijednostima.\n");
		return ERROR;
	}

	return provjera;
}

int stvoriCvor(Pozicija* q){
	Pozicija tmp = NULL;

	tmp = (Pozicija)malloc(sizeof(_stud));
	if (tmp == NULL) {
		printf("\nGreska pri alociranju memorije (stvaranje cvora).");
		return ERROR;
	}

	tmp->Next = NULL;
	*q = tmp;

	return SUCCESS;
}

int citajIzDatoteke(char* imeD, hPozicija tablica){
	int provjera = SUCCESS;
	char* buff = NULL;
	int brojac = 0;
	int buff_L = 0;
	int u = 0;
	FILE* fp = NULL;
	Pozicija q = NULL;

	buff = (char*)malloc(sizeof(char) * BUFFER);
	if (buff == NULL) {
		printf("\nGreska pri alokaciji memorije (buffer).\n");
		return ERROR;
	}

	fp = fopen(imeD, "r");
	if (fp == NULL) {
		printf("\nGreska pri otvaranju datoteke!\n");
		return ERROR;
	}

	while (!feof(fp))
	{
		memset(buff, 0, BUFFER);

		//redom citamo iz datoteke mb, ime, prezime jednog po jednog studenta i spremamo u buffer
		fscanf(fp, " %s", buff);

		if (buff == NULL || strlen(buff) == 0) continue;
		buff_L = strlen(buff);

		//prvo ce se procitati mb, zatim ime, pa prezime, i tako se ponavlja za svakog studenta
		//zato nam treba brojac koji ce izmjenjivati radnje potrebne za mb, ime i prezime
		if (brojac % 3 == 0){
			if (q != NULL){
				provjera = unesiHash(q, tablica);
				if (provjera) break;
				u = 1;
			}

			provjera = stvoriCvor(&q);
			if (provjera) break;

			//citamo mb broj iz buffera i spremamo ga u listu
			provjera = sscanf(buff, " %s", &q->mat_br);
			//provjeravamo jesmo li procitali nesto sto nije char, u tom slucaju program javlja gresku
			if (provjera == EOF || provjera != 1){
				provjera = ERROR;
				break;
			}
			provjera = SUCCESS;
			u = 0;
		}
		else if (brojac % 3 == 1){
			//alocira se memorija za ime studenta, cita se ime iz buffera i obavlja se ista provjera
			q->ime = (char*)malloc(sizeof(char) * (buff_L + 1));
			if (q->ime == NULL) return ERROR;
			memset(q->ime, 0, buff_L + 1);

			provjera = sscanf(buff, " %s", q->ime);
			if (provjera == EOF || provjera != 1){
				provjera = ERROR;
				break;
			}
			provjera = SUCCESS;
			u = 0;
		}
		else if (brojac % 3 == 2){
			//alocira se memorija za prezime, cita se prezime iz buffera i obavlja se ista provjera
			q->prezime = (char*)malloc(sizeof(char) * (buff_L + 1));
			if (q->prezime == NULL) return ERROR;
			memset(q->prezime, 0, buff_L + 1);
			
			provjera = sscanf(buff, " %s", q->prezime);
			if (provjera == EOF || provjera != 1){
				provjera = ERROR;
				break;
			}
			provjera = SUCCESS;
			u = 0;
		}
		brojac++;
	}

	fclose(fp);
	//zadnji student
	if ((provjera==0) && (u==0) && (q != NULL)){
		provjera = unesiHash(q, tablica);
	}
	return provjera;
}

int ispisListe(Pozicija P){
	int i = 1;
	while (P != NULL){
		printf("\n\t\t%d.)\t%s %s", i++, P->ime, P->prezime);
		P = P->Next;
	}

	return SUCCESS;
}

int ispisHashTablice(hPozicija tablica){
	int i = 0;
	printf("\n***HASH TABLICA***\n");

	while (i < VEL){
		printf("\n\n\tStudenti sa hash vrijednosti (%d) : ", i+1);
		ispisListe(tablica->stablo);
		tablica++;
		i++;
	}

	return SUCCESS;
}