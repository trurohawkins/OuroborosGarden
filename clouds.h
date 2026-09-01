#pragma once 

void initClouds();
Mass *makeCloud();
void rain(void *);
void breeze(void *);
Form *placeClouds(int x, int y);
void *renderClouds(void *data);
void freeClouds();


