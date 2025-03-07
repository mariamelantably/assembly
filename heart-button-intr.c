#include "hardware.h"


void advance(void){
    if (mode == 1){
        while (1){
        show(heart, 70);
        show(small, 10);
        show(heart, 10);
        show(small, 10);
        }
    }
}



/* delay -- pause for n microseconds */
void delay(unsigned n)
{
    unsigned t = 2*n;
    while (t > 0) {
        /* 500nsec per iteration at 16MHz */
        nop(); nop(); nop();
        t--;
    }
}

static unsigned mode = 1;

/* heart -- GPIO values for heart image */
const unsigned heart[] = {
    0x28f0, 0x5e00, 0x8060
};

/* small -- GPIO values for small heart */
const unsigned small[] = {
    0x2df0, 0x5fb0, 0x8af0
};

#define JIFFY 5000              /* Delay in microsecs */

/* show -- display three rows of a picture n times */
void show(const unsigned img[], int n)
{
    while (n-- > 0) {
        /* Takes 15msec per iteration */
        for (int p = 0; p < 3; p++) {
            GPIO_OUT = img[p];
            delay(JIFFY);
        }
    }
}

void gpiote_handler(void) {
    mode = 1 - mode;
    advance();
}


/* init -- main program */
void init(void)
{
    GPIO_DIR = 0xfff0;
    GPIO_PINCNF[BUTTON_A] = 0;
    GPIO_PINCNF[BUTTON_B] = 0;

    /* Set row pins to high-drive mode to increase brightness */
    SET_FIELD(GPIO_PINCNF[ROW1], GPIO_PINCNF_DRIVE, GPIO_DRIVE_S0H1);
    SET_FIELD(GPIO_PINCNF[ROW2], GPIO_PINCNF_DRIVE, GPIO_DRIVE_S0H1);
    SET_FIELD(GPIO_PINCNF[ROW3], GPIO_PINCNF_DRIVE, GPIO_DRIVE_S0H1);

    while (1) {
        advance();
    }
}