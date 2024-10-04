#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct BgIngate;

#define FLAGS (ACTOR_FLAG_10 | ACTOR_FLAG_20)

#define THIS ((BgIngate*)thisx)

typedef void (*BgIngateActionFunc)(struct BgIngate*, PlayState*);

#define BGINGATE_GET_PATH_INDEX(thisx) ((thisx)->params & 0xFF)

typedef struct BgIngate {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ BgIngateActionFunc actionFunc;
    /* 0x160 */ u16 unk160;
    /* 0x164 */ Path* timePath;
    /* 0x168 */ s16 timePathTimeSpeed;
    /* 0x16A */ s16 unk16A;
    /* 0x16C */ s16 unk16C;
    /* 0x16E */ s16 csId;
    /* 0x170 */ Vec3f timePathTargetPos;
    /* 0x17C */ f32 timePathProgress;
    /* 0x180 */ s32 timePathTotalTime;
    /* 0x184 */ s32 timePathWaypointTime;
    /* 0x188 */ s32 timePathWaypoint;
    /* 0x18C */ s32 timePathElapsedTime;
} BgIngate; // size = 0x190

RECOMP_PATCH void BgIngate_Update(Actor* thisx, PlayState* play) {
    BgIngate* this = THIS;

    if (this->timePathElapsedTime > 30 && !CHECK_EVENTINF(EVENTINF_35)) {
        this->timePathTotalTime = 4 * 100;
        this->timePathTimeSpeed = 4;
        this->timePathWaypointTime = this->timePathTotalTime / (this->timePath->count - (SUBS_TIME_PATHING_ORDER - 1));
    }

    this->actionFunc(this, play);
}

RECOMP_PATCH void func_80953B40(BgIngate* this) {
    s32 temp;

    if (!CHECK_EVENTINF(EVENTINF_35)) {
        this->timePathTotalTime = 4 * 1000;
        this->timePathTimeSpeed = 4;
    } else {
        this->timePathTotalTime = 1 * 2000;
        this->timePathTimeSpeed = 1;
    }
    temp = this->timePath->count - (SUBS_TIME_PATHING_ORDER - 1);
    this->timePathWaypointTime = this->timePathTotalTime / temp;
    this->timePathWaypoint = SUBS_TIME_PATHING_ORDER - 1;
    this->timePathElapsedTime = 0;
    this->unk160 &= ~0x1;
    this->unk160 &= ~0x2;
}