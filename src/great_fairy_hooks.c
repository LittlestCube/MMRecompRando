#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct EnDyExtra;

typedef void (*EnDyExtraActionFunc)(struct EnDyExtra*, PlayState*);

typedef struct EnDyExtra {
    /* 0x000 */ Actor actor;
    /* 0x144 */ EnDyExtraActionFunc actionFunc;
    /* 0x148 */ s16 type;
    /* 0x14A */ s16 trigger;
    /* 0x14C */ s16 timer;
    /* 0x150 */ f32 alphaScale;
    /* 0x154 */ Vec3f scale; // not used, leftover from OoT
    /* 0x160 */ Vec3f initPos; // set and not used
} EnDyExtra; // size = 0x16C

struct BgDyYoseizo;

typedef void (*BgDyYoseizoActionFunc)(struct BgDyYoseizo*, PlayState*);

#define GREAT_FAIRY_GET_SWITCHFLAG(thisx) (((thisx)->params & 0xFE00) >> 9)
#define GREAT_FAIRY_GET_TYPE(thisx) ((thisx)->params & 0xF)
#define GREAT_FAIRY_ROTZ(thisx) ((thisx)->home.rot.z)

#define GREAT_FAIRY_LIMB_MAX 0x1C

#define LOCATION_GREAT_FAIRY (0x030000 | GREAT_FAIRY_GET_TYPE(&this->actor))
#define LOCATION_GREAT_FAIRY_HUMAN GI_MASK_GREAT_FAIRY

typedef enum GreatFairyType {
    /* 0 */ GREAT_FAIRY_TYPE_MAGIC,
    /* 1 */ GREAT_FAIRY_TYPE_POWER,
    /* 2 */ GREAT_FAIRY_TYPE_WISDOM,
    /* 3 */ GREAT_FAIRY_TYPE_COURAGE,
    /* 4 */ GREAT_FAIRY_TYPE_KINDNESS
} GreatFairyType;

#define BG_DY_YOSEIZO_EFFECT_COUNT 200

typedef struct BgDyYoseizoEffect {
    /* 0x00 */ u8 alive; // drawn if 1, respawn if 0
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ Vec3f velocity;
    /* 0x1C */ Vec3f accel;
    /* 0x28 */ Color_RGB8 primColor;
    /* 0x2B */ Color_RGB8 envColor;
    /* 0x2E */ s16 alpha;
    /* 0x30 */ f32 scale;
    /* 0x34 */ s16 timer; // lifetime
    /* 0x36 */ s16 type; // 0 is general radiance, else is directed towards Player
    /* 0x38 */ s16 pitch;
    /* 0x3A */ s16 yaw;
    /* 0x3C */ s16 roll; // all three are f32 in OoT
} BgDyYoseizoEffect; // size = 0x40

typedef struct BgDyYoseizo {
    /* 0x0000 */ Actor actor;
    /* 0x0144 */ BgDyYoseizoActionFunc actionFunc;
    /* 0x0148 */ SkelAnime skelAnime;
    /* 0x018C */ Vec3s jointTable[GREAT_FAIRY_LIMB_MAX];
    /* 0x0234 */ Vec3s morphTable[GREAT_FAIRY_LIMB_MAX];
    /* 0x02DC */ Vec3s headRot;
    /* 0x02DC */ Vec3s torsoRot;
    /* 0x02E8 */ EnDyExtra* beam;
    /* 0x02EC */ f32 unk2EC; // unused
    /* 0x02F0 */ f32 unk2F0;
    /* 0x02F4 */ f32 unk2F4;
    /* 0x02F8 */ union {
        s16 unk2F8;
        s16 csId; // used on BgDyYoseizo_TrainPlayer
    };
    /* 0x02FA */ s16 eyeIndex;
    /* 0x02FC */ s16 mouthIndex;
    /* 0x02FE */ s16 blinkTimer;
    /* 0x0300 */ s16 timer;
    /* 0x0302 */ u16 unk302;
    /* 0x0304 */ BgDyYoseizoEffect effects[BG_DY_YOSEIZO_EFFECT_COUNT];
} BgDyYoseizo; // size = 0x3504

void BgDyYoseizo_Bob(BgDyYoseizo* this, PlayState* play);
s32 SkelAnime_Update(SkelAnime* skelAnime);
void func_80A0B290(BgDyYoseizo* this, PlayState* play);

struct EnElfgrp;

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((EnElfgrp*)thisx)

typedef void (*EnElfgrpActionFunc)(struct EnElfgrp*, PlayState*);

