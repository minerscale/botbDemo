#include <genesis.h>
#include "resources.h"
#include "functions.h"
#include <genesis.h>
#include "functions.h"
#include "part2.h"

Sprite* sprites[40];
u16 spriteSpawned[40];
fix32 spriteX[40];
fix32 spriteY[40];
s16 shipPositions[20] = {100, 20, 100, 20, 100, 20, 10, 20, 10, 20, 80, 40, 160,20};
u16 shipMovTime[20]   = {0  , 20, 20 , 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 ,20};
fix32 shipVel;
fix32 spriteVel;
u16 HScrolls = 0;
u16 offset;
u16 otherOffset1;
u16 otherOffset2;

u16 palette_white[64] = {[0 ... 63] = 0x0FFF};

void HIntsCallback(){
    offset = -offset;
    VDP_setHorizontalScroll(PLAN_B,(HScrolls * offset) + otherOffset1);
    VDP_setHorizontalScroll(PLAN_A,(HScrolls * offset) + otherOffset2);
}

void part2(){
    u16 palette[64];
    u16 ind;

    // Display Part 2
    SYS_disableInts();
    VDP_clearPlan(PLAN_A, TRUE);
    VDP_clearPlan(PLAN_B, TRUE);
    ind = TILE_USERINDEX;
    VDP_drawImageEx(PLAN_A, &part, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 7, 12, FALSE, TRUE);
    ind += part.tileset->numTile;
    VDP_drawImageEx(PLAN_A, &IIimage, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 29, 12, FALSE, TRUE);
    SYS_enableInts();
    memcpy(&palette[0], part.palette->data, 16 * 2);
    VDP_fadeIn(0, 63, palette, 8, FALSE);
    sleep(96);
    VDP_fadeOutAll(16, FALSE);

    // Bar 14
    // Display Parallax:
    SYS_disableInts();
    VDP_clearPlan(PLAN_A, TRUE);
    ind = TILE_USERINDEX;
    VDP_drawImageEx(PLAN_A, &parallax, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 1, 12, FALSE, TRUE);
    SYS_enableInts();
    memcpy(&palette[0],parallax.palette->data, 16*2);
    VDP_fadeIn(0, 63, palette, 8, FALSE);
    sleep(112);
    VDP_fadeOutAll(8,FALSE);

    SPR_init(0,0,0);
    // Paralax scrolling!
    SYS_disableInts();
    VDP_setScreenWidth256();
    VDP_setPlanSize(32,32);
    VDP_clearPlan(PLAN_A, TRUE);
    ind = TILE_USERINDEX;
    VDP_drawImageEx(PLAN_B, &sydneyBG, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
    ind += sydneyBG.tileset->numTile;
    VDP_drawImageEx(PLAN_A, &sydneyFG, TILE_ATTR_FULL(PAL1, TRUE, FALSE, FALSE, ind), 0, 7, FALSE, TRUE);
    SYS_enableInts();
    memcpy(&palette[0],sydneyBG.palette->data,16*2);
    memcpy(&palette[16],sydneyFG.palette->data,16*2);
    memcpy(&palette[32],gradius.palette->data,16*2);
    memcpy(&palette[48],spinThing.palette->data,16*2);
    VDP_fadeIn(0, 63, palette, 8, TRUE);

    sprites[0] = SPR_addSpriteEx(&gradius, -24, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE),0,
        SPR_FLAG_AUTO_VISIBILITY | SPR_FLAG_AUTO_SPRITE_ALLOC | SPR_FLAG_AUTO_TILE_UPLOAD);
    SPR_setVRAMTileIndex(sprites[0],0x640);

    sprites[1] = SPR_addSpriteEx(&spinThing, 0, 220, TILE_ATTR(PAL3, FALSE, FALSE, FALSE),0,
        SPR_FLAG_AUTO_VISIBILITY | SPR_FLAG_AUTO_SPRITE_ALLOC | SPR_FLAG_AUTO_TILE_UPLOAD);
    SPR_setVRAMTileIndex(sprites[1],0x646);

    fix32 shipPosX = FIX32(-24);
    fix32 shipPosY = FIX32(80);

    setRandomSeed(35812);
    u16 index = 0;
    u16 animIndex = 1;
    u16 animWait = shipMovTime[1];
    for (u16 frame = 0; frame < 1248; frame++){
        VDP_setHorizontalScroll(PLAN_A,-frame/2);
        VDP_setHorizontalScroll(PLAN_B,(-frame/4)+50);

        if (frame%16 == 0 && frame < 1120){
            for (index = 2; index < 40; index++){
                if (spriteSpawned[index] == FALSE){
                    sprites[index] = SPR_addSpriteEx(&spinThing, 20, 20, TILE_ATTR(PAL3, FALSE, FALSE, FALSE),0,
                        SPR_FLAG_AUTO_VISIBILITY | SPR_FLAG_AUTO_SPRITE_ALLOC);
                    SPR_setVRAMTileIndex(sprites[index],0x646);
                    spriteSpawned[index] = TRUE;
                    spriteX[index] = FIX32(320);
                    spriteY[index] = FIX32(random()/274);
                    break;
                }
            }
        }


        for (index = 2; index < 40; index++){
            if (spriteSpawned[index] == TRUE){
                if (frame < 1120){
                    spriteX[index] -= FIX32(1);
                    SPR_setPosition (sprites[index], fix32ToInt(spriteX[index]), fix32ToInt(spriteY[index]));
                }else{
                    spriteX[index] -= FIX32(1);
                    spriteY[index] += spriteVel;
                    SPR_setPosition (sprites[index], fix32ToInt(spriteX[index]), fix32ToInt(spriteY[index]));
                }
                if (spriteX[index] < FIX32(-16)){
                    SPR_releaseSprite(sprites[index]);
                    spriteSpawned[index] = FALSE;
                }
            }
        }


        if (frame%128 < 64 && frame % 8 == 0){
            SPR_setFrame(sprites[0],7-((frame/8) % 8));
        } else if (frame%128 >= 64 && frame % 8 == 0){
            SPR_setFrame(sprites[0],(frame/8) % 8);
        }

        animWait -= 1;
        if (animWait == 0){
            animIndex += 1;
            animWait = shipPositions[animIndex-1];
        }
        //shipPosX = FIX32((fix32ToInt(fix32Mul(FIX32(frame),FIX32(1.25)))%320)-24);
        shipPosX = FIX32(8);
        if (frame < 1120){
            shipPosY = fix32Mul(sinFix32((frame*8)%1024),FIX32(50)) + FIX32(100);
        }else{
            shipVel += FIX32(0.05);
            spriteVel += FIX32(0.02);
            shipPosY += shipVel;
        }

        if (frame == 1120){
            for (u16 i = 0; i < 64; i++){
                VDP_setPaletteColor(i,0x0FFF);
            }
            VDP_fade(0, 63, palette_white, palette, 16, TRUE);
        }

        SPR_setPosition(sprites[0],fix32ToInt(shipPosX),fix32ToInt(shipPosY));
        SPR_update();

        VDP_waitVSync();
    }

    VDP_fadeOutAll(8,FALSE);

    SPR_end();

    SYS_disableInts();
    VDP_clearPlan(PLAN_A,TRUE);
    VDP_clearPlan(PLAN_B,TRUE);
    VDP_setHorizontalScroll(PLAN_A,0);
    VDP_setHorizontalScroll(PLAN_B,0);
    VDP_setVerticalScroll(PLAN_A,0);
    VDP_setVerticalScroll(PLAN_B,0);
    ind = TILE_USERINDEX;
    VDP_drawImageEx(PLAN_B, &satelliteBG, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
    ind += satelliteBG.tileset->numTile;
    VDP_drawImageEx(PLAN_A, &satelliteFG, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
    VDP_drawImageEx(PLAN_A, &satelliteFG, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 16, 0, FALSE, TRUE);
    VDP_drawImageEx(PLAN_A, &satelliteFG, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 0, 16, FALSE, TRUE);
    VDP_drawImageEx(PLAN_A, &satelliteFG, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 16, 16, FALSE, TRUE);
    SYS_enableInts();

    memcpy(&palette[0],satelliteBG.palette->data, 16*2);
    memcpy(&palette[16],satelliteFG.palette->data, 16*2);

    VDP_fadeIn(0, 63, palette, 8, FALSE);

    u16 frame;
    for (frame = 0; frame < 838; frame++){
        VDP_waitVSync();
        VDP_setHorizontalScroll(PLAN_A,frame/2);
        VDP_setHorizontalScroll(PLAN_B,(frame/4));
        VDP_setVerticalScroll(PLAN_A,frame/2);
        VDP_setVerticalScroll(PLAN_B,(frame/4));
    }
    otherOffset1 = frame/4;
    otherOffset2 = frame/2;

    SYS_disableInts();
    SYS_setHIntCallback(&HIntsCallback);
    VDP_setHInterrupt(TRUE);
    SYS_enableInts();
    for (frame = 0; frame < 40; frame++){
        if (frame % 4 == 0){
            otherOffset1 += 1;
        }
        if (frame % 2 == 0){
            otherOffset2 += 1;
        }
        HScrolls += 2;
        VDP_setVerticalScroll(PLAN_A,otherOffset2);
        VDP_setVerticalScroll(PLAN_B,otherOffset1);

        if (frame == 32){
            SYS_disableInts();
            VDP_fadeOutAll(8, TRUE);
            SYS_enableInts();
        } 
        VDP_waitVSync();
    }
    SYS_disableInts();
    VDP_setHInterrupt(FALSE);
    SYS_setHIntCallback(0);
    VDP_setScreenWidth320();
    VDP_setPlanSize(64,64);
    VDP_clearPlan(PLAN_A,TRUE);
    VDP_clearPlan(PLAN_B,TRUE);
    VDP_setHorizontalScroll(PLAN_A,0);
    VDP_setHorizontalScroll(PLAN_B,0);
    VDP_setVerticalScroll(PLAN_A,0);
    VDP_setVerticalScroll(PLAN_B,0);
    SYS_enableInts();
}
