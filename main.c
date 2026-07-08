#include <FormEngine.h>
#include <TUI.h>
#include <AudioMan.h>

int worldX = 80;
int worldY = 40;

int main() {
	startWorld(true);

	makeWorld(worldX, worldY);
	setFrameDimension(worldX, worldY);
	setFramePosition(worldX/2, worldY/2);

	runWorld();

	endWorld();
	return 0;
}
