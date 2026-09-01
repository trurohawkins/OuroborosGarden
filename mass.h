#pragma once
typedef struct {
	Form *self;
	linkedList *body;
} Mass;

Mass *makeMass(int id);
Form *addToMass(Mass *m, int x, int y);
bool removeFromMass(Mass *m, int x, int y);
void renderMass(Mass *m, RenderCommand reco);
