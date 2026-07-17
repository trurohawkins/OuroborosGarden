#pragma once
Form *makeDirt();

void addEco(int x, int y, float amnt);
float pullEco(int x, int y, float amnt);
float changeEco(Form *f, float amnt);
void dirtColor(Form *f);
Form *checkSoil(int x, int y);

void calcFlow(int x, int y);
void *renderDirt(void *data);
void dfsDirt(int x, int y, int max, Form **buffer);
