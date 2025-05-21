#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);
	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);
	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

struct Stiva {
	Masina info;
	struct Stiva* next;
};
typedef struct Stiva Stiva;

void pushStack(Stiva** nod, Masina masina) {
	Stiva* aux = (Stiva*)malloc(sizeof(Stiva));
	aux->info = masina;
	aux->next = (*nod);
	(*nod) = aux;
}

Masina popStack(Stiva** nod) {
	Masina masina;
	if ((*nod) != NULL) {
		Stiva* aux = (*nod);
		masina = aux->info;
		(*nod) = aux->next;
		free(aux);
	}
	else {
		masina.id = -1;
	}
	return masina;
}

char isEmptyStack(Stiva* nod) {
	return nod == NULL;
}

Stiva* citireStackMasiniDinFisier(const char* numeFisier) {
	Stiva* nod = NULL;
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		pushStack(&nod, citireMasinaDinFisier(f));
	}
	fclose(f);
	return nod;
}

void dezalocareStivaDeMasini(Stiva** cap) {
	while (!isEmptyStack(*cap)) {
		Masina m = popStack(cap);
		free(m.model);
		free(m.numeSofer);
	}
}

int size(Stiva* cap) {
	int marime = 0;
	while (cap != NULL) {
		marime++;
		cap = cap->next;
	}
	return marime;
}

typedef struct nod Nod;
struct nod {
	Masina info;
	Nod* next;
	Nod* prev;
};

typedef struct coada Coada;
struct coada {
	Nod* first;
	Nod* last;
};

void enqueue(Coada* coada, Masina masina) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masina;
	nou->next = coada->first;
	nou->prev = NULL;
	if (coada->first == NULL) {
		coada->first = nou;
		coada->last = nou;
		return;
	}
	coada->first->prev = nou;
	coada->first = nou;
}

Masina dequeue(Coada* coada) {
	if (coada->last == NULL) {
		Masina m;
		m.id = -1;
		return m;
	}
	else {
		Masina m = coada->last->info;
		if (coada->last->prev == NULL) {
			coada->first = NULL;
			coada->last = NULL;
		}
		else {
			Nod* ultim = coada->last;
			coada->last = ultim->prev;
			ultim->prev->next = NULL;
			free(ultim);
		}
		return m;
	}
}

void* citireCoadaDeMasiniDinFisier(const char* numeFisier) {
	Coada* coada = (Coada*)malloc(sizeof(Coada));
	coada->first = NULL;
	coada->last = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f == NULL) {
		printf("Eroare la deschiderea fisierului!\n");
		return NULL;
	}
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		enqueue(coada, m);
	}
	fclose(f);
	return coada;
}

void dezalocareCoadaDeMasini(Coada* coada) {
	while (coada->last != NULL) {
		Masina m = dequeue(coada);
		free(m.model);
		free(m.numeSofer);
	}
	free(coada);
}

Masina getMasinaByID(Stiva* stiva, int id) {
	while (stiva != NULL) {
		if (stiva->info.id == id) {
			return stiva->info;
		}
		stiva = stiva->next;
	}
	Masina m;
	m.id = -1;
	return m;
}

Masina getMasinaByID_Coada(Coada* coada, int id) {
	Nod* curent = coada->first;
	while (curent != NULL) {
		if (curent->info.id == id) {
			return curent->info;
		}
		curent = curent->next;
	}
	Masina m;
	m.id = -1;
	return m;
}

float calculeazaPretTotal(Stiva* stiva) {
	float suma = 0;
	while (stiva != NULL) {
		suma += stiva->info.pret;
		stiva = stiva->next;
	}
	return suma;
}

float calculeazaPretTotal_Coada(Coada* coada) {
	float suma = 0;
	Nod* curent = coada->first;
	while (curent != NULL) {
		suma += curent->info.pret;
		curent = curent->next;
	}
	return suma;
}

int main() {
	Stiva* cap = citireStackMasiniDinFisier("masini_arbore.txt");

	printf("Masina extrasa din stiva:\n");
	afisareMasina(popStack(&cap));

	printf("Numar elemente ramase in stiva: %d\n", size(cap));
	printf("Pret total masini din stiva: %.2f\n", calculeazaPretTotal(cap));

	int cautID = 2;
	Masina m = getMasinaByID(cap, cautID);
	if (m.id != -1) afisareMasina(m);
	else printf("Masina cu ID %d nu a fost gasita in stiva!\n", cautID);

	dezalocareStivaDeMasini(&cap);

	Coada* coada = (Coada*)citireCoadaDeMasiniDinFisier("masini.txt");
	printf("\nPret total masini din coada: %.2f\n", calculeazaPretTotal_Coada(coada));

	Masina m2 = getMasinaByID_Coada(coada, 3);
	if (m2.id != -1) afisareMasina(m2);
	else printf("Masina cu ID %d nu a fost gasita in coada!\n", 3);

	dezalocareCoadaDeMasini(coada);

	return 0;
}
