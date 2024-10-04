#include "modding.h"
#include "global.h"

#include "apcommon.h"

bool justDied = false;

RECOMP_CALLBACK("*", recomp_after_moon_crash)
void moon_crash_triggered(SramContext* sramCtx) {
    justDied = true;
}