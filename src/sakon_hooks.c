#include "modding.h"
#include "global.h"
#include "overlays/actors/ovl_En_GirlA/z_en_girla.h"

#include "apcommon.h"

#define ENFSN_IS_BACKROOM(thisx) ((thisx)->params & 1)
#define ENFSN_IS_SHOP(thisx) (!((thisx)->params & 1))

#define ENFSN_OFFER_FINAL_PRICE (1 << 6)
#define ENFSN_HAGGLE (1 << 7)
#define ENFSN_ANGRY (1 << 8)
#define ENFSN_CALM_DOWN (1 << 9)

#define FSN_LIMB_MAX 0x12
#define ENFSN_LIMB_MAX FSN_LIMB_MAX + 1 // Note: adding 1 to FSN_LIMB_MAX due to bug in the skeleton, see bug in object_fsn.xml

struct EnFsn;

typedef void (*EnFsnActionFunc)(struct EnFsn*, PlayState*);

typedef struct EnFsn {
    /* 0x000 */ Actor actor;
    /* 0x144 */ UNK_TYPE1 pad144[0x4C];
    /* 0x190 */ SkelAnime skelAnime;
    /* 0x1D4 */ EnFsnActionFunc actionFunc;
    /* 0x1D8 */ EnFsnActionFunc prevActionFunc; // Used to return to correct browsing function
    /* 0x1DC */ ColliderCylinder collider;
    /* 0x228 */ s16 fidgetTableY[ENFSN_LIMB_MAX];
    /* 0x24E */ s16 fidgetTableZ[ENFSN_LIMB_MAX];
    /* 0x274 */ Vec3s headRot;
    /* 0x27A */ Vec3s torsoRot; // Set but never used
    /* 0x280 */ Vec3s jointTable[ENFSN_LIMB_MAX];
    /* 0x2F2 */ Vec3s morphTable[ENFSN_LIMB_MAX];
    /* 0x364 */ s16 eyeTexIndex;
    /* 0x366 */ s16 blinkTimer;
    /* 0x368 */ s16 cutsceneState;
    /* 0x36A */ s16 csId;
    /* 0x36C */ s16 lookToShopkeeperCsId;
    /* 0x36E */ s16 lookToShelfCsId;
    /* 0x370 */ s16 lookToShopkeeperFromShelfCsId;
    /* 0x372 */ s16 lookToShopkeeperBuyingCsId;
    /* 0x374 */ s16 price;
    /* 0x376 */ u16 textId;
    /* 0x378 */ u8 isSelling;
    /* 0x379 */ u8 cursorIndex;
    /* 0x37C */ s32 getItemId;
    /* 0x380 */ s16 stolenItem1;
    /* 0x382 */ s16 stolenItem2;
    /* 0x384 */ s16 itemIds[3];
    /* 0x38A */ s16 totalSellingItems;
    /* 0x38C */ s16 numSellingItems;
    /* 0x390 */ EnGirlA* items[3];
    /* 0x39C */ s16 delayTimer;
    /* 0x3A0 */ s32 stickAccumX;
    /* 0x3A4 */ s32 stickAccumY;
    /* 0x3A8 */ Vec3f cursorPos;
    /* 0x3B4 */ Color_RGBAu32 cursorColor;
    /* 0x3C4 */ f32 cursorAnimTween;
    /* 0x3C8 */ u8 cursorAnimState;
    /* 0x3C9 */ u8 drawCursor;
    /* 0x3CC */ StickDirectionPrompt stickLeftPrompt;
    /* 0x404 */ StickDirectionPrompt stickRightPrompt;
    /* 0x43C */ f32 arrowAnimTween;
    /* 0x440 */ f32 stickAnimTween;
    /* 0x444 */ u8 arrowAnimState;
    /* 0x445 */ u8 stickAnimState;
    /* 0x448 */ f32 shopItemSelectedTween;
    /* 0x44C */ s16 animIndex;
    /* 0x44E */ u16 flags;
} EnFsn; // size = 0x450

struct EnSuttari;

typedef void (*EnSuttariActionFunc)(struct EnSuttari*, PlayState*);

#define ENSUTTARI_GET_PATH_INDEX(thisx) (((thisx)->params & 0x7E00) >> 9)

#define ENSUTTARI_PATH_INDEX_NONE 0x3F

#define OBJECT_BOJ_LIMB_MAX 0x10

