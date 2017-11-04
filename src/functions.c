#include <genesis.h>
#include "functions.h"

// Sleep a number of frames
void sleep(u16 x){
    for (u16 i = 0; i < x; i++){
        VDP_waitVSync();
    }
}

// Draw an Int to the screen, Good for debugging:
void VDP_drawInt(s32 num, u16 x, u16 y, u16 min){
    char data[16];
    intToStr(num,data,min);
    VDP_drawText(data,x,y);
}

// Draw a fix32 to the screen. DrawInt won't do this.
void VDP_drawFix32(fix32 num, u16 x, u16 y, u16 min){
    char data[16];
    fix32ToStr(num,data,min);
    VDP_drawText(data,x,y);
}
