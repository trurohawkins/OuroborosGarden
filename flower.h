#pragma once

Form *makeFlower();
bool placeFlower(int x, int y);
bool growFlower(Form *f);
void *renderFlower(void *data);
void spreadFlower(int x, int y);

