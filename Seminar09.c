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

// creare structura pentru un nod dintr-un arbore binar de cautare
typedef struct Nod {
	Masina info;
	struct Nod* st;
	struct Nod* dr;
} Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	if (!fgets(buffer, 100, file)) {
		Masina m = { 0, 0, 0.0, NULL, NULL, 0 };
		return m;
	}
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);
	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);
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

int calculeazaInaltimeArbore(Nod* arbore) {
	if (arbore != NULL) {
		int inaltimeSt = calculeazaInaltimeArbore(arbore->st);
		int inaltimeDr = calculeazaInaltimeArbore(arbore->dr);
		return (inaltimeSt > inaltimeDr ? inaltimeSt : inaltimeDr) + 1;
	}
	else {
		return 0;
	}
}

int calculeazaGradEchilibru(Nod* arbore) {
	return calculeazaInaltimeArbore(arbore->st) - calculeazaInaltimeArbore(arbore->dr);
}

void rotireStanga(Nod** arbore) {
	Nod* aux = (*arbore)->dr;
	(*arbore)->dr = aux->st;
	aux->st = (*arbore);
	(*arbore) = aux;
}

void rotireDreapta(Nod** arbore) {
	Nod* aux = (*arbore)->st;
	(*arbore)->st = aux->dr;
	aux->dr = (*arbore);
	(*arbore) = aux;
}

void adaugaMasinaInArboreEchilibrat(Nod** arbore, Masina masinaNoua) {
	if ((*arbore) != NULL) {
		if ((*arbore)->info.id < masinaNoua.id) {
			adaugaMasinaInArboreEchilibrat(&(*arbore)->dr, masinaNoua);
		}
		else {
			adaugaMasinaInArboreEchilibrat(&(*arbore)->st, masinaNoua);
		}
		int GE = calculeazaGradEchilibru(*arbore);
		if (GE == 2) {
			if (calculeazaGradEchilibru((*arbore)->st) == 1) {
				rotireDreapta(arbore);
			}
			else {
				rotireStanga(&(*arbore)->st);
				rotireDreapta(arbore);
			}
		}
		else if (GE == -2) {
			if (calculeazaGradEchilibru((*arbore)->dr) != -1) {
				rotireDreapta(&(*arbore)->dr);
			}
			rotireStanga(arbore);
		}
	}
	else {
		*arbore = (Nod*)malloc(sizeof(Nod));
		(*arbore)->info = masinaNoua;
		(*arbore)->st = NULL;
		(*arbore)->dr = NULL;
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (!f) return NULL;
	Nod* arbore = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		if (m.model != NULL) // evitam ultima linie goala
			adaugaMasinaInArboreEchilibrat(&arbore, m);
	}
	fclose(f);
	return arbore;
}

void afisareInOrdine(Nod* rad) {
	if (rad) {
		afisareInOrdine(rad->st);
		afisareMasina(rad->info);
		afisareInOrdine(rad->dr);
	}
}

void afisarePreOrdine(Nod* rad) {
	if (rad) {
		afisareMasina(rad->info);
		afisarePreOrdine(rad->st);
		afisarePreOrdine(rad->dr);
	}
}

void afisarePostOrdine(Nod* rad) {
	if (rad) {
		afisarePostOrdine(rad->st);
		afisarePostOrdine(rad->dr);
		afisareMasina(rad->info);
	}
}

void afisareMasiniDinArbore(Nod* radacina) {
	printf("--- Inordine ---\n");
	afisareInOrdine(radacina);
	printf("--- Preordine ---\n");
	afisarePreOrdine(radacina);
	printf("--- Postordine ---\n");
	afisarePostOrdine(radacina);
}

void dezalocareMasina(Masina m) {
	free(m.model);
	free(m.numeSofer);
}

void dezalocareArboreDeMasini(Nod* rad) {
	if (rad) {
		dezalocareArboreDeMasini(rad->st);
		dezalocareArboreDeMasini(rad->dr);
		dezalocareMasina(rad->info);
		free(rad);
	}
}

Masina getMasinaByID(Nod* rad, int id) {
	if (rad == NULL) {
		Masina m = { 0, 0, 0.0, NULL, NULL, 0 };
		return m;
	}
	else if (rad->info.id == id) {
		return rad->info;
	}
	else if (rad->info.id < id) {
		return getMasinaByID(rad->dr, id);
	}
	else {
		return getMasinaByID(rad->st, id);
	}
}

int determinaNumarNoduri(Nod* rad) {
	if (rad == NULL) return 0;
	return 1 + determinaNumarNoduri(rad->st) + determinaNumarNoduri(rad->dr);
}

float calculeazaPretulMasinilorUnuiSofer(Nod* rad, const char* numeSofer) {
	if (!rad) return 0;
	float s = 0;
	if (strcmp(rad->info.numeSofer, numeSofer) == 0) {
		s += rad->info.pret;
	}
	s += calculeazaPretulMasinilorUnuiSofer(rad->st, numeSofer);
	s += calculeazaPretulMasinilorUnuiSofer(rad->dr, numeSofer);
	return s;
}
float calculeazaPretTotal(Nod* arbore) {
	if (arbore == NULL) {
		return 0;
	}
	return arbore->info.pret + calculeazaPretTotal(arbore->st) + calculeazaPretTotal(arbore->dr);
}


int main() {
	Nod* arbore = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	afisareMasiniDinArbore(arbore);
	printf("Numar noduri: %d\n", determinaNumarNoduri(arbore));
	printf("Pret total: %.2f\n", calculeazaPretTotal(arbore));
	printf("Pret masini pentru soferul 'Ion': %.2f\n", calculeazaPretulMasinilorUnuiSofer(arbore, "Ion"));
	dezalocareArboreDeMasini(arbore);
	return 0;
}
