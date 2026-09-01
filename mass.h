#pragma once
typedef struct {
	Form *self;
	linkedList *body;
	bool placed;
} Mass;

Mass *makeMass(int id);
Form *addToMass(Mass *m, int x, int y);
bool removeFromMass(Mass *m, int x, int y);
void renderMass(Mass *m, RenderCommand reco);
void removeMass(Mass *m);
void placeMass(Mass *m);
void moveMass(Mass *m, int xd, int yd);

void freeMass(void *mass);
