#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define KIBAKO2_COLLECTIBLE_ID(thisx) ((thisx)->params & 0x3F)
#define KIBAKO2_COLLECTIBLE_FLAG(thisx) (((thisx)->params >> 0x8) & 0x7F)
#define KIBAKO2_SKULLTULA_SPAWN_PARAM(thisx) ((((thisx)->params & 0x1F) << 2) | 0xFF01)
#define KIBAKO2_CONTENTS(thisx) (((thisx)->params >> 0xF) & 1)

#define LOCATION_SKULL_TOKEN (0x060000 | (play->sceneId << 8) | (this->dyna.actor.params & 0x1F))

typedef enum {
    /* 0 */ OBJKIBAKO2_CONTENTS_COLLECTIBLE,
    /* 1 */ OBJKIBAKO2_CONTENTS_SKULLTULA
} ObjKibako2Contents;

struct ObjKibako2;

typedef void (*ObjKibako2ActionFunc)(struct ObjKibako2*, PlayState*);

typedef struct ObjKibako2 {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ ColliderCylinder collider;
    /* 0x1A8 */ ObjKibako2ActionFunc actionFunc;
    /* 0x1AC */ s8 unk_1AC;
    /* 0x1AD */ s8 skulltulaNoiseTimer;
} ObjKibako2; // size = 0x1B0

RECOMP_PATCH bool ObjKibako2_ContainsSkulltula(ObjKibako2* this, PlayState* play) {
    s32 actorSpawnParam = KIBAKO2_SKULLTULA_SPAWN_PARAM(&this->dyna.actor);
    s32 flag = -1;

    if (rando_location_is_checked(LOCATION_SKULL_TOKEN)) {
        return false;
    }

    if ((u16)actorSpawnParam & 3) {
        flag = ((actorSpawnParam & 0x3FC) >> 2) & 0xFF;
    }
    return !((flag >= 0) && Flags_GetTreasure(play, flag));
}