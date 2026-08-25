#pragma once
Form *makeDirt();
Form *placeDirt(int x, int y);

float addEco(int x, int y, float amnt);
float pullEco(int x, int y, float amnt);
void pushEco(int x, int y, float amnt);

void dirtFlow(void *);
void spreadEco(Form *from, int x, int y);
void ecoEvaporation(void *);

float changeEco(Form *form, float amnt);
Form *checkSoil(int x, int y);

void calcFlow(int x, int y);
void *renderDirt(void *data);
Color dirtColor(Form *dirt);
void dfsDirt(int x, int y, int max, Form **buffer);
float calcIntake(float eco);

