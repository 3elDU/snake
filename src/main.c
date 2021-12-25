#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

#define SNAKE_CLR_FG COLOR_GREEN
#define FRUIT_CLR_FG COLOR_YELLOW
#define BIG_FRUIT_CLR_FG COLOR_MAGENTA

#define SNAKE_CLR 1
#define FRUIT_CLR 2
#define BIG_FRUIT_CLR 3

struct Tail {
	int x;
	int y;

	int index;
};

void print_help_menu() {
	clear();

	mvprintw(1, 0, "Control the snake with WASD");

	attron(COLOR_PAIR(SNAKE_CLR));
	mvprintw(3, 0, "This is the color of the snake");
	
	attron(COLOR_PAIR(FRUIT_CLR));
	mvprintw(4, 0, "This is the color of the food");

	attron(COLOR_PAIR(BIG_FRUIT_CLR));
	mvprintw(5, 0, "This is the color of the bonus");

	attroff(COLOR_PAIR(BIG_FRUIT_CLR));
	mvprintw(7, 0, "Press F to play");

	refresh();
	
	while (true) {
		switch (getch()) {
			case 'f':
				return;

			case 'F':
				return;

			default: break;
		}

		usleep(100000);
	}
}

int main() {
	// init ncurses
	initscr();

	if (!has_colors()) {
		puts("Terminal doesn't support colors!");

		endwin();
		return 1;
	}
	start_color();

	curs_set(0);
	noecho();
	raw();
	nodelay(stdscr, TRUE);
	set_escdelay(0);	

	init_pair(SNAKE_CLR, SNAKE_CLR_FG, COLOR_BLACK);
	init_pair(FRUIT_CLR, FRUIT_CLR_FG, COLOR_BLACK);
	init_pair(BIG_FRUIT_CLR, BIG_FRUIT_CLR_FG, COLOR_BLACK);

	print_help_menu();
	
	// the game begins here
	int snake_x = getmaxx(stdscr)/2, \
		snake_y = getmaxy(stdscr)/2;

	// movement vector
	int dir_x = 0, dir_y = 0;

	int score = 0;
	struct Tail *tails = calloc(getmaxx(stdscr) * getmaxy(stdscr), sizeof(struct Tail));

	bool running = TRUE;
	while (running) {

		int key = getch();

		if (key == 'a' || key == 'A') 		{ dir_y = 0;  dir_x = -1; }
		else if (key == 's' || key == 'S')	{ dir_y = 1;  dir_x = 0;  }
		else if (key == 'w' || key == 'W')	{ dir_y = -1; dir_x = 0; }
		else if (key == 'd' || key == 'D') 	{ dir_x = 1;  dir_y = 0; }

		else if (key == 'q' || key == 'Q') running = FALSE;

		clear();

		attron(COLOR_PAIR(SNAKE_CLR));
		mvaddch(snake_y, snake_x, 'O');
		attroff(COLOR_PAIR(SNAKE_CLR));

		refresh();

		snake_x += dir_x, snake_y += dir_y;

		if (dir_y != 0) usleep(50000);
		else usleep(25000);
	}

	// exit
	free(tails);
	endwin();
	return 0;
}
