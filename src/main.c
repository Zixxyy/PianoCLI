#include "zpiano.h"
#include <stdio.h>

int main(void) {
    if (zpiano_init() != 0) {
        return 1;
    }

    // До-Ре-Ми
    zpiano_play(262, 60, 300); // C4
    zpiano_play(294, 60, 300); // D4
    zpiano_play(330, 60, 300); // E4
    zpiano_play(349, 70, 500); // F4

    zpiano_clear();
    zpiano_close();
    return 0;
}
