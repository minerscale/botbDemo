#include <genesis.h>
#include "resources.h"
#include "functions.h"

s8 credits[48][40] = {
    " Puke7 (For making botb! ALL N00BZ! :D/)",
    "     Stef (For making SGDK and XGM.)",
    " Minerscale (For making this cool demo!)",
    "     Th4D34D (For making the music!)",
    "     Rob (For making the music too!)",
    "    Tobikomi (For Minerscale's logo!)",
    "  Delta Razero (For the cool Rob logo)!",
    "      Delek (For making Deflemask.)",
    "  Marcb0t (For being a good freind ^_^)",
    "                    ",
    "  All the participants of FM Synth 2 :)",
    "                    ",
    "  And some people I like so the credits",
    "  Can end at the same time as the song.",
    "        In no particualar order:",
    "                  Jredd",
    "                Tennisers",
    "                djmaximum",
    "              MovieMovies1",
    "                TomoAlien",
    "              pigdevil2010",
    "               BiscuitExe",
    "             raphaelgoulart",
    "                 MiDoRi",
    "                xracercar",
    "               mootbooxle",
    "            TheOfficialLobst",
    "                 DaJoshy",
    "                  Zlew",
    "                   mk7",
    "                 chunter",
    "                  TiTAN",
    "                 Strobe",
    "                ViLXDRYAD",
    "                 Sinc-X",
    "                  Robyn",
    "                Ordinate",
    "              Blaze Weednix",
    "                 b00daw",
    "                 Apsarah",
    "                Quirby64",
    "                Savestate",
    "                Dimeback",
    "               sleeparrow",
    "                   m9m",
    "                 Dropbit",
    "                Catskull",
};

void part3(){
    u16 palette[64];
    u16 ind;

    // Display Part 3
    SYS_disableInts();
    ind = TILE_USERINDEX;
    VDP_drawImageEx(PLAN_A, &part, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 7, 12, FALSE, TRUE);
    ind += part.tileset->numTile;
    VDP_drawImageEx(PLAN_A, &IIIimage, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 29, 12, FALSE, TRUE);
    SYS_enableInts();
    memcpy(&palette[0], part.palette->data, 16 * 2);
    VDP_fadeIn(0, 63, palette, 8, FALSE);
    sleep(96);
    VDP_fadeOutAll(16, FALSE);

    // Display thanking people:
    SYS_disableInts();
    VDP_clearPlan(PLAN_A, TRUE);
    ind = TILE_USERINDEX;
    VDP_drawImageEx(PLAN_A, &thanks, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 0, 10, FALSE, TRUE);
    VDP_loadFont(&font_lib, TRUE);
    SYS_enableInts();
    memcpy(&palette[0],thanks.palette->data, 16*2);
    VDP_fadeIn(0, 63, palette, 8, FALSE);
    sleep(112);
    VDP_fadeOutAll(8,FALSE);
    VDP_clearPlan(PLAN_A, TRUE);

    
    memcpy(&palette[0],palette_grey, 16*2);
    VDP_fadeIn(0, 63, palette, 8, FALSE);
    for (u16 frame; frame < 1536; frame ++){
        if (frame % 32 == 0){
            VDP_clearText(0, ((frame/32) + 30) % 64, 40);
            VDP_drawText(credits[frame/32], 0,((frame/32) + 30) % 64);
        }
        VDP_setVerticalScroll(PLAN_A, frame/4);
        VDP_waitVSync();
    }
    VDP_fadeOutAll(32,FALSE);

    // Display botb
    SYS_disableInts();
    VDP_setHorizontalScroll(PLAN_A,0);
    VDP_setVerticalScroll(PLAN_A,0);
    VDP_clearPlan(PLAN_A, TRUE);
    ind = TILE_USERINDEX;
    VDP_drawImageEx(PLAN_A, &botb, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 0, 12, FALSE, TRUE);
    SYS_enableInts();
    memcpy(&palette[0],botb.palette->data, 16*2);
    VDP_fadeIn(0, 63, palette, 64, FALSE);
}
