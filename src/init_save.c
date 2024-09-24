#include "modding.h"
#include "z64save.h"
#include "z64horse.h"
#include "overlays/gamestates/ovl_file_choose/z_file_select.h"

#include "apcommon.h"

bool saveOpened = false;

RECOMP_CALLBACK("*", recomp_on_load_save)
void rando_on_load_save(FileSelectState* fileSelect, SramContext* sramCtx) {
    saveOpened = true;
}

extern SavePlayerData sSaveDefaultPlayerData;
extern ItemEquips sSaveDefaultItemEquips;
extern Inventory sSaveDefaultInventory;

extern Inventory sSaveDebugInventory;

void Sram_ClearHighscores(void);
void Sram_GenerateRandomSaveFields(void);
void Sram_ResetSave(void);

void Sram_SetInitialWeekEvents(void) {
    SET_WEEKEVENTREG(WEEKEVENTREG_15_20);
    SET_WEEKEVENTREG(WEEKEVENTREG_59_04);
    SET_WEEKEVENTREG(WEEKEVENTREG_31_04);

    // Guards let you out of Clock Town
    SET_WEEKEVENTREG(WEEKEVENTREG_12_20);

    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_EAST_CLOCK_TOWN);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_WEST_CLOCK_TOWN);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_NORTH_CLOCK_TOWN);

    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_TERMINA_FIELD);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_IKANA_GRAVEYARD);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_ROMANI_RANCH);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_GORMAN_TRACK);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_MOUNTAIN_VILLAGE_WINTER);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_GORON_SHRINE);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_SNOWHEAD);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_SOUTHERN_SWAMP_POISONED);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_WOODFALL);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_DEKU_PALACE);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_GREAT_BAY_COAST);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_PIRATES_FORTRESS);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_ZORA_HALL);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_WATERFALL_RAPIDS);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_IKANA_CANYON);
    // Attached to the scene but unused. Entrance cutscene is instead triggered by `ACTOR_OBJ_DEMO`
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_IKANA_CASTLE);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_STONE_TOWER);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_STONE_TOWER_INVERTED);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_EAST_CLOCK_TOWN);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_WEST_CLOCK_TOWN);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_NORTH_CLOCK_TOWN);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_WOODFALL_TEMPLE);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_SNOWHEAD_TEMPLE);
    // Attached to the scene but unused. Entrance cutscene is instead triggered by `ACTOR_OBJ_DEMO`
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_PIRATES_FORTRESS_EXTERIOR);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_STONE_TOWER_TEMPLE);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_STONE_TOWER_TEMPLE_INVERTED);
    // Unused as no cutscene is attached to this script
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_THE_MOON);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_MOON_DEKU_TRIAL);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_MOON_GORON_TRIAL);
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_MOON_ZORA_TRIAL);

    // skippable mask transformations
    SET_WEEKEVENTREG(PACK_WEEKEVENTREG_FLAG(30, 0x10));
    SET_WEEKEVENTREG(PACK_WEEKEVENTREG_FLAG(30, 0x20));
    SET_WEEKEVENTREG(PACK_WEEKEVENTREG_FLAG(30, 0x40));
    SET_WEEKEVENTREG(PACK_WEEKEVENTREG_FLAG(30, 0x80));

    // moon's tear deku scrub starts out in flower
    SET_WEEKEVENTREG(PACK_WEEKEVENTREG_FLAG(73, 0x04));

    // skip the princess prison cutscene
    SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_WOODFALL_TEMPLE_PRISON);
}

void Sram_InitDebugSave(void) {
    PlayState* play = gPlay;
    Sram_ResetSave();

    Lib_MemCpy(&gSaveContext.save.saveInfo.playerData, &sSaveDefaultPlayerData, sizeof(SavePlayerData));
    Lib_MemCpy(&gSaveContext.save.saveInfo.equips, &sSaveDefaultItemEquips, sizeof(ItemEquips));
    Lib_MemCpy(&gSaveContext.save.saveInfo.inventory, &sSaveDefaultInventory, sizeof(Inventory));
    gSaveContext.save.saveInfo.checksum = 0;
    
    gSaveContext.save.playerForm = PLAYER_FORM_HUMAN;

    gSaveContext.save.hasTatl = true;

    gSaveContext.save.saveInfo.horseData.sceneId = SCENE_F01;
    gSaveContext.save.saveInfo.horseData.pos.x = -1420;
    gSaveContext.save.saveInfo.horseData.pos.y = 257;
    gSaveContext.save.saveInfo.horseData.pos.z = -1285;
    gSaveContext.save.saveInfo.horseData.yaw = -0x7554;

    gSaveContext.save.isFirstCycle = false;

    Sram_SetInitialWeekEvents();

    // skip the *insanely long* skull kid tatl/tael backstory cutscene
    u8* save_ptr = (u8*) &gSaveContext;
    save_ptr[0x5EB] |= 0x10;
    save_ptr[0x42F3] |= 0x10;

    gSaveContext.cycleSceneFlags[SCENE_INSIDETOWER].switch0 = 1;
    gSaveContext.save.saveInfo.permanentSceneFlags[SCENE_INSIDETOWER].switch0 = 1;

    gSaveContext.save.saveInfo.playerData.healthCapacity = 0x60;
    gSaveContext.save.saveInfo.playerData.health = 0x60;

    Sram_GenerateRandomSaveFields();

    gSaveContext.save.saveInfo.playerData.threeDayResetCount = 1;
}