#define ENELFGRP_GET_TYPE(thisx) ((thisx)->params & 0xF) //!< Same type as Great Fairies
#define ENELFGRP_GET_SWITCH_FLAG_PARAMS(thisx) (((thisx)->params & 0xFE00) >> 9)
#define ENELFGRP_GET_SWITCHFLAG_ROT(thisx) ((thisx)->home.rot.z)

typedef enum ElfgrpType {
    /* 0 */ ENELFGRP_TYPE_MAGIC = GREAT_FAIRY_TYPE_MAGIC,
    /* 1 */ ENELFGRP_TYPE_POWER = GREAT_FAIRY_TYPE_POWER,
    /* 2 */ ENELFGRP_TYPE_WISDOM = GREAT_FAIRY_TYPE_WISDOM,
    /* 3 */ ENELFGRP_TYPE_COURAGE = GREAT_FAIRY_TYPE_COURAGE,
    /* 4 */ ENELFGRP_TYPE_KINDNESS = GREAT_FAIRY_TYPE_KINDNESS,
    /* 5 */ ENELFGRP_TYPE_MAX
} ElfgrpType;

typedef struct EnElfgrp {
    /* 0x000 */ Actor actor;
    /* 0x144 */ s16 timer;
    /* 0x146 */ u8 talkedOnceFlag;
    /* 0x147 */ u8 type;
    /* 0x148 */ s8 unk_148; // set and not used
    /* 0x14A */ u16 stateFlags;
    /* 0x14C */ EnElfgrpActionFunc actionFunc;
} EnElfgrp; // size = 0x150

// State flags
#define ELFGRP_STATE_0 (1 << 0)
#define ELFGRP_STATE_1 (1 << 1)
#define ELFGRP_STATE_2 (1 << 2)
#define ELFGRP_STATE_3 (1 << 3)
#define ELFGRP_STATE_4 (1 << 4)

// Used for the type in EnElfgrp_SpawnStrayFairies
typedef enum ElfgrpSpawnedFairyTypes {
    /* 0 */ SPAWNED_STRAY_FAIRY_TYPE_PRESENT,  // STRAY_FAIRY_TYPE_FAIRY_FOUNTAIN
    /* 1 */ SPAWNED_STRAY_FAIRY_TYPE_RETURNING // STRAY_FAIRY_TYPE_RETURNING_TO_FOUNTAIN
} ElfgrpSpawnedFairyTypes;

s32 EnElfgrp_GetHeldFairiesCount(PlayState* play, s32 type);
s32 EnElfgrp_GetFountainFairiesCount(PlayState* play, s32 type);
void EnElfgrp_SpawnStrayFairies(EnElfgrp* this, PlayState* play, s32 count, s32 fairyType);
void func_80A3A398(EnElfgrp* this, PlayState* play);
void func_80A3A520(EnElfgrp* this, PlayState* play);
void EnElfgrp_DoNothing(EnElfgrp* this, PlayState* play);
void func_80A3A7FC(EnElfgrp* this, PlayState* play);
void func_80A3A8F8(EnElfgrp* this, PlayState* play);

void EnElfgrp_SetCutscene(EnElfgrp* this, s32 numCutscenes);

