#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ERROR (-1)
#define SUCCESS (0)
#define BUFFER (1024)

struct Cvor;
typedef struct Cvor CVOR;
typedef struct Cvor* Cvor_Poz;
struct Cvor {
	char* El;
	Cvor_Poz R;
	Cvor_Poz L;
};

struct Stog;
typedef struct Stog STOG;
typedef struct Stog* Stog_Poz;
struct Stog {
	Cvor_Poz Cvor_El;
	Stog_Poz Next;
};

int stvoriNoviCvorNaStogu(Stog_Poz*);
int stvoriNoviCvorUStablu(Cvor_Poz*);
int Pop(Stog_Poz, Cvor_Poz*);
int Push(Stog_Poz, Cvor_Poz);
int Ispis(Cvor_Poz);
int stvoriStablo(char*, Stog_Poz, Cvor_Poz*);

int main(int argc, char* argv[]) {

	int provjera = SUCCESS;
	char* imeDatoteke = NULL;
	Stog_Poz stog = NULL;
	Cvor_Poz root = NULL;

	provjera = stvoriNoviCvorNaStogu(&stog);
	if (provjera) {
		printf("\n\nGreška kod alociranja memorije (stog)!");
		return provjera;
	}

	imeDatoteke = (char*)malloc(BUFFER * sizeof(char));
	if (imeDatoteke == NULL) {
		printf("\n\nGreška kod alociranja memorije (datoteka)!");
		return ERROR;
	}
	memset(imeDatoteke, '\0', BUFFER);

	printf("Unesite ime datoteke iz koje želite proèitati postfix izraz: ");
	scanf(" %s", imeDatoteke);
	if (strrchr(imeDatoteke, '.') == NULL) {
		strcat(imeDatoteke, ".txt");
	}

	provjera = stvoriStablo(imeDatoteke, stog, &root);
	if (provjera == SUCCESS) {
		printf("\n*** INFIX IZRAZ: ***\n");
		Ispis(root);
	}

	return provjera;
}

int stvoriNoviCvorNaStogu(Stog_Poz* S) {
	Stog_Poz q = NULL;
	q = (Stog_Poz)malloc(sizeof(STOG));
	if (q == NULL) return ERROR;

	q->Cvor_El = NULL;
	q->Next = NULL;
	*S = q;

	return SUCCESS;
}

int Pop(Stog_Poz S, Cvor_Poz* elem){
	Stog_Poz tmp = NULL;
	Cvor_Poz q = NULL;

	if (S == NULL) return ERROR;

	tmp = S->Next;
	if (tmp == NULL) return ERROR;

	S->Next = tmp->Next;
	q = tmp->Cvor_El;
	free(tmp);
	*elem = q;

	return SUCCESS;
}

int Push(Stog_Poz S, Cvor_Poz C){
	int provjera = SUCCESS;
	Stog_Poz q = NULL;

	provjera = stvoriNoviCvorNaStogu(&q);
	if (provjera) return ERROR;

	q->Cvor_El = C;
	q->Next = S->Next;
	S->Next = q;

	return SUCCESS;
}

int stvoriNoviCvorUStablu(Cvor_Poz* C) {
	Cvor_Poz q = NULL;
	q = (Cvor_Poz)malloc(sizeof(CVOR));
	if (q == NULL) return ERROR;

	q->El = NULL;
	q->R = NULL;
	q->L = NULL;

	*C = q;
	return SUCCESS;
}

int stvoriStablo(char* imeDatoteke, Stog_Poz stog, Cvor_Poz* root){
	int provjera = SUCCESS;
	int number = 0;
	int brojac = 0;
	char* buff = NULL;
	Cvor_Poz q = NULL;
	FILE* fp = NULL;

	buff = (char*)malloc(sizeof(char) * BUFFER);
	if (buff == NULL){
		printf("\n\nGreška kod alokacije memorije (buffer)!");
		return ERROR;
	}
	memset(buff, '\0', BUFFER);

	fp = fopen(imeDatoteke, "r");
	if (fp == NULL)
	{
		printf("\n\nDatoteka nije otvorena!\n");
		return ERROR;
	}

	while (!feof(fp))
	{
		memset(buff, '\0', BUFFER);

		provjera = stvoriNoviCvorUStablu(&q);
		if (provjera) break;

		//vrijednosti iz datoteke jednu po jednu prebacujemo na buffer
		fscanf(fp, " %s", buff);
		if (buff == NULL || strlen(buff) == 0) continue;

		brojac = strlen(buff);
		brojac++;
		q->El = (char*)malloc(sizeof(char) * brojac);
		if (q->El == NULL){
			printf("\n\nGreška kod alokacije memorije (element)!\n");
			provjera = ERROR;
			break;
		}
		memset(q->El, '\0', brojac);
		brojac--;

		// u element kopiramo vrijednost iz buffera
		strncpy(q->El, buff, brojac);
		q->L = NULL;
		q->R = NULL;

		//ako je vrijednost na bufferu operator, skidamo zadnja 2 operanda sa stoga i postavljamo ih kao djecu R i L toga operatora
		brojac = sscanf(buff, " %d", &number);
		if (brojac == EOF || brojac <= 0){
			provjera = Pop(stog, &q->R);
			if (provjera) break;

			provjera = Pop(stog, &q->L);
			if (provjera) break;
		}
		// stavljamo vrijednost iz buffera na stog
		provjera = Push(stog, q);
		if (provjera) break;
	}

	fclose(fp);
	free(buff);

	if (provjera == SUCCESS){
		provjera = Pop(stog, &q);
		*root = q;
	}

	return provjera;
}

int Ispis(Cvor_Poz P) {

	// ispisujemo INFIX izraz (inorder)
	if (P != NULL) {
		if (P->L != NULL)
			printf("(");
		Ispis(P->L);
		printf(" %s", P->El);
		Ispis(P->R);
		if (P->R != NULL)
			printf(")");
	}

	return SUCCESS;
}