#include <genesis.h>
#include "resources.h"
#include "functions.h"
#include "title.h"
#include "part1.h"
#include "part2.h"
#include "part3.h"

int main()
{
    SYS_disableInts();
    VDP_setPaletteColors(0, (u16*) palette_black, 64);
    VDP_setScreenHeight240();
    SYS_enableInts();

    XGM_startPlay(song); // This starts the song!
    XGM_setLoopNumber(0);
    title();
    part1();
    part2();
    part3();
    while(TRUE){
        VDP_waitVSync();
    }
}