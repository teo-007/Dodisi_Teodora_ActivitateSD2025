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

struct Heap {
	Masina* masini;
	int lungime; // capacitate
	int nrElem;  // nr de elemente vizibile
};
typedef struct Heap Heap;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	if (fgets(buffer, 100, file) == NULL) {
		Masina m = { 0 };
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

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.masini = (Masina*)malloc(lungime * sizeof(Masina));
	heap.nrElem = 0;
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int fiuSt = pozitieNod * 2 + 1;
	int fiuDr = pozitieNod * 2 + 2;
	int pozMax = pozitieNod;

	if (fiuSt < heap.nrElem && heap.masini[pozMax].id < heap.masini[fiuSt].id) {
		pozMax = fiuSt;
	}
	if (fiuDr < heap.nrElem && heap.masini[pozMax].id < heap.masini[fiuDr].id) {
		pozMax = fiuDr;
	}
	if (pozMax != pozitieNod) {
		Masina aux = heap.masini[pozMax];
		heap.masini[pozMax] = heap.masini[pozitieNod];
		heap.masini[pozitieNod] = aux;

		if (pozMax <= (heap.nrElem - 2) / 2) {
			filtreazaHeap(heap, pozMax);
		}
	}
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (f == NULL) {
		printf("Eroare la deschiderea fisierului %s\n", numeFisier);
		exit(1);
	}
	Heap heap = initializareHeap(10);

	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		if (m.model != NULL) { // doar dacă am citit ceva valid
			heap.masini[heap.nrElem++] = m;
		}
	}

	for (int i = (heap.nrElem - 2) / 2; i >= 0; i--) {
		filtreazaHeap(heap, i);
	}

	fclose(f);
	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrElem; i++) {
		afisareMasina(heap.masini[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	if (heap.nrElem == heap.lungime) {
		printf("Nu exista elemente ascunse in heap.\n");
	}
	else {
		printf("Elemente ascunse:\n");
		for (int i = heap.nrElem; i < heap.lungime; i++) {
			afisareMasina(heap.masini[i]);
		}
	}
}

Masina extrageMasina(Heap* heap) {
	if (heap->nrElem == 0) {
		Masina gol = { 0 };
		return gol;
	}
	Masina masinaExtrasa = heap->masini[0];
	heap->masini[0] = heap->masini[heap->nrElem - 1];
	heap->nrElem--;

	for (int i = (heap->nrElem - 2) / 2; i >= 0; i--) {
		filtreazaHeap(*heap, i);
	}

	return masinaExtrasa;
}

void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++) {
		if (heap->masini[i].model != NULL)
			free(heap->masini[i].model);
		if (heap->masini[i].numeSofer != NULL)
			free(heap->masini[i].numeSofer);
	}
	free(heap->masini);
	heap->masini = NULL;
	heap->nrElem = 0;
	heap->lungime = 0;
}

int main() {
	Heap h = citireHeapDeMasiniDinFisier("masini_arbore.txt");

	printf("\nAfisare heap initial:\n");
	afisareHeap(h);

	printf("\n--- Extragem cea mai importanta masina ---\n");
	Masina m = extrageMasina(&h);
	printf("\nMasina extrasa:\n");
	afisareMasina(m);

	printf("\nAfisare heap dupa extractie:\n");
	afisareHeap(h);

	printf("\nElemente ascunse:\n");
	afiseazaHeapAscuns(h);

	dezalocareHeap(&h);

	return 0;
}
