#include<stdio.h>
#include<malloc.h>

struct Masina
{
	int id;
	int nrKm;
	char* model;
	float pret;
	char tipMotor;
};

struct Masina initializare(int id, int nrKm, const char* model, float pret, char tipMotor)
{
	struct Masina m;
	m.id = id;
	m.nrKm = nrKm;
	m.pret = pret;
	m.tipMotor = tipMotor;
	m.model = (const char*)malloc(strlen(model) * sizeof(const char) + 1);
	strcpy(m.model, model);
	return m;
}

void afisare(struct Masina m)
{
	printf("ID: %d\nNrKm: %d\nModel: %s\nPret: %.2f\nTipMotor: %c\n\n", m.id, m.nrKm, m.model, m.pret, m.tipMotor);
}

void afisareVector(struct Masina* vector, int nrElemente)
{
	for (int i = 0; i < nrElemente; i++)
	{
		afisare(vector[i]);
	}
}

struct Masina* copiazaPrimeleNElemente(struct Masina* vector, int nrElemente, int* nrElementeCopiate)
{
	struct Masina* vectorNou = NULL;

	if (vector == NULL || *nrElementeCopiate <= 0)
	{
		return vectorNou;
	}

	if (*nrElementeCopiate > nrElemente)
	{
		*nrElementeCopiate = nrElemente;
	}

	vectorNou = malloc(sizeof(struct Masina) * *nrElementeCopiate);

	for (int i = 0; i < *nrElementeCopiate; i++)
	{
		vectorNou[i] = initializare(vector[i].id, vector[i].nrKm, vector[i].model, vector[i].pret, vector[i].tipMotor);
	}

	return vectorNou;
}

void dezalocare(struct Masina* m)
{
	free((*m).model);
	(*m).model = NULL;
}

void dezalocareVector(struct Masina** vector, int* nrElemente)
{
	for (int i = 0; i < *nrElemente; i++)
	{
		if ((*vector)[i].model != NULL)
		{
			free((*vector)[i].model);
		}
	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaMasiniIeftine(struct Masina* vector, char nrElemente, float prag, struct Masina** vectorNou, int* dimensiune)
{
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++)
	{
		if (vector[i].pret < prag)
		{
			(*dimensiune) += 1;
		}
	}

	*vectorNou = malloc(sizeof(struct Masina) * *dimensiune);
	int k = 0;
	for (int i = 0; i < nrElemente; i++)
	{
		if (vector[i].pret < prag)
		{
			(*vectorNou)[k] = vector[i];
			(*vectorNou)[k].model = malloc(sizeof(char) * strlen(vector[i].model) + 1);
			strcpy((*vectorNou)[k].model, vector[i].model);
			k++;
		}
	}
}

struct Masina getPrimulElementConditionat(struct Masina* vector, int nrElemente, const char* conditie)
{
	struct Masina m;
	m.id = 1;

	return m;
}

int main() {
	int nrElemente = 3;
	struct Masina* vec;
	vec = malloc(sizeof(struct Masina) * nrElemente);

	vec[0] = initializare(5, 200000, "Lamborghini", 1000.3, 'B');
	vec[1] = initializare(3, 80000, "Porche", 30100.5, 'D');
	vec[2] = initializare(8, 100, "Cyber Truck", 2000, 'E');


	int nrElementeCopiate = 5;
	struct Masina* vec2;
	vec2 = copiazaPrimeleNElemente(vec, nrElemente, &nrElementeCopiate);


	struct Masina* vec3;
	int dimensiuneVec3;
	copiazaMasiniIeftine(vec, nrElemente, 100000, &vec3, &dimensiuneVec3);
	afisareVector(vec3, dimensiuneVec3);

	dezalocareVector(&vec, &nrElemente);
	dezalocareVector(&vec2, &nrElementeCopiate);
	return 0;
}