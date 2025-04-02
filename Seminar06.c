#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct StructuraMasina 
{
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};

typedef struct StructuraMasina Masina;
typedef struct nod nod;	

struct nod
{
	Masina info;
	nod* next;
};

struct HashTable 
{
	nod** vector;
	int dim;
};
typedef struct HashTable HashTable;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	Masina m1;
	m1.id = -1; 

	if (!fgets(buffer, sizeof(buffer), file)) {
		return m1;  
	}

	char* aux;
	char sep[3] = ",\n";

	aux = strtok(buffer, sep);
	if (!aux) return m1;
	m1.id = atoi(aux);

	aux = strtok(NULL, sep);
	if (!aux) return m1;
	m1.nrUsi = atoi(aux);

	aux = strtok(NULL, sep);
	if (!aux) return m1;
	m1.pret = atof(aux);

	aux = strtok(NULL, sep);
	if (!aux) return m1;
	m1.model = malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	if (!aux) return m1;
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

	aux = strtok(NULL, sep);
	if (!aux) return m1;
	m1.serie = *aux;

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

void afisareListaMasini(nod* cap)
{
	nod* temp = cap;
	while (temp != NULL)
	{
		afisareMasina(temp->info);
		temp = temp->next;
	}
}

void adaugaMasinaInLista(nod** cap, Masina masinaNoua) 
{
	nod* nou = malloc(sizeof(nod));
	nou->info = masinaNoua; //copiere shallow
	nou->next = NULL;
	if ((*cap) != NULL)
	{
		nod* aux = (*cap);
		while (aux->next != NULL)
		{
			aux = aux->next;
		}
		aux->next = nou;
	}
	else (*cap) = nou;
}


HashTable initializareHashTable(int dimensiune) 
{
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = (nod**)malloc(dimensiune * sizeof(nod*));

	for (int i = 0; i < dimensiune; i++) 
	{
		ht.vector[i] = NULL;
	}
	return ht;
}

int calculeazaHash(const char* cheieNumeSofer, int dimensiune) 
{
	if (!dimensiune && dimensiune < 0)
	{
		return -1;
	}
	unsigned int suma = 0;
	for (int i = 0; i < strlen(cheieNumeSofer); i++)
	{
		suma += (int)cheieNumeSofer[i];
	}
	suma = suma % dimensiune; //rezultat hashcode

	return suma;
}

void inserareMasinaInTabela(HashTable hash, Masina masina) 
{
	int poz = calculeazaHash(masina.numeSofer, hash.dim);
	if (hash.vector[poz] == NULL)
	{
		adaugaMasinaInLista(&(hash.vector[poz]), masina);
	}
	else
	{
		//avem coliziune
		adaugaMasinaInLista(&(hash.vector[poz]), masina);
	}
}

HashTable citireMasiniDinFisier(const char* numeFisier, int dim)
{
	FILE* f = fopen(numeFisier, "r");
	if (!f)
	{
		printf("Eroare: Nu s-a putut deschide fisierul %s\n", numeFisier);
		exit(1);
	}

	HashTable hash = initializareHashTable(dim);
	while (!feof(f))
	{
		Masina m = citireMasinaDinFisier(f);
		if (m.id != -1)
		{
			inserareMasinaInTabela(hash, m);
		}
	}
	fclose(f);
	return hash;
}


void afisareTabelaDeMasini(HashTable ht) 
{
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			printf("Suntem la lantul %d: \n", i+1);
			afisareListaMasini(ht.vector[i]);
		}
	}
}

void dezalocareTabelaDeMasini(HashTable *ht) 
{
	//sunt dezalocate toate masinile din tabela de dispersie
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	//calculeaza pretul mediu al masinilor din fiecare cluster.
	//trebuie sa returnam un vector cu valorile medii per cluster.
	//lungimea vectorului este data de numarul de clustere care contin masini
	return NULL;
}

Masina getMasinaDupaCheie(HashTable ht, const char* numeSofer) 
{
	Masina m;
	m.id = -1;
	int poz = calculeazaHash(numeSofer, ht.dim);
	if (poz < 0 || poz >= ht.dim || ht.vector[poz] == NULL) return m;
	nod* aux = ht.vector[poz];

	while (aux!=NULL && strcmp(aux->info.numeSofer, numeSofer)!=0) 
	{
		aux = aux->next;
	}
	if (aux != NULL)
	{
		m.numeSofer = (char*)malloc(strlen(numeSofer) + 1);
		strcpy(m.numeSofer, numeSofer);
		m.model = (char*)malloc(strlen(aux->info.model) + 1);
		strcpy(m.model, aux->info.model);
		m.id = aux->info.id;
		m.nrUsi = aux->info.nrUsi;
		m.pret = aux->info.pret;
		m.serie = aux->info.serie;
	}

	return m;
}

int main() 
{
	HashTable hash;
	hash = citireMasiniDinFisier("masini.txt", 5);
	afisareTabelaDeMasini(hash);
	printf("Masina lui Ionescu este: ");
	Masina IonescuMasina = getMasinaDupaCheie(hash, "Ionescu");
	if (IonescuMasina.id != -1)
	{
		afisareMasina(IonescuMasina);
		free(IonescuMasina.model);
		free(IonescuMasina.numeSofer);
	}

	return 0;
}