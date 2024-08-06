/**
 * @file main.c
 * @brief 'main' and main game loop
 * @author brennelul
 */

#include "tetris.h"

/**
 * @brief The main function of any program
 */
int main() {
  srand(time(NULL));
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, true);
  timeout(10);
  game_loop();
  endwin();
  return 0;
}

/**
 * @ingroup other_funcs
 * @brief The main game loop, initializes and starts the game
 */
void game_loop() {
  FSM_STATES_g state = START;
  GameInfo_t *stats = updateCurrentState();
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  long last_time = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
  long current_time;
  UserAction_t sig = 0;
  while (state != EXIT_STATE) {
    print_something(state);
    sig = getch();
    userInput(&state, get_signal(sig));
    while (state == MOVING) {
      sig = getch();
      moving_state(&state, get_signal(sig));
      print_game();
      clock_gettime(CLOCK_MONOTONIC, &ts);
      current_time = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
      if (current_time - last_time > stats->speed) {
        move_down(&state);
        last_time = current_time;
      }
    }
    refresh();
  }
}
