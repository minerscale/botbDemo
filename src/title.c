#include <genesis.h>
#include "title.h"
#include "resources.h"
#include "functions.h"
#include "part1.h"

void title()
{
    u16 palette[64];
    u16 ind;

    // Display Rob's logo!
    SYS_disableInts();
    VDP_setHorizontalScroll(PLAN_A,10);
    VDP_setVerticalScroll(PLAN_A,10);
    ind = TILE_USERINDEX;
    VDP_drawImageEx(PLAN_A, &blueROB1, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 8, 2, FALSE, TRUE);
    ind += blueROB1.tileset->numTile;
    VDP_drawImageEx(PLAN_A, &blueROB2, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ind), 15, 2, FALSE, TRUE);
    ind += blueROB2.tileset->numTile;
    VDP_drawImageEx(PLAN_A, &blueROB3, TILE_ATTR_FULL(PAL3, FALSE, FALSE, FALSE, ind), 21, 2, FALSE, TRUE);
    ind += blueROB3.tileset->numTile;
    SYS_enableInts();

    for (u16 i = 0; i < 56; i++){
        if (i + 17 < 64) VDP_setPaletteColor(i + 17, 0x0440);
        if (i + 15 < 64) VDP_setPaletteColor(i + 15, 0x0660);
        if (i + 14 < 64) VDP_setPaletteColor(i + 14, 0x0880);
        if (i + 13 < 64) VDP_setPaletteColor(i + 13, 0x0660);
        if (i + 11 < 64) VDP_setPaletteColor(i + 11, 0x0440);
        if (i + 10 < 64) VDP_setPaletteColor(i + 10, 0x0000);
        VDP_waitVSync();
    }
    VDP_fadeOutAll(8,FALSE);

    SYS_disableInts();
    VDP_setHorizontalScroll(PLAN_A,0);
    VDP_setVerticalScroll(PLAN_A,0);
    VDP_clearPlan(PLAN_A, TRUE);
    ind = TILE_USERINDEX;
    VDP_drawImageEx(PLAN_A, &ROB, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 5, 0, FALSE, TRUE);
    SYS_enableInts();
    memcpy(&palette[0], ROB.palette->data, 16 * 2);

    // Bar 1:
    VDP_fadeIn(0, 63, palette, 8, FALSE);
    sleep(112);
    VDP_fadeOutAll(8, FALSE);

    // Bar 2:
    // Display th4D34D's logo!
    SYS_disableInts();
    VDP_clearPlan(PLAN_A, TRUE);
    VDP_setVerticalScroll(PLAN_A,20);
    ind = TILE_USERINDEX;
    VDP_drawImageEx(PLAN_B, &th4D34D, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 5, 0, FALSE, TRUE);
    ind += th4D34D.tileset->numTile;
    VDP_drawImageEx(PLAN_A, &thugLife, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 16, 0, FALSE, TRUE);
    SYS_enableInts();
    memcpy(&palette[0], th4D34D.palette->data, 16 * 2);
    memcpy(&palette[16], thugLife.palette->data, 16*2);
    VDP_fadeIn(0, 63, palette, 8, FALSE);
    sleep(16);
    s16 sunglassesPos = 34;
    for (u16 i = 0; i < 48; i++){
        sunglassesPos -= 2;
        VDP_setVerticalScroll(PLAN_A,sunglassesPos);
        VDP_waitVSync();
    }
    sleep(48);
    VDP_fadeOutAll(8, FALSE);

    // Bar 3:
    // Display my logo!
    SYS_disableInts();
    VDP_clearPlan(PLAN_A, TRUE);
    VDP_clearPlan(PLAN_B, TRUE);
    VDP_setVerticalScroll(PLAN_A,0);
    ind = TILE_USERINDEX;
    VDP_drawImageEx(PLAN_A, &Minerscale, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
    SYS_enableInts();
    memcpy(&palette[0], Minerscale.palette->data, 16 * 2);
    VDP_fadeIn(0, 63, palette, 8, TRUE);
    for (u16 i = 0; i < 128; i++){
        VDP_setPaletteColor(7,noteColours[(i/8)%6]);
        if (i == 120) VDP_fadeOutAll(8, TRUE);
        VDP_waitVSync();
    }

    // Bar 4:
    // Display presents
    SYS_disableInts();
    VDP_clearPlan(PLAN_A, TRUE);
    ind = TILE_USERINDEX;
    VDP_drawImageEx(PLAN_A, &presents, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 2, 2, FALSE, TRUE);
    ind += presents.tileset->numTile;
    SYS_enableInts();
    memcpy(&palette[0], presents.palette->data, 16 * 2);
    VDP_fadeIn(0, 15, palette, 8, FALSE);
    sleep(18);

    // Bar 5:
    // Display Work
    memcpy(&palette[16], work.palette->data, 16 * 2);
    SYS_disableInts();
    VDP_drawImageEx(PLAN_A, &work, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 4, 8, FALSE, TRUE);
    ind += work.tileset->numTile;
    SYS_enableInts();
    VDP_fadeIn(16, 31, palette, 8, FALSE);
    sleep(22);

    // Display Without
    memcpy(&palette[32], without.palette->data, 16 * 2);
    SYS_disableInts();
    VDP_drawImageEx(PLAN_A, &without, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ind), 6, 14, FALSE, TRUE);
    ind += without.tileset->numTile;
    SYS_enableInts();
    VDP_fadeIn(32, 47, palette, 8, FALSE);
    sleep(28);

    // Bar 5 & 1/2
    // Display Rules
    memcpy(&palette[48], rules.palette->data, 16 * 2);
    SYS_disableInts();
    VDP_drawImageEx(PLAN_A, &rules, TILE_ATTR_FULL(PAL3, FALSE, FALSE, FALSE, ind), 8, 20, FALSE, TRUE);
    ind += rules.tileset->numTile;
    SYS_enableInts();
    VDP_fadeIn(48, 63, palette, 8, FALSE);
    sleep(48);

    VDP_fadeOutAll(8,FALSE);
    return;
}
