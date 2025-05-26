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
    char buffer[256];
    char sep[] = ",\n";
    Masina m1 = { 0, 0, 0.0f, NULL, NULL, 0 };

    if (fgets(buffer, sizeof(buffer), file) == NULL || strlen(buffer) < 2)
        return m1;

    char* aux = strtok(buffer, sep);
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
    if (!aux || strlen(aux) == 0) return m1;
    m1.serie = aux[0];

    return m1;
}

void afisareMasina(Masina masina) {
    printf("Id: %d\n", masina.id);
    printf("Nr. usi: %d\n", masina.nrUsi);
    printf("Pret: %.2f\n", masina.pret);
    printf("Model: %s\n", masina.model);
    printf("Nume sofer: %s\n", masina.numeSofer);
    printf("Serie: %c\n\n", masina.serie);
}

typedef struct NodLP NodLP;
typedef struct NodLS NodLS;

struct NodLS {
    NodLP* nodPrincipal;
    NodLS* next;
};

struct NodLP {
    Masina info;
    NodLS* vecini;
    NodLP* next;
};

void inserareLP(NodLP** cap, Masina m) {
    NodLP* nou = (NodLP*)malloc(sizeof(NodLP));
    nou->info = m;
    nou->next = NULL;
    nou->vecini = NULL;

    if (*cap == NULL) {
        *cap = nou;
    }
    else {
        NodLP* temp = *cap;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = nou;
    }
}

void inserareLS(NodLS** cap, NodLP* nodPrincipal) {
    NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
    nou->nodPrincipal = nodPrincipal;
    nou->next = *cap;
    *cap = nou;
}

NodLP* cautaNodDupaID(NodLP* cap, int id) {
    while (cap != NULL && cap->info.id != id) {
        cap = cap->next;
    }
    return cap;
}

void inserareMuchie(NodLP* cap, int idStart, int idStop) {
    NodLP* nodStart = cautaNodDupaID(cap, idStart);
    NodLP* nodStop = cautaNodDupaID(cap, idStop);

    if (nodStart && nodStop) {
        inserareLS(&nodStart->vecini, nodStop);
        inserareLS(&nodStop->vecini, nodStart);
    }
}

NodLP* citireNoduriMasiniDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        perror("Eroare deschidere fisier masini.txt");
        return NULL;
    }

    NodLP* cap = NULL;
    while (!feof(f)) {
        Masina m = citireMasinaDinFisier(f);
        if (m.model != NULL && m.numeSofer != NULL) {
            inserareLP(&cap, m);
        }
    }

    fclose(f);
    return cap;
}

void citireMuchiiDinFisier(NodLP* cap, const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        perror("Eroare deschidere fisier muchii.txt");
        return;
    }

    int idStart, idStop;
    while (fscanf(f, "%d %d", &idStart, &idStop) == 2) {
        inserareMuchie(cap, idStart, idStop);
    }

    fclose(f);
}

void dezalocareListaAdiacentaLS(NodLS* cap) {
    while (cap != NULL) {
        NodLS* temp = cap;
        cap = cap->next;
        free(temp);
    }
}

void dezalocareNoduriGraf(NodLP** cap) {
    while (*cap != NULL) {
        NodLP* temp = (*cap)->next;
        dezalocareListaAdiacentaLS((*cap)->vecini);
        free((*cap)->info.model);
        free((*cap)->info.numeSofer);
        free(*cap);
        *cap = temp;
    }
}

NodLP* citesteGrafMasini(const char* numeFisierMasini, const char* numeFisierMuchii) {
    NodLP* graf = citireNoduriMasiniDinFisier(numeFisierMasini);
    if (graf != NULL) {
        citireMuchiiDinFisier(graf, numeFisierMuchii);
    }
    return graf;
}

void afisareGraf(NodLP* graf) {
    while (graf != NULL) {
        afisareMasina(graf->info);
        printf("Vecini: ");
        NodLS* vecin = graf->vecini;
        while (vecin != NULL) {
            printf("%d ", vecin->nodPrincipal->info.id);
            vecin = vecin->next;
        }
        printf("\n\n");
        graf = graf->next;
    }
}

int main() {
    NodLP* graf = citesteGrafMasini("masini.txt", "muchii.txt");
    if (graf) {
        afisareGraf(graf);
        dezalocareNoduriGraf(&graf);
    }
    return 0;
}
