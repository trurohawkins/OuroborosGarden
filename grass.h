#pragma once
Form *makeGrass();
bool placeGrass(int x, int y);
bool growGrass(Form *g);
void spreadGrass(Form *g);
Form *checkGrass(int x, int y);
Color grassColor(Form *f);
void *renderGrass(void *data);
