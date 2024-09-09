#include "modding.h"
#include "global.h"

RECOMP_IMPORT("*", int recomp_printf(const char* fmt, ...));

// Patches a function in the base game that's used to check if the player should quickspin.
RECOMP_PATCH s32 func_808333CC(Player* this) {
    recomp_printf("Spin attacking\n");
    // Always spin attack.
    return true;
}
