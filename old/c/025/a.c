#include <ncurses.h>

int main() {

    initscr();

    int i = 23423;

    printw("%d\n",i);
    refresh();

    endwin();

    sleep(1);

    return 0;
}
