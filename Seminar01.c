#include<stdio.h>
#include<stdlib.h>

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

void modifica_Pret(struct Masina* m, float pretNou)
{
	if (pretNou > 0);
	(*m).pret = pretNou;
}

void dezalocare(struct Masina* m) {
	free((*m).model);
	(*m).model = NULL;
}

int main() {
	struct Masina masina = initializare(1, 100, "Prosche", 4000.50, 'B'); // 'B'
	afisare(masina);
	modifica_Pret(&masina, 3000.50);
	afisare(masina);

	dezalocare(&masina);
	afisare(masina);
	return 0;
}