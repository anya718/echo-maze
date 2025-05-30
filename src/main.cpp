#include "display.h"
#include "utils.h"
#include "game.h"

int main() {
    GPIO_init();
    display_init();

	while(1) {
        start();
        play_level();
	}
 
	return 0;
}
