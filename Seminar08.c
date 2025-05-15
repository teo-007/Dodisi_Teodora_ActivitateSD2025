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

struct Nod;

typedef struct Nod
{
    Masina info;
    struct Nod* dr;
    struct Nod* st;
} Nod;

Masina citireMasinaDinFisier(FILE* file)
{
    char buffer[100];
    char sep[3] = ",\n";
    Masina m1 = { 0 };

    if (fgets(buffer, 100, file) == NULL)
        return m1;

    char* aux;
    aux = strtok(buffer, sep);
    if (aux == NULL) return m1;
    m1.id = atoi(aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) return m1;
    m1.nrUsi = atoi(aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) return m1;
    m1.pret = atof(aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) return m1;
    m1.model = malloc(strlen(aux) + 1);
    strcpy(m1.model, aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) return m1;
    m1.numeSofer = malloc(strlen(aux) + 1);
    strcpy(m1.numeSofer, aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) return m1;
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

void adaugaMasinaInArbore(Nod** nod, Masina masinaNoua)
{
    if ((*nod) != NULL)
    {
        if (masinaNoua.id > (*nod)->info.id)
        {
            adaugaMasinaInArbore(&(*nod)->dr, masinaNoua);
        }
        else if (masinaNoua.id < (*nod)->info.id)
        {
            adaugaMasinaInArbore(&(*nod)->st, masinaNoua);
        }
    }
    else
    {
        (*nod) = (Nod*)malloc(sizeof(Nod));
        (*nod)->info = masinaNoua;
        (*nod)->st = NULL;
        (*nod)->dr = NULL;
    }
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier)
{
    FILE* f = fopen(numeFisier, "r");
    if (!f) return NULL;
    Nod* nod = NULL;
    while (!feof(f))
    {
        Masina m = citireMasinaDinFisier(f);
        if (m.model != NULL && m.numeSofer != NULL)
            adaugaMasinaInArbore(&nod, m);
    }
    fclose(f);
    return nod;
}

void afisarePreOrdine(Nod* radacina)
{
    if (radacina != NULL)
    {
        afisareMasina(radacina->info);
        afisarePreOrdine(radacina->st);
        afisarePreOrdine(radacina->dr);
    }
}

void afisarePostOrdine(Nod* radacina)
{
    if (radacina != NULL)
    {
        afisarePostOrdine(radacina->st);
        afisarePostOrdine(radacina->dr);
        afisareMasina(radacina->info);
    }
}

void afisareInOrdine(Nod* radacina)
{
    if (radacina != NULL)
    {
        afisareInOrdine(radacina->st);
        afisareMasina(radacina->info);
        afisareInOrdine(radacina->dr);
    }
}

void afisareMasiniDinArbore(Nod* nod)
{
    printf("\n--- Preordine ---\n");
    afisarePreOrdine(nod);

    printf("\n--- Inordine ---\n");
    afisareInOrdine(nod);

    printf("\n--- Postordine ---\n");
    afisarePostOrdine(nod);
}

void dezalocareArboreDeMasini(Nod** radacina)
{
    if (*radacina != NULL) {
        dezalocareArboreDeMasini(&((*radacina)->st));
        dezalocareArboreDeMasini(&((*radacina)->dr));
        free((*radacina)->info.model);
        free((*radacina)->info.numeSofer);
        free(*radacina);
        *radacina = NULL;
    }
}

Masina getMasinaByID(Nod* radacina, int id)
{
    Masina m;
    m.id = -1;
    if (radacina != NULL)
    {
        if (id < radacina->info.id)
            return getMasinaByID(radacina->st, id);
        else if (id > radacina->info.id)
            return getMasinaByID(radacina->dr, id);
        else
            return radacina->info;
    }
    return m;
}

int determinaNumarNoduri(Nod* radacina)
{
    if (radacina == NULL) return 0;
    return 1 + determinaNumarNoduri(radacina->st) + determinaNumarNoduri(radacina->dr);
}

int calculeazaInaltimeArbore(Nod* radacina)
{
    if (radacina == NULL) return 0;
    int hSt = calculeazaInaltimeArbore(radacina->st);
    int hDr = calculeazaInaltimeArbore(radacina->dr);
    return 1 + (hSt > hDr ? hSt : hDr);
}

float calculeazaPretTotal(Nod* radacina) 
{
    if (radacina == NULL) return 0;
    return radacina->info.pret + calculeazaPretTotal(radacina->st) + calculeazaPretTotal(radacina->dr);
}

float calculeazaPretulMasinilorUnuiSofer(Nod* radacina, const char* numeSofer) 
{
    if (radacina == NULL) return 0;
    float suma = 0;
    if (strcmp(radacina->info.numeSofer, numeSofer) == 0)
        suma += radacina->info.pret;
    suma += calculeazaPretulMasinilorUnuiSofer(radacina->st, numeSofer);
    suma += calculeazaPretulMasinilorUnuiSofer(radacina->dr, numeSofer);
    return suma;
}

int main()
{
    Nod* rad = citireArboreDeMasiniDinFisier("masini_arbore.txt");

    afisareMasiniDinArbore(rad);

    printf("\nNumar noduri: %d\n", determinaNumarNoduri(rad));
    printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(rad));
    printf("Pret total: %.2f\n", calculeazaPretTotal(rad));
    printf("Pret masini sofer 'Ionel': %.2f\n", calculeazaPretulMasinilorUnuiSofer(rad, "Ionel"));

    Masina m = getMasinaByID(rad, 6);
    if (m.id != -1) {
        printf("\nMasina cu ID 6:\n");
        afisareMasina(m);
    }
    else {
        printf("\nMasina cu ID 6 nu a fost gasita.\n");
    }
    dezalocareArboreDeMasini(&rad);
    return 0;
}