void Sram_InitSave(FileSelectState* fileSelect2, SramContext* sramCtx) {
    s32 phi_v0;
    u16 i;
    FileSelectState* fileSelect = fileSelect2;
    s16 maskCount;

    if (gSaveContext.flashSaveAvailable) {
        Sram_InitDebugSave();
        if (fileSelect->buttonIndex == 0) {
            //gSaveContext.save.cutsceneIndex = 0xFFF0;
        }

        for (phi_v0 = 0; phi_v0 < ARRAY_COUNT(gSaveContext.save.saveInfo.playerData.playerName); phi_v0++) {
            gSaveContext.save.saveInfo.playerData.playerName[phi_v0] =
                fileSelect->fileNames[fileSelect->buttonIndex][phi_v0];
        }

        gSaveContext.save.saveInfo.playerData.newf[0] = 'Z';
        gSaveContext.save.saveInfo.playerData.newf[1] = 'E';
        gSaveContext.save.saveInfo.playerData.newf[2] = 'L';
        gSaveContext.save.saveInfo.playerData.newf[3] = 'D';
        gSaveContext.save.saveInfo.playerData.newf[4] = 'A';
        gSaveContext.save.saveInfo.playerData.newf[5] = '3';

        gSaveContext.save.saveInfo.checksum = Sram_CalcChecksum(&gSaveContext.save, sizeof(Save));

        Lib_MemCpy(sramCtx->saveBuf, &gSaveContext.save, sizeof(Save));
        Lib_MemCpy(&sramCtx->saveBuf[0x2000], &gSaveContext.save, sizeof(Save));

        for (i = 0; i < ARRAY_COUNT(gSaveContext.save.saveInfo.playerData.newf); i++) {
            fileSelect->newf[fileSelect->buttonIndex][i] = gSaveContext.save.saveInfo.playerData.newf[i];
        }

        fileSelect->threeDayResetCount[fileSelect->buttonIndex] =
            gSaveContext.save.saveInfo.playerData.threeDayResetCount;

        for (i = 0; i < ARRAY_COUNT(gSaveContext.save.saveInfo.playerData.playerName); i++) {
            fileSelect->fileNames[fileSelect->buttonIndex][i] = gSaveContext.save.saveInfo.playerData.playerName[i];
        }

        fileSelect->healthCapacity[fileSelect->buttonIndex] = gSaveContext.save.saveInfo.playerData.healthCapacity;
        fileSelect->health[fileSelect->buttonIndex] = gSaveContext.save.saveInfo.playerData.health;
        fileSelect->defenseHearts[fileSelect->buttonIndex] = gSaveContext.save.saveInfo.inventory.defenseHearts;
        fileSelect->questItems[fileSelect->buttonIndex] = gSaveContext.save.saveInfo.inventory.questItems;
        fileSelect->time[fileSelect->buttonIndex] = gSaveContext.save.time;
        fileSelect->day[fileSelect->buttonIndex] = gSaveContext.save.day;
        fileSelect->isOwlSave[fileSelect->buttonIndex] = gSaveContext.save.isOwlSave;
        fileSelect->rupees[fileSelect->buttonIndex] = gSaveContext.save.saveInfo.playerData.rupees;
        fileSelect->walletUpgrades[fileSelect->buttonIndex] = CUR_UPG_VALUE(UPG_WALLET);

        for (i = 0, maskCount = 0; i < 24; i++) {
            if (gSaveContext.save.saveInfo.inventory.items[i + 24] != ITEM_NONE) {
                maskCount++;
            }
        }

        fileSelect->maskCount[fileSelect->buttonIndex] = maskCount;
        fileSelect->heartPieceCount[fileSelect->buttonIndex] = GET_QUEST_HEART_PIECE_COUNT;
    }

    gSaveContext.save.time = *((uintptr_t*) 0x801F6AF0);
    gSaveContext.flashSaveAvailable = *((uintptr_t*) 0x801F6AF2);
}