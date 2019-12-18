#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>


#define SUCCESS (0)
#define ERROR (-1)
#define BUFFER (1024)


typedef struct s*Stablo;
typedef struct s _stablo;
struct s{
	char *name;
	int isRoot;
	Stablo Child;
	Stablo Next;
};

typedef struct stog _st;
typedef struct stog *Pos;
struct stog
{
	Pos Node;
	Pos Next;
};

int stvoriProstorZaDirektorij(Stablo *); //funkcija koja radi malloc za stablo
int napraviDirektorij(Stablo, char[BUFFER]);//stvara direktorij
Stablo ubaciDirektorij(Stablo,Stablo);//smjesta stvoreni direktorij kao dijete odredenom direktoriju
int ispisiDirektorije(Stablo,char*);//ispisuje sve direktorije u trenutnom stablu
Stablo udiUDirektorij(Stablo,char*,char*,Pos); 
Stablo izadiIzDirektorija(Stablo,char*, Pos);
int ispisiPath(char*);
int StvoriNovoStabloZaStog(Pos *);//malloc za stog
int Push(Pos, Stablo);
int Pop(Pos, Stablo*);

int main(int argc,char *argv[])
{
	//inicijaliziramo sve varijable i stvaramo prostor za one koje je potrebno
	Stablo root;
	stvoriProstorZaDirektorij(&root);
	root->name = (char*)malloc(BUFFER*sizeof(char));
	memset(root->name, '\0', sizeof(root->name)); //funkcija koja sva "prazna" mjesta puni sa \0
	root->name = "C";
	root->Child = NULL;
	root->Next = NULL;
	root->isRoot = 1;

	char *path=NULL;
	path = (char*)malloc(sizeof(char)* BUFFER);
	memset(path, '\0', BUFFER);


	Pos stog =NULL;
	StvoriNovoStabloZaStog(&stog);
	stog->Next = NULL;

	int nParams = 0;
	Stablo trenutni = NULL;
	trenutni = root;
	char instrukcija[BUFFER], komanda[BUFFER], direktorij[BUFFER];

	//postavljanje imena pathu(za pocetak)
	strcat(path, root->name);
	strcat(path, ":\\");

	//infinity while petlja koja ce se vrtiti sve dok na izlazu ne damo exit komandu
	while(_stricmp(instrukcija, "exit")!=0)
	{
		ispisiPath(path);
		memset(komanda, 0, BUFFER);//praznimo ove tri varijable koje koristimo pri unosu
		memset(direktorij, 0, BUFFER);
		memset(instrukcija, 0, BUFFER);
	
		fgets(komanda,BUFFER,stdin);
		nParams = sscanf(komanda," %s %s",instrukcija,direktorij);

		if(nParams == EOF || nParams <=0)continue;

		if(strcmp(instrukcija,"md")==0)
			napraviDirektorij(trenutni,direktorij);

		else if(strcmp(instrukcija,"dir")==0)
			ispisiDirektorije(trenutni,path);

		else if(strcmp(instrukcija,"cd")==0)
			trenutni=udiUDirektorij(trenutni,direktorij,path,stog);

		else if(strcmp(instrukcija,"cd..")==0)
			trenutni = izadiIzDirektorija(trenutni,path, stog);

		else if(strcmp(instrukcija,"exit")==0)
			continue;
		else
			printf("Neispravan unos");
	}

}

int stvoriProstorZaDirektorij(Stablo *q)
{
	Stablo tmp = NULL;

	tmp = (Stablo)malloc(sizeof(_stablo));
	if (tmp == NULL) return ERROR;

	tmp->isRoot = 0;
	tmp->Child = NULL;
	tmp->Next = NULL;

	*q = tmp;
	return SUCCESS;
}

int napraviDirektorij(Stablo s, char ime[BUFFER])
{


	Stablo k = NULL;
	stvoriProstorZaDirektorij(&k);

	k->Child = NULL;
	k->isRoot = 0;
	k->name = (char*)malloc(strlen(ime) +1);

	memset(k->name,'\0',(strlen(ime) + 1));
	strcpy(k->name,ime,strlen(ime));

	s->Child = ubaciDirektorij(k,s->Child);

	return SUCCESS;

}

Stablo ubaciDirektorij(Stablo q, Stablo child)
{

	Stablo prev = NULL;

	if (child == NULL)
		return q;
	else
	{
		prev = child;

		while (prev->Next != NULL && strcmp(q->name, prev->Next->name) > 0)
		{
			prev = prev->Next;
		}
		if (prev->Next != NULL && strcmp(q->name, prev->Next->name) == 0)
		{
			printf("Direktorij tog imena vec postoji");
			return child;
		}

		q->Next = prev->Next;
		prev->Next = q;

		return child;
	}

}

Stablo udiUDirektorij(Stablo node, char *name,char *path,Pos s)
{
	Stablo current = NULL;
	char *a = NULL;
	
	if (node == NULL)
	{
		printf(NULL);
		return NULL;
	}

	current = node->Child;

	while (strcmp(current->name,name)!=0)
		current = current->Next;

	if (current ==NULL)
	{
		printf("U sistemu ne postoji takva putanja.");
		return node;
	}



	Push(s, node);
	if(!node->isRoot)
		strcat(path, "\\");
	strcat(path, current->name);

	return current;


}
Stablo izadiIzDirektorija(Stablo node,char *path,Pos s)
{

	int result = SUCCESS;
	Stablo temp = NULL;
	char *a = NULL;

	result = Pop(s, &temp);
	if (result)
	{
		return node;
	}

	a = strrchr(path, '\\');
	if (temp->isRoot)
	{
		a++;
	}
	*a = '\0';

	return temp;

}

int ispisiDirektorije(Stablo q, char *path)
{
	int i = 0;
	printf("\r\nSvi direktoriji od %s", path);

	q = q->Child;
	while (q != NULL)
	{
		printf("\r\n\t<DIR> %s", q->name);
		q = q->Next;
		i++;
	}
	printf("\n\n");

	return SUCCESS;
}

int ispisiPath(char *path)
{
	printf("%s>", path);
	return SUCCESS;
}

int StvoriNovoStabloZaStog(Pos *q) 
{
	Pos tmp = NULL;

	tmp = (Pos)malloc(sizeof(_st));
	if (tmp == NULL) return ERROR;

	tmp->Next = NULL;
	*q = tmp;

	return SUCCESS;
}

int Push(Pos S, Stablo T)//klasicni push i pop na kraju
{
	int result = SUCCESS;
	Pos q = NULL;

	result = StvoriNovoStabloZaStog(&q);
	if (result)
	{
		printf("Alokacija memorije neuspjesna!");
		return result;
	}
	q->Node = T;
	q->Next = S->Next;
	S->Next = q;

	return SUCCESS;
}

int Pop(Pos S, Stablo *node)
{
	Pos tmp = NULL;

	if (NULL == S->Next)
		return ERROR;

	tmp = S->Next;
	if (tmp == NULL)
		return ERROR;

	S->Next = tmp->Next;
	if (tmp->Node == NULL)
		return ERROR;

	*node = tmp->Node;
	free(tmp);

	return SUCCESS;
}