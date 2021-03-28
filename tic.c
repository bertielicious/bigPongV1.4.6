#include "config.h"
void tic(void)
{
    SOUND = LO;
    __delay_ms(10);
    SOUND = HI;
}
