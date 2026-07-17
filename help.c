void incPos(int *px, int *py, int dx, int dy) {
	World *w = &theWorld;//getWorld();
	if ((*px + dx) < 0) {
		*px = w->x + (*px + dx);
	} else {
		*px = (*px + dx) % w->x;
	}

	if ((*py + dy) < 0) {
		*py = w->y + (*py + dy);
	} else {
		*py = (*py + dy) % w->y;
	}
}


