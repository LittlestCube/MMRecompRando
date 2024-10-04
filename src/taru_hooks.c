#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define OBJ_TARU_GET_100(thisx) ((thisx)->params & 0x100)
#define OBJ_TARU_GET_1F(thisx) ((thisx)->params & 0x1F)
#define OBJ_TARU_GET_3F(thisx) ((thisx)->params & 0x3F)
#define OBJ_TARU_GET_SWITCH_FLAG(thisx) ((thisx)->params & 0x7F)
#define OBJ_TARU_GET_7F00(thisx) (((thisx)->params >> 8) & 0x7F)
#define OBJ_TARU_GET_80(thisx) ((thisx)->params & 0x80)
#define OBJ_TARU_GET_8000(thisx) (((thisx)->params >> 0xF) & 1)

struct ObjTaru;

typedef void (*ObjTaruActionFunc)(struct ObjTaru*, PlayState*);

typedef struct ObjTaru {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ ColliderCylinder collider;
    /* 0x1A8 */ ObjTaruActionFunc actionFunc;
    /* 0x1AC */ s8 unk_1AC;
    /* 0x1AD */ s8 unk_1AD;
} ObjTaru; // size = 0x1B0

#define OBJ_TSUBO_P001F(thisx) ((thisx)->params & 0x1F)

#define ENSW_GETS_3(params) ((params & 3) & 0xFF)
#define ENSW_GET_3(thisx) (ENSW_GETS_3((thisx)->params))
#define ENSW_GETS_3FC(params) (((params & 0x3FC) >> 2) & 0xFF)

#define LOCATION_SKULL_TOKEN (0x060000 | (play->sceneId << 8) | OBJ_TSUBO_P001F(&this->dyna.actor))

RECOMP_PATCH bool func_80B9B6E0(ObjTaru* this, PlayState* play) {
    s32 chestFlag = -1;
    s32 skulltulaParams = (OBJ_TSUBO_P001F(&this->dyna.actor) * 4) | 0xFF01;

    if (rando_location_is_checked(LOCATION_SKULL_TOKEN)) {
        return false;
    }

    if (ENSW_GETS_3(skulltulaParams)) {
        chestFlag = ENSW_GETS_3FC(skulltulaParams);
    }

    return (chestFlag < 0) == true || !Flags_GetTreasure(play, chestFlag);
}