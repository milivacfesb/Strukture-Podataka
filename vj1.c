#include <stdio.h>

typedef struct {
	char ime[10];
	char prezime[10];
	int bodovi;
}studenti;

int main() {
	FILE *f = NULL;
	int i = 0;
	int brojac = 0;
	studenti *s = NULL;

	f = fopen("studenti.txt", "r");
	if (f == NULL) {
		printf("Greška pri otvaranju datoteke!\n");
		getchar();
		exit(1);
	}

	while (!feof(f)) {
		if (fgetc(f) == '\n') ++brojac;
	}
	rewind(f);
	s = (studenti*)malloc(brojac * sizeof(studenti));

	for (i = 0; i < brojac; i++)
	{
		fscanf(f, "%s %s %d", s[i].ime, s[i].prezime, &s[i].bodovi);
	}

	for (i = 0; i < brojac; i++) {
		printf("%s \t %s \t %d \t %lf%%\n", s[i].ime, s[i].prezime, s[i].bodovi, ((float)s[i].bodovi / 30 * 100));
	}

	fclose(f);

	getchar();

	return 0;
}