RECOMP_PATCH void EnElfgrp_Init(Actor* thisx, PlayState* play) {
    s32 pad;
    EnElfgrp* this = THIS;
    s32 numberInFountain;

    this->type = ENELFGRP_GET_TYPE(&this->actor);
    this->unk_148 = 0;
    this->stateFlags = 0;
    this->actor.focus.pos.y += 40.0f;
    this->actor.flags &= ~ACTOR_FLAG_TARGETABLE;

    switch (this->type) {
        case ENELFGRP_TYPE_POWER:
        case ENELFGRP_TYPE_WISDOM:
        case ENELFGRP_TYPE_COURAGE:
        case ENELFGRP_TYPE_KINDNESS:
            this->unk_148 = this->type - 1;
            numberInFountain = EnElfgrp_GetFountainFairiesCount(play, this->type);
            this->talkedOnceFlag = 1 << this->type;

            if (numberInFountain < STRAY_FAIRY_TOTAL) {
                EnElfgrp_SpawnStrayFairies(this, play, numberInFountain, SPAWNED_STRAY_FAIRY_TYPE_PRESENT);
            }

            if (numberInFountain >= STRAY_FAIRY_TOTAL) {
                this->actionFunc = func_80A3A520;
                EnElfgrp_SetCutscene(this, 2);
                break;
            }

            if ((EnElfgrp_GetHeldFairiesCount(play, this->type) + numberInFountain) >= STRAY_FAIRY_TOTAL) {
                this->actionFunc = func_80A3A398;

                switch (this->type) {
                    case ENELFGRP_TYPE_POWER:
                        //if (CHECK_WEEKEVENTREG(WEEKEVENTREG_OBTAINED_GREAT_SPIN_ATTACK)) {
                        if (rando_location_is_checked(LOCATION_GREAT_FAIRY)) {
                            EnElfgrp_SetCutscene(this, 1);
                        } else {
                            this->stateFlags |= ELFGRP_STATE_2;
                        }
                        break;

                    case ENELFGRP_TYPE_WISDOM:
                        if (gSaveContext.save.saveInfo.playerData.isDoubleMagicAcquired == true) {
                            EnElfgrp_SetCutscene(this, 1);
                        }
                        break;

                    case ENELFGRP_TYPE_COURAGE:
                        if (gSaveContext.save.saveInfo.playerData.doubleDefense) {
                            EnElfgrp_SetCutscene(this, 1);
                        }
                        break;

                    case ENELFGRP_TYPE_KINDNESS:
                        if (INV_CONTENT(ITEM_SWORD_GREAT_FAIRY) == ITEM_SWORD_GREAT_FAIRY) {
                            EnElfgrp_SetCutscene(this, 1);
                        } else {
                            this->stateFlags |= ELFGRP_STATE_4;
                        }
                        break;

                    default:
                        break;
                }
            } else if (EnElfgrp_GetHeldFairiesCount(play, this->type)) {
                this->actionFunc = func_80A3A7FC;
                this->actor.textId = (this->type * 3) + 0x581;
            } else {
                this->actionFunc = func_80A3A8F8;

                if ((gSaveContext.save.saveInfo.weekEventReg[9] & this->talkedOnceFlag)) { // talked for first time
                    this->actor.textId = (this->type * 3) + 0x580;
                } else {
                    this->actor.textId = (this->type * 3) + 0x57F;
                }
            }
            break;

        default: // ENELFGRP_TYPE_MAGIC
            numberInFountain = EnElfgrp_GetFountainFairiesCount(play, ENELFGRP_TYPE_MAGIC);
            this->talkedOnceFlag = 1 << ENELFGRP_TYPE_MAGIC;

            if (numberInFountain >= STRAY_FAIRY_TOTAL) {
                this->actionFunc = func_80A3A520;

                if ((ENELFGRP_GET_SWITCHFLAG_ROT(&this->actor) != 0) &&
                    Flags_GetSwitch(play, ENELFGRP_GET_SWITCHFLAG_ROT(&this->actor))) {
                    this->actionFunc = EnElfgrp_DoNothing;
                } else if (rando_location_is_checked(LOCATION_GREAT_FAIRY_HUMAN)) {
                    EnElfgrp_SetCutscene(this, 4);
                } else if (INV_CONTENT(ITEM_MASK_DEKU) != ITEM_MASK_DEKU && INV_CONTENT(ITEM_MASK_GORON) != ITEM_MASK_GORON && INV_CONTENT(ITEM_MASK_ZORA) != ITEM_MASK_ZORA && INV_CONTENT(ITEM_MASK_FIERCE_DEITY) != ITEM_MASK_FIERCE_DEITY) {
                    EnElfgrp_SetCutscene(this, 5);
                } else {
                    this->stateFlags |= ELFGRP_STATE_1;
                    EnElfgrp_SetCutscene(this, 6);
                }
            } else if (CHECK_WEEKEVENTREG(WEEKEVENTREG_08_80)) {
                EnElfgrp_SpawnStrayFairies(this, play, STRAY_FAIRY_TOTAL - 1, SPAWNED_STRAY_FAIRY_TYPE_PRESENT);
                this->actionFunc = func_80A3A398;

                if (INV_CONTENT(ITEM_MASK_DEKU) == ITEM_MASK_DEKU || INV_CONTENT(ITEM_MASK_GORON) == ITEM_MASK_GORON || INV_CONTENT(ITEM_MASK_ZORA) == ITEM_MASK_ZORA || INV_CONTENT(ITEM_MASK_FIERCE_DEITY) == ITEM_MASK_FIERCE_DEITY) {
                    if (rando_location_is_checked(LOCATION_GREAT_FAIRY_HUMAN)) {
                        EnElfgrp_SetCutscene(this, 2);
                    } else {
                        EnElfgrp_SetCutscene(this, 3);
                        this->stateFlags |= ELFGRP_STATE_1;
                    }
                } else if (rando_location_is_checked(LOCATION_GREAT_FAIRY)) {
                    EnElfgrp_SetCutscene(this, 1);
                }
            } else {
                EnElfgrp_SpawnStrayFairies(this, play, STRAY_FAIRY_TOTAL - 1, SPAWNED_STRAY_FAIRY_TYPE_PRESENT);
                this->actionFunc = func_80A3A8F8;
                if ((gSaveContext.save.saveInfo.weekEventReg[9] & this->talkedOnceFlag)) {
                    this->actor.textId = 0x580;
                } else {
                    this->actor.textId = 0x578;
                }
                this->actor.flags |= (ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_FRIENDLY);
            }
            break;
    }
}

