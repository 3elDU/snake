#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>

#define SNAKE_CLR_FG COLOR_GREEN
#define FRUIT_CLR_FG COLOR_YELLOW
#define GAME_OVER_CLR_FG COLOR_MAGENTA

#define SNAKE_CLR 1
#define FRUIT_CLR 2
#define GAME_OVER_CLR 3

#define SNAKE_SPEED 0.5

struct Tail {
	int index;
};

void print_help_menu() {
	clear();

	mvprintw(1, 0, "Control the snake with WASD");

	attron(COLOR_PAIR(SNAKE_CLR));
	mvprintw(3, 0, "This is the color of the snake");
	
	attron(COLOR_PAIR(FRUIT_CLR));
	mvprintw(4, 0, "This is the color of the food");

	attroff(COLOR_PAIR(FRUIT_CLR));
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

// 0 - the user wants to exit
// 1 - the user wants to play again
unsigned print_lose_menu() {
	erase();

	const char* message1 = "GAME OVER!";
	const char* message2 = "Press Q to exit, or F to play again.";

	attron(COLOR_PAIR(GAME_OVER_CLR));
	mvprintw(getmaxy(stdscr)/2, getmaxx(stdscr)/2-strlen(message1)/2, message1);
	attroff(COLOR_PAIR(GAME_OVER_CLR));

	mvprintw(getmaxy(stdscr)/2+1, getmaxx(stdscr)/2-strlen(message2)/2, message2);

	refresh();

	// wait for keys
	while (true) {
		int key = getch();

		if (key == 'q' || key == 'Q') return 0;
		else if (key == 'f' || key == 'F') return 1;

		usleep(100000);
	}
}

int randomRange(int from, int to) {
	return from+rand()%(to+1-from);
}

void play_game() {
// remember width and height of the terminal
	const unsigned width = getmaxx(stdscr);
	const unsigned height = getmaxy(stdscr);
	
	// the game begins here
	int snake_x = getmaxx(stdscr)/2, \
		snake_y = getmaxy(stdscr)/2;

	// movement vector
	int dir_x = 0, dir_y = 0;

	// food location
	int food_x = randomRange(0, width-1), \
		food_y = randomRange(0, height-1);

	int score = 0;

	struct Tail *tails = calloc(width * height, sizeof(struct Tail));

	clear();

	bool running = TRUE;
	while (running) {

		int key = getch();

		if ((key == 'a' || key == 'A')		&& dir_x != 1) 		{ dir_y = 0;  dir_x = -1; }
		else if ((key == 's' || key == 'S')	&& dir_y != -1)		{ dir_y = 1;  dir_x = 0;  }
		else if ((key == 'w' || key == 'W')	&& dir_y != 1)		{ dir_y = -1; dir_x = 0; }
		else if ((key == 'd' || key == 'D')	&& dir_x != -1) 	{ dir_x = 1;  dir_y = 0; }

		else if (key == 'q' || key == 'Q') running = FALSE;


		erase();

		// draw tails
		attron(COLOR_PAIR(SNAKE_CLR));
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				
				// if tail collides with snake head, end the agme
				if (tails[y*width+x].index && x == snake_x && y == snake_y) {
					running = FALSE;
				}

				if (tails[y*width+x].index) {
					mvaddch(y, x, '+');
				}
			}
		}
		attroff(COLOR_PAIR(SNAKE_CLR));


		// draw food
		attron(COLOR_PAIR(FRUIT_CLR));
		mvaddch(food_y, food_x, '$');
		attroff(COLOR_PAIR(FRUIT_CLR));

		// draw snake's head
		attron(COLOR_PAIR(SNAKE_CLR));
		mvaddch(snake_y, snake_x, 'O');
		attroff(COLOR_PAIR(SNAKE_CLR));

		refresh();



		// update the game logic

		// delete "old" tails
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				if (tails[y*width+x].index > 0) tails[y*width+x].index--; 
			}
		}
		
		if (snake_x == food_x && snake_y == food_y) {
			score++;
			food_x = randomRange(0, width-1);
			food_y = randomRange(0, height-1);
		}

		tails[snake_y*width+snake_x].index = score;
		snake_x += dir_x, snake_y += dir_y;

		// if snake goes behind the edge, teleport it to another edge
		if (snake_x < 0) snake_x = width-1;
		else if (snake_x >= width) snake_x = 0;

		if (snake_y < 0) snake_y = height-1;
		else if (snake_y >= height) snake_y = 0;

		if (dir_y != 0) usleep(50000 * (1.0/SNAKE_SPEED));
		else usleep(25000 * (1.0/SNAKE_SPEED));
	}

	free(tails);
}

int main() {
	// init RNG
	srand((unsigned)time(NULL));

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
	init_pair(GAME_OVER_CLR, GAME_OVER_CLR_FG, COLOR_BLACK);

	print_help_menu();
	
	bool play = TRUE;
	while (play) {
		play_game();

		play = print_lose_menu();
	}

	// exit
	endwin();
	return 0;
}
