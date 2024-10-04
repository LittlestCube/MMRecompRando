#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct EnSellnuts;

#define FLAGS (ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_10 | ACTOR_FLAG_20)

#define THIS ((EnSellnuts*)thisx)

#define ENSELLNUTS_GET_1(thisx) ((thisx)->params & 1)
#define ENSELLNUTS_GET_PATH_INDEX(thisx) (((thisx)->params & 0xFC00) >> 0xA)

#define ENSELLNUTS_PATH_INDEX_NONE 0x3F

typedef void (*EnSellnutsActionFunc)(struct EnSellnuts*, PlayState*);

typedef struct EnSellnuts {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[28];
    /* 0x230 */ Vec3s morphTable[28];
    /* 0x2D8 */ EnSellnutsActionFunc actionFunc;
    /* 0x2DC */ ColliderCylinder collider;
    /* 0x328 */ s16 unk_328;
    /* 0x32C */ f32 unk_32C;
    /* 0x330 */ Path* path;
    /* 0x334 */ s32 unk_334;
    /* 0x338 */ u16 unk_338;
    /* 0x33A */ u16 unk_33A;
    /* 0x33C */ s16 csId;
    /* 0x33E */ UNK_TYPE1 unk33E[0x2];
    /* 0x340 */ u16 unk_340;
    /* 0x342 */ u16 unk_342;
    /* 0x344 */ s16 unk_344;
    /* 0x346 */ UNK_TYPE1 unk346[0x4];
    /* 0x34A */ s16 unk_34A;
    /* 0x34C */ s16 unk_34C;
    /* 0x34E */ s16 unk_34E;
    /* 0x350 */ s16 unk_350;
    /* 0x354 */ f32 unk_354;
    /* 0x358 */ f32 unk_358;
    /* 0x35C */ f32 unk_35C;
    /* 0x360 */ f32 unk_360;
    /* 0x364 */ s16 unk_364;
    /* 0x366 */ s16 unk_366;
    /* 0x368 */ s16 unk_368;
    /* 0x36A */ UNK_TYPE1 unk36A[0x2];
    /* 0x36C */ f32 unk_36C;
    /* 0x370 */ f32 unk_370;
    /* 0x374 */ f32 unk_374;
} EnSellnuts; // size = 0x378

bool given = false;

void func_80ADBCE4(EnSellnuts* this, PlayState* play);

RECOMP_PATCH void func_80ADBBEC(EnSellnuts* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        SET_WEEKEVENTREG(WEEKEVENTREG_17_80);
        this->actionFunc = func_80ADBCE4;
    } else {
        Actor_OfferGetItem(&this->actor, play, GI_DEED_LAND, 300.0f, 300.0f);
    }
}