typedef struct EnSuttari {
    /* 0x000 */ Actor actor;
    /* 0x144 */ EnFsn* enFsn;
    /* 0x148 */ EnSuttariActionFunc actionFunc;
    /* 0x14C */ UNK_TYPE1 unk_14C[0x4];
    /* 0x150 */ SkelAnime skelAnime;
    /* 0x194 */ UNK_TYPE1 unk_194[0x4];
    /* 0x198 */ ColliderCylinder collider;
    /* 0x1E4 */ u16 flags1;
    /* 0x1E6 */ u16 flags2;
    /* 0x1E8 */ u16 textId;
    /* 0x1EA */ UNK_TYPE1 unk_1EA[0x2];
    /* 0x1EC */ Path* paths[2];
    /* 0x1F4 */ s32 unk1F4[2];
    /* 0x1FC */ UNK_TYPE1 unk_1FC[0x1A];
    /* 0x216 */ Vec3s jointTable[OBJECT_BOJ_LIMB_MAX];
    /* 0x276 */ Vec3s morphTable[OBJECT_BOJ_LIMB_MAX];
    /* 0x2D6 */ Vec3s trackTarget;
    /* 0x2DC */ Vec3s headRot;
    /* 0x2E2 */ Vec3s torsoRot;
    /* 0x2E8 */ UNK_TYPE1 unk_2E8[0x12];
    /* 0x2FA */ s16 fidgetTableY[OBJECT_BOJ_LIMB_MAX];
    /* 0x31A */ s16 fidgetTableZ[OBJECT_BOJ_LIMB_MAX];
    /* 0x33A */ UNK_TYPE1 unk_33A[0xB6];
    /* 0x3F0 */ s16 playerDetected;
    /* 0x3F2 */ s16 unk3F2;
    /* 0x3F4 */ s16 unk3F4;
    /* 0x3F6 */ s16 unk3F6;
    /* 0x3F8 */ Vec3f unk3F8;
    /* 0x404 */ Path* timePath;
    /* 0x408 */ Vec3f timePathTargetPos;
    /* 0x414 */ f32 timePathProgress;
    /* 0x418 */ s32 timePathTotalTime;
    /* 0x41C */ s32 timePathWaypointTime;
    /* 0x420 */ s32 timePathWaypoint;
    /* 0x424 */ s32 timePathElapsedTime;
    /* 0x428 */ u8 scheduleResult;
    /* 0x42A */ s16 timePathTimeSpeed;
    /* 0x42C */ s32 unk42C;
    /* 0x430 */ s32 unk430;
    /* 0x434 */ s16 unk434;
    /* 0x436 */ s16 unk436;
    /* 0x438 */ Vec3f unk438;
    /* 0x444 */ Vec3f unk444;
    /* 0x450 */ s32 animIndex;
    /* 0x454 */ UNK_TYPE1 unk_454[0x2];
    /* 0x456 */ s16 csIdList[2];
    /* 0x45A */ s16 csIdIndex;
} EnSuttari; // size = 0x45C

void EnSuttari_TriggerTransition(PlayState* play, u16 entrance);

void func_80BADE14(EnSuttari* this, PlayState* play);

RECOMP_PATCH void EnSuttari_UpdateCollider(EnSuttari* this, PlayState* play) {
    this->collider.dim.pos.x = this->actor.world.pos.x;
    this->collider.dim.pos.y = this->actor.world.pos.y;
    this->collider.dim.pos.z = this->actor.world.pos.z;

    if (this->flags1 & 0x40) {
        if (this->collider.base.acFlags & AC_HIT) {
            this->collider.base.acFlags &= ~AC_HIT;
            if (this->actor.colChkInfo.damageEffect == 0xF) {
                this->flags1 |= 0x100;
                this->flags1 &= ~0x40;
                SET_WEEKEVENTREG(WEEKEVENTREG_RECOVERED_STOLEN_BOMB_BAG);
                Enemy_StartFinishingBlow(play, &this->actor);
                EnSuttari_TriggerTransition(play, ENTRANCE(NORTH_CLOCK_TOWN, 7));
            } else if (this->actor.colChkInfo.damageEffect == 0xE) {
                this->flags1 |= 0x200;
                this->flags1 &= ~0x40;
            }
        }
    }

    CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);

    if ((this->flags1 & 1) && (this->actionFunc != func_80BADE14)) {
        Actor_UpdateBgCheckInfo(play, &this->actor, 35.0f, 30.0f, 30.0f, UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_4);
    } else {
        Actor_UpdateBgCheckInfo(play, &this->actor, 35.0f, 30.0f, 30.0f, UPDBGCHECKINFO_FLAG_4);
    }
}