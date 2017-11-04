#include <genesis.h>
#include "resources.h"
#include "functions.h"
#include <genesis.h>
#include "functions.h"
#include "part1.h"

s16 offset = 1;
s16 HScroll = 0;

Sprite* sprites[40];

u16 noteColours[6] = {0x00EE,0x0000,0x00E0,0x0151,0x0EEE,0x0080};

// Forward
void HIntCallback();
void part1();
void pianoScreen();

void part1(){
    u16 palette[64];
    u16 ind;

    // Bar 6
    // Display Part 1
    SYS_disableInts();
    VDP_clearPlan(PLAN_A, TRUE);
    ind = TILE_USERINDEX;
    VDP_drawImageEx(PLAN_A, &part, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 7, 12, FALSE, TRUE);
    ind += part.tileset->numTile;
    VDP_drawImageEx(PLAN_A, &Iimage, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 29, 12, FALSE, TRUE);
    SYS_enableInts();
    memcpy(&palette[0], part.palette->data, 16 * 2);
    VDP_fadeIn(0, 63, palette, 8, FALSE);
    sleep(112);
    VDP_fadeOutAll(8, FALSE);

    // Bar 6 & 1/2
    // Display Piano text.
    SYS_disableInts();
    VDP_clearPlan(PLAN_A, TRUE);
    ind = TILE_USERINDEX;
    VDP_drawImageEx(PLAN_A, &pianoText, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 9, 12, FALSE, TRUE);
    SYS_enableInts();
    memcpy(&palette[0], pianoText.palette->data, 16 * 2);
    VDP_fadeIn(0, 63, palette, 8, FALSE);
    sleep(184); // Timing is no longer perfect becuase I had to make up for lag frames.
    VDP_fadeOutAll(8, FALSE);

    // Bar 7
    SYS_disableInts();
    VDP_loadTileData(image2.tileset->tiles, TILE_USERINDEX, 1, FALSE);
    VDP_fillTileMapRect(PLAN_A , TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, TILE_USERINDEX), 0, 0, 40, 30);
    memcpy(&palette[0], palette_grey, 16 * 2);
    SYS_enableInts();
    VDP_fadeIn(0,63, palette, 16, FALSE);
    sleep(16);
    /*SYS_disableInts();
    VDP_loadTileData(image3.tileset->tiles, TILE_USERINDEX, 1, FALSE);
    SYS_enableInts();
    sleep(16);
    SYS_disableInts();
    VDP_loadTileData(image2.tileset->tiles, TILE_USERINDEX, 1, FALSE);
    SYS_enableInts();
    sleep(16);*/
    SYS_disableInts();
    VDP_loadTileData(image1.tileset->tiles, TILE_USERINDEX, 1, FALSE);
    SYS_enableInts();
    sleep(16);
    VDP_fadeOutAll(16,FALSE);

    // Bar 7.5
    SYS_disableInts();
    ind = TILE_USERINDEX;
    VDP_drawImageEx(PLAN_B, &space, TILE_ATTR_FULL(PAL0, TRUE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
    ind += space.tileset->numTile;
    VDP_drawImageEx(PLAN_A, &piano, TILE_ATTR_FULL(PAL1, TRUE, FALSE, FALSE, ind), 0, 13, FALSE, TRUE);
    SYS_enableInts();
    memcpy(&palette[0], space.palette->data, 16 * 2);
    memcpy(&palette[16], piano.palette->data, 16 * 2);
    VDP_setPaletteColor(33, noteColours[0]);
    VDP_setBackgroundColor(0);
    VDP_fadeIn(0,32,palette,16,TRUE);

    pianoScreen();

}

void HIntCallback(){
    offset = -offset;
    VDP_setHorizontalScroll(PLAN_B,(HScroll * offset) - 40);
}

fix32 noteVel[40];
fix32 notePos[40];
u16 noteBounced[40];
u16 noteSpawned[40];
u16 noteX[40];
u16 bounceDistance[40];
// It takes 2 beats for the notes to hit the keys
// A multiple of 16 will land you on a black key
// 9 + a multiple of 16 will land you on a white key

#define F1 9
#define Fs1 16
#define G1 25
#define Gs1 32
#define A1 41
#define As1 48
#define B1 57
#define C2 73
#define Cs2 80
#define D2 89
#define Ds2 96
#define E2 105
#define F2 121
#define Fs2 128
#define G2 137
#define Gs2 144
#define A2 153
#define As2 160
#define B2 169
#define C3 185
#define Cs3 192
#define D3 201
#define Ds3 208
#define E3 217
#define F3 233
#define Fs3 240
#define G3 249
#define Gs3 256
#define A3 265
#define As3 272
#define B3 281
#define C4 297
#define Cs4 304
                      
u16 noteValues[100] =     {G2 ,A2 ,G2 ,G1 ,D2 ,C2 ,B1 ,C2 ,D2 ,G1 ,F1 ,G1 ,A1 ,C2 ,B1 ,C2 ,D2 ,A1 ,As1,B1 ,As1,A1 ,G1 ,F1 ,F1 ,Fs1,G1 ,A1 ,F1 ,B1 ,G1 ,D2 ,G2 ,D2 ,A2 ,D3 ,G2 ,D2 ,B2 ,G2 ,D2 ,B2 ,A2 ,Fs2,D2 ,B1 ,G1 ,A1 ,C2 ,D2 ,A2 ,B2 ,D3 ,Fs3,D3 ,B2 ,A2 ,B2 ,F3 ,Cs3,As2,Gs2,As2,Cs3,Ds3,F3 ,E3 ,C3 ,A2 ,G2 ,A2 ,C3 ,E3 ,Ds3,F3 ,G3 ,As3,B3 ,As3,G3 ,Ds3,D3 ,Ds3,E3 };
u16 timeToNextNote[100] = {12 ,20 ,12 ,4  ,12 ,4  ,12 ,4  ,12 ,16 ,4  ,12 ,4  ,16 ,16 ,16 ,12 ,2  ,2  ,4  ,2  ,2  ,2  ,16 ,4  ,12 ,4  ,12 ,4  ,12 ,4  ,12 ,16 ,4  ,16 ,12 ,4  ,16 ,12 ,4  ,12 ,4  ,12 ,4  ,12 ,16 ,4  ,12 ,4  ,64 ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,4  ,2  ,2  ,0  };
u16 keyType[100] =        {0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,1  ,0  ,1  ,0  ,0  ,0  ,0  ,1  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,1  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,1  ,0  ,0  ,0  ,0  ,0  ,1  ,1  ,1  ,1  ,1  ,1  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,1  ,0  ,0  ,1  ,0  ,1  ,0  ,1  ,0  ,1  ,0  };
u16 noteColourCounter = 0;

#define NUMBER_OF_NOTES (sizeof(noteValues)/sizeof(noteValues[0]))

void pianoScreen(){
    // ======================================================================================================
    // Start of piano section.
    //

    fix32 xvel = FIX32(0);
    fix32 xpos = FIX32(-10);
    fix32 directionx = FIX32(1);

    fix32 yvel = 0;
    fix32 ypos = 0;
    fix32 directiony = FIX32(1);

    u16 noteCounter = 0;
    u16 noteIndex = 0;
    VDP_setVerticalScroll(PLAN_B,0);

    SYS_setHIntCallback(&HIntCallback);
    VDP_setHInterrupt(TRUE);

    VDP_setPaletteColor(33, 0x04C4);

    SPR_init(0,0,0);
    sprites[0] = SPR_addSpriteEx(&note, 140, 140, TILE_ATTR(PAL2, FALSE, FALSE, FALSE), TILE_FONTINDEX,
        SPR_FLAG_AUTO_VISIBILITY | SPR_FLAG_AUTO_SPRITE_ALLOC | SPR_FLAG_AUTO_TILE_UPLOAD); // Create refrence tile.

    SPR_update();
    // Repeat for 5.5 bars:
    for (u16 frame = 0; frame < 704; frame++){
        // Do movement of the top.
        xvel += directionx;
        xpos += xvel + FIX32(0.04);
        if (xvel > FIX32(1.5)){
            directionx = FIX32(-0.1);
        }else if (xvel < FIX32(-1.5)){
            directionx = FIX32(0.1);
        }
        HScroll = fix32ToInt(xpos);

        yvel += directiony + FIX32(0.002);
        ypos = ypos + yvel;
        if (yvel > FIX32(1.75)){
            directiony = FIX32(-0.05);
        }else if (yvel < FIX32(-1.75)){
            directiony = FIX32(0.05);
        }
        VDP_setVerticalScroll(PLAN_B,fix32ToInt(ypos)+20);

        // Handle the sprites falling from the top:
        if (noteCounter == 0){
            noteCounter = timeToNextNote[noteIndex];
            for (u16 x = 1; x < 40; x++){
                if (noteSpawned[x] == FALSE){
                    sprites[x] = SPR_addSpriteEx(&note, x * 16, -16, TILE_ATTR(PAL2, TRUE, FALSE, FALSE), TILE_FONTINDEX,
                        SPR_FLAG_AUTO_VISIBILITY | SPR_FLAG_AUTO_SPRITE_ALLOC);
                    noteSpawned[x] = TRUE;
                    noteVel[x] = FIX32(0);
                    notePos[x] = FIX32(-16);
                    noteBounced[x] = FALSE;
                    noteX[x] = noteValues[noteIndex];
                    bounceDistance[x] = keyType[noteIndex];
                    break;
                }
            }
            noteIndex += 1;
            noteIndex = noteIndex % NUMBER_OF_NOTES;
        }
        for (u16 i = 1; i < 40; i++){
            if (noteSpawned[i] == TRUE){
                if (noteBounced[i] == FALSE && notePos[i] > FIX32(210 - (20 * bounceDistance[i]))){
                    VDP_setPaletteColor(33, noteColours[noteColourCounter % 6]);
                    VDP_setPaletteColor(34, noteColours[(noteColourCounter-1) % 6]);
                    noteColourCounter += 1;
                    noteBounced[i] = TRUE;
                    noteVel[i] = FIX32(-1);
                }
                noteVel[i] += FIX32(0.1);
                notePos[i] += noteVel[i];
                SPR_setPosition(sprites[i], noteX[i], fix32ToInt(notePos[i]));
                if(fix32ToInt(notePos[i]) > 240){
                    SPR_releaseSprite(sprites[i]);
                    noteSpawned[i] = FALSE;
                }
            }
        }
        noteCounter -= 1;
        SPR_update();
        VDP_waitVSync();
    }
    VDP_setHInterrupt(FALSE);
    SYS_setHIntCallback(0);
    VDP_fadeOutAll(8,FALSE);

    SPR_end();
}