RECOMP_PATCH void func_80A0B35C(BgDyYoseizo* this, PlayState* play) {
    BgDyYoseizo_Bob(this, play);
    SkelAnime_Update(&this->skelAnime);

    if (this->timer == 60) {
        if (!Flags_GetSwitch(play, GREAT_FAIRY_GET_SWITCHFLAG(&this->actor))) {
            rando_send_location(LOCATION_GREAT_FAIRY);
            switch (GREAT_FAIRY_GET_TYPE(&this->actor)) {
                case GREAT_FAIRY_TYPE_MAGIC:
                    break;

                case GREAT_FAIRY_TYPE_WISDOM:
                    if (gSaveContext.save.saveInfo.playerData.isDoubleMagicAcquired != true) {
                        gSaveContext.save.saveInfo.playerData.isDoubleMagicAcquired = true;
                        gSaveContext.magicFillTarget = MAGIC_DOUBLE_METER;
                        gSaveContext.save.saveInfo.playerData.magicLevel = 0;
                    }
                    break;

                case GREAT_FAIRY_TYPE_COURAGE:
                    if (gSaveContext.save.saveInfo.playerData.doubleDefense != true) {
                        gSaveContext.save.saveInfo.playerData.doubleDefense = true;
                    }
                    break;

                default:
                    break;
            }
        }
        Interface_SetHudVisibility(9);
    }

    if ((this->timer < 50) && (GREAT_FAIRY_GET_TYPE(&this->actor) == GREAT_FAIRY_TYPE_COURAGE)) {
        if (gSaveContext.save.saveInfo.inventory.defenseHearts < 20) {
            gSaveContext.save.saveInfo.inventory.defenseHearts++;
        }
    }

    if (this->timer == 50) {
        gSaveContext.healthAccumulator = 0x140;
        Magic_Add(play, MAGIC_FILL_TO_CAPACITY);
    }

    if (this->timer == 0) {
        this->beam->trigger = true;
        this->actionFunc = func_80A0B290;
    }
}

void func_80A3A274(EnElfgrp* this, PlayState* play);

RECOMP_PATCH void func_80A3A398(EnElfgrp* this, PlayState* play) {
    if (CutsceneManager_IsNext(this->actor.csId)) {
        CutsceneManager_StartWithPlayerCs(this->actor.csId, &this->actor);
        this->actionFunc = func_80A3A274;
        Flags_UnsetSwitch(play, ENELFGRP_GET_SWITCH_FLAG_PARAMS(&this->actor));

        if (this->stateFlags & ELFGRP_STATE_1) {
            Item_Give(play, ITEM_MASK_GREAT_FAIRY);
        }

        if (this->stateFlags & ELFGRP_STATE_2) {
            //SET_WEEKEVENTREG(WEEKEVENTREG_OBTAINED_GREAT_SPIN_ATTACK);
        }

        if (this->stateFlags & ELFGRP_STATE_4) {
            Item_Give(play, ITEM_SWORD_GREAT_FAIRY);
        }

        this->stateFlags &= ~ELFGRP_STATE_3;
    } else if (this->actor.xzDistToPlayer < 350.0f) {
        CutsceneManager_Queue(this->actor.csId);
    }
}

void func_80A3A4AC(EnElfgrp* this, PlayState* play);

RECOMP_PATCH void func_80A3A520(EnElfgrp* this, PlayState* play) {
    if (Cutscene_IsCueInChannel(play, CS_CMD_ACTOR_CUE_103)) {
        this->actionFunc = EnElfgrp_DoNothing;
    } else if (CutsceneManager_IsNext(this->actor.csId)) {
        CutsceneManager_StartWithPlayerCs(this->actor.csId, &this->actor);
        this->actionFunc = func_80A3A4AC;
        Flags_SetSwitch(play, ENELFGRP_GET_SWITCH_FLAG_PARAMS(&this->actor));

        if (this->stateFlags & ELFGRP_STATE_1) {
            Item_Give(play, ITEM_MASK_GREAT_FAIRY);
        }

        if (ENELFGRP_GET_SWITCHFLAG_ROT(&this->actor) != 0) {
            Flags_SetSwitch(play, ENELFGRP_GET_SWITCHFLAG_ROT(&this->actor));
        }
    } else if (this->actor.xzDistToPlayer < 350.0f) {
        CutsceneManager_Queue(this->actor.csId);
    }
}