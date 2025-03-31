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

struct nod
{
	Masina info;
	struct nod* next;
	struct nod* prev;
};

typedef struct nod nod;

struct lista_dubla
{
	nod* first;
	nod* last;
};

typedef struct lista_dubla lista_dubla;

Masina citireMasinaDinFisier(FILE* file)
{
	char buffer[100];
	char sep[3] = ",\n";
	Masina m1;
	m1.model = NULL;
	m1.numeSofer = NULL;

	if (fgets(buffer, 100, file) == NULL) 
	{
		m1.id = -1;  
		return m1;
	}

	char* aux = strtok(buffer, sep);
	if (aux == NULL) 
	{
		m1.id = -1;
		return m1;
	}
	m1.id = atoi(aux);

	aux = strtok(NULL, sep);
	if (aux == NULL) 
	{
		m1.id = -1;
		return m1;
	}
	m1.nrUsi = atoi(aux);

	aux = strtok(NULL, sep);
	if (aux == NULL) 
	{
		m1.id = -1;
		return m1;
	}
	m1.pret = atof(aux);

	aux = strtok(NULL, sep);
	if (aux == NULL) 
	{
		m1.id = -1;
		return m1;
	}
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	if (aux == NULL) 
	{
		m1.id = -1;
		free(m1.model);
		return m1;
	}
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	if (aux == NULL) 
	{
		m1.id = -1;
		free(m1.model);
		free(m1.numeSofer);
		return m1;
	}
	m1.serie = *aux;

	return m1;
}

void afisareMasina(Masina masina) 
{
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasiniDeLaInceput(lista_dubla lista) 
{
	nod* p = lista.first;
	while (p != NULL)
	{
		afisareMasina(p->info);
		p = p->next;
	}
}

void afisareListaMasiniDeLaSfarsit(lista_dubla lista)
{
	nod* aux = lista.last;
	while (aux != NULL)
	{
		afisareMasina(aux->info);
		aux = aux->prev;
	}
}

void adaugaMasinaInListaLaSfarsit(lista_dubla* lista, Masina masinaNoua)
{
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = masinaNoua;
	nou->prev = lista->last;
	nou->next = NULL;
	if (lista->last != NULL)
	{
		lista->last->next = nou;
	}
	else
	{
		lista->first = nou;
	}
	lista->last = nou;
}

void adaugaLaInceputInLista(lista_dubla* list, Masina masinaNoua)
{
	nod* aux = (nod*)malloc(sizeof(nod));
	aux->info = masinaNoua;
	aux->next = list->first;
	aux->prev = NULL;
	if (list->first != NULL)
	{
		list->first->prev = aux;
	}
	else
	{
		list->last = aux;
	}
	list->first = aux;

}

lista_dubla citireLDMasiniDinFisier(const char* numeFisier) 
{
	FILE* f = fopen(numeFisier, "r");
	lista_dubla lista;
	lista.first = NULL;
	lista.last = NULL;
	while (!feof(f))
	{
		Masina aux;
		aux = citireMasinaDinFisier(f);
		adaugaMasinaInListaLaSfarsit(&lista, aux); //shallow copy
	}
	fclose(f);
	return lista;
}

void dezalocareLDMasini(lista_dubla* lista) 
{
	while (lista->first != NULL)
	{
		nod* aux = lista->first;
		free(aux->info.model);
		free(aux->info.numeSofer);
		lista->first = lista->first->next;	
		free(aux);
	}
	lista->last = NULL;
}

float calculeazaPretMediu(lista_dubla lista) 
{
	float suma = 0;
	unsigned int k = 0;
	lista.first->info.pret;
	while (lista.first != NULL)
	{
		suma += lista.first->info.pret;
		k++;
		lista.first = lista.first->next;
	}

	return (suma>0)? suma/k : 0;
}

void stergeMasinaDupaID(/*lista masini*/ int id) 
{
	//sterge masina cu id-ul primit.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
}

char* getNumeSoferMasinaScumpa(lista_dubla lista)
{
	if (lista.first == NULL) { return NULL;}

	nod* temp = lista.first;
	nod* max = lista.first;

	while (temp != NULL)
	{
		if (temp->info.pret > max->info.pret)
		{
			max = temp;
		}
		temp = temp->next;
	}

	char* numeSofer = malloc(strlen(max->info.numeSofer) + 1);
	strcpy_s(numeSofer, strlen(max->info.numeSofer) + 1, max->info.numeSofer);

	return numeSofer;
}


int main() 
{
	lista_dubla LD = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasiniDeLaInceput(LD);
	printf("--------------------\n");

	afisareListaMasiniDeLaInceput(LD);

	printf("Pretul mediu al masinilor este: %.2f\n", calculeazaPretMediu(LD));

	printf("\nNumele soferului masinii celei mai scumpe este: %s\n", getNumeSoferMasinaScumpa(LD));

	dezalocareLDMasini(&LD);

	return 0;
}