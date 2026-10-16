#pragma once 

void initClouds();
Mass *makeCloud();
void rain(void *);
int breeze(void *data, Action *a, float delta);
Form *placeClouds(int x, int y);
void *renderClouds(void *data);
void freeClouds();
