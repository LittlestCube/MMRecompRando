#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct ObjMakekinsuta;

#define OBJMAKEKINSUTA_GET_1F(thisx) (((thisx)->params >> 8) & 0x1F)
#define OBJMAKEKINSUTA_GETS_3(params) ((params & 3) & 0xFF)
#define OBJMAKEKINSUTA_GETS_3FC(params) (((params & 0x3FC) >> 2) & 0xFF)
#define OBJMAKEKINSUTA_GET_SWITCH_FLAG(thisx) ((thisx)->params & 0x7F)

#define LOCATION_SKULL_TOKEN (0x060000 | (play->sceneId << 8) | OBJMAKEKINSUTA_GET_1F(&this->actor))

typedef struct ObjMakekinsuta {
    /* 0x000 */ Actor actor;
    /* 0x144 */ s8 unk144;
} ObjMakekinsuta; // size = 0x148

RECOMP_PATCH bool func_8099FA40(ObjMakekinsuta* this, PlayState* play) {
    s32 chestFlag = -1;
    s32 skulltulaParams = (OBJMAKEKINSUTA_GET_1F(&this->actor) << 2) | 0xFF01;

    if (rando_location_is_checked(LOCATION_SKULL_TOKEN)) {
        return false;
    }

    if (OBJMAKEKINSUTA_GETS_3(skulltulaParams)) {
        chestFlag = OBJMAKEKINSUTA_GETS_3FC(skulltulaParams);
    }
    return (chestFlag < 0) == true || !Flags_GetTreasure(play, chestFlag);
}