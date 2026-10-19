extern "C" {
#include "z_en_syateki_itm.hpp"

#define FLAGS 0x80000010

void EnSyatekiItm_Init(Actor* thisx, PlayState* play);
void EnSyatekiItm_Destroy(Actor* thisx, PlayState* play);
void EnSyatekiItm_Update(Actor* thisx, PlayState* play);

ActorInit En_Syateki_Itm_InitVars = {
    ACTOR_EN_SYATEKI_ITM,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnSyatekiItm),
    (ActorFunc)EnSyatekiItm_Init,
    (ActorFunc)EnSyatekiItm_Destroy,
    (ActorFunc)EnSyatekiItm_Update,
    NULL,
};

__attribute__((used)) static Vec3f sGreenAppearHome = { 0.0f, -10.0f, -270.0f };
__attribute__((used)) static Vec3f sBlueSeqHome1 = { -220.0f, 66.0f, -320.0f };
__attribute__((used)) static Vec3f sBlueSeqHome2 = { 260.0f, 66.0f, -320.0f };
__attribute__((used)) static Vec3f sGreenThrowHome = { 0.0f, -10.0f, -270.0f };
__attribute__((used)) static Vec3f sBlueSimulHome1 = { -220.0f, 66.0f, -320.0f };
__attribute__((used)) static Vec3f sBlueSimulHome2 = { 260.0f, 66.0f, -320.0f };
__attribute__((used)) static Vec3f sRedLeftHome1 = { 260.0f, 100.0f, -320.0f };
__attribute__((used)) static Vec3f sRedLeftHome2 = { 360.0f, 100.0f, -320.0f };
__attribute__((used)) static Vec3f sRedRightHome1 = { -230.0f, 94.0f, -360.0f };
__attribute__((used)) static Vec3f sRedRightHome2 = { -400.0f, 94.0f, -360.0f };
__attribute__((used)) static Vec3f sGreenAppearFinal = { 0.0f, 53.0f, -270.0f };
// Indices: 0 = BlueSeqFinal1, 1 = BlueSeqFinal2, 2 = GreenThrowFinal (all zero, so kept out of .bss), 3 = BlueSimulFinal1, 4 = BlueSimulFinal2
__attribute__((used)) static Vec3f sFinalPosTable[] = {
    { -60.0f, 63.0f, -320.0f }, { 60.0f, 63.0f, -320.0f }, { 0.0f, 0.0f, 0.0f }, { -60.0f, 63.0f, -320.0f }, { 60.0f, 63.0f, -320.0f },
};
__attribute__((used)) static Vec3f sRedLeftFinal1 = { -230.0f, 0.0f, 0.0f };
__attribute__((used)) static Vec3f sRedLeftFinal2 = { -230.0f, 0.0f, 0.0f };
__attribute__((used)) static Vec3f sRedRightFinal1 = { 260.0f, 0.0f, 0.0f };
__attribute__((used)) static Vec3f sRedRightFinal2 = { 260.0f, 0.0f, 0.0f };
__attribute__((used)) static s16 sTargetColors[] = { 0, 1, 0, 1, 2, 2 };
__attribute__((used)) static s16 sRupeeTypes[] = { 0, 1, 1, 0, 1, 1, 2, 2, 2, 2 };
__attribute__((used)) static Vec3f sRupeePos[] = {
    { -40.0f, 0.0f, -90.0f }, { -20.0f, 0.0f, -90.0f }, { 0.0f, 0.0f, -90.0f },   { 20.0f, 0.0f, -90.0f },
    { 40.0f, 0.0f, -90.0f },  { -40.0f, 0.0f, -60.0f }, { -20.0f, 0.0f, -60.0f }, { 0.0f, 0.0f, -60.0f },
    { 20.0f, 0.0f, -60.0f },  { 40.0f, 0.0f, -60.0f },
};

void FUN_0034557c(PlayState* play, s32 arg); // func_8008EF44 (N64)
void FUN_002599b0(EnSyatekiItm* self, PlayState* play); // StartRound (functions_*.cpp)
void FUN_001764fc(EnSyatekiItm* self, PlayState* play); // SpawnTargets (functions_*.cpp)
void FUN_002aa244(EnSyatekiItm* self, PlayState* play); // CleanupGame (functions_*.cpp)
void FUN_0021cf54(EnSyatekiItm* self, PlayState* play); // CheckTargets (functions_*.cpp)
void FUN_00239124(EnSyatekiItm* self, PlayState* play); // EndGame (functions_*.cpp)

#define PLAYER_YAW(p) (*(s16*)((u8*)(p) + 0x2220))
#define SHOOTING_GALLERY_STATUS(p) (*(s8*)((u8*)(p) + 0x5C74)) // PlayState, no field in repo header

void FUN_0019ef00(EnSyatekiItm* self, PlayState* play) {
    s32 i;
    Player* player = GET_PLAYER(play);

    if (self->signal == ENSYATEKI_START) {
        player->actor.world.pos.x = -12.0f;
        player->actor.world.pos.y = 20.0f;
        player->actor.world.pos.z = 182.0f;
        PLAYER_YAW(player) = player->actor.world.rot.y = player->actor.shape.rot.y = 0x7F03;
        player->actor.world.rot.x = player->actor.shape.rot.x = player->actor.world.rot.z = player->actor.shape.rot.z =
            0;
        FUN_0034557c(play, 15);
        self->roundNum = self->hitCount = 0;
        for (i = 0; i < 6; i++) {
            self->roundFlags[i] = false;
        }
        for (i = 0; i < 10; i++) {
            self->markers[i]->galleryFlag = false;
        }
        self->actionFunc = FUN_002599b0;
    }
}

void FUN_002599b0(EnSyatekiItm* self, PlayState* play) {
    s32 i;
    s32 j;
    Player* player = GET_PLAYER(play);

    if (self->unkTimer == 0) {
        if (LINK_IS_ADULT) {
            for (i = 0, j = 0; i < 6; i++) {
                if (self->roundFlags[i]) {
                    j++;
                }
            }
            if (j >= 6) {
                player->actor.freezeTimer = 15;
                self->signal = ENSYATEKI_END;
                self->actionFunc = FUN_002aa244;
                return;
            }
            i = Rand_ZeroFloat(5.99f);
            while (self->roundFlags[i]) {
                i = Rand_ZeroFloat(5.99f);
                if (1) {}
            }
            self->roundNum = i + 1;
            self->roundFlags[i] = true;
        } else {
            self->roundNum++;
            if (self->roundNum > 6) {
                player->actor.freezeTimer = 15;
                self->signal = ENSYATEKI_END;
                self->actionFunc = FUN_002aa244;
                return;
            }
        }

        self->timer = (self->roundNum == 1) ? 75 : 45;

        Audio_PlaySoundGeneral(0x10004B1, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        self->actionFunc = FUN_001764fc;
    }
}

void FUN_0021cf54(EnSyatekiItm* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 j = 0;

    if (self->targetState[0] != ENSYATEKIHIT_NONE) {
        if (self->targetState[0] == ENSYATEKIHIT_HIT) {
            self->curMarkers[0]->galleryFlag = true;
        }
        j++;
    }
    if (self->targetState[1] != ENSYATEKIHIT_NONE) {
        if (self->targetState[1] == ENSYATEKIHIT_HIT) {
            self->curMarkers[1]->galleryFlag = true;
        }
        j++;
    }

    if (SHOOTING_GALLERY_STATUS(play) == -1) {
        player->actor.freezeTimer = 15;
        self->signal = ENSYATEKI_END;
        self->actionFunc = FUN_002aa244;
    } else if (j == self->numTargets && SHOOTING_GALLERY_STATUS(play) > 0) {
        self->actionFunc = FUN_002599b0;
    }
}

void FUN_002aa244(EnSyatekiItm* self, PlayState* play) {
    s32 i;

    for (i = 0; i < 2; i++) {
        if ((self->targetState[i] == ENSYATEKIHIT_NONE) && (self->targets[i] != NULL)) {
            Actor_Kill((Actor*)self->targets[i]);
        }
    }
    self->actionFunc = FUN_00239124;
}

void FUN_00239124(EnSyatekiItm* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    player->actor.freezeTimer = 15;
    if (self->signal == ENSYATEKI_RESULTS) {
        self->signal = ENSYATEKI_NONE;
        self->actionFunc = FUN_0019ef00;
    }
    if (self->signal == ENSYATEKI_START) {
        self->signal = ENSYATEKI_NONE;
        self->actionFunc = FUN_0019ef00;
    }
}

void EnSyatekiItm_Init(Actor* thisx, PlayState* play) {
    EnSyatekiItm* self = (EnSyatekiItm*)thisx;
    s32 i;

    self->man = (EnSyatekiMan*)Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, ACTOR_EN_SYATEKI_MAN, 140.0f,
                                                  0.0f, 255.0f, 0, -0x4000, 0, 0);
    if (self->man == NULL) {
        Actor_Kill(&self->actor);
        return;
    }
    for (i = 0; i < 10; i++) {
        self->markers[i] = (EnSyatekiItmMarker*)Actor_SpawnAsChild(&play->actorCtx, &self->actor, play,
                                                                   ACTOR_EN_EX_RUPPY, sRupeePos[i].x, sRupeePos[i].y,
                                                                   sRupeePos[i].z, 0, 0, 0, 4);
        if (self->markers[i] == NULL) {
            Actor_Kill(&self->actor);
            return;
        }
        self->markers[i]->colorIdx = sRupeeTypes[i];
    }
    self->actionFunc = FUN_0019ef00;
}

void EnSyatekiItm_Destroy(Actor* thisx, PlayState* play) {
}

void EnSyatekiItm_Update(Actor* thisx, PlayState* play) {
    EnSyatekiItm* self = (EnSyatekiItm*)thisx;

    self->actionFunc(self, play);

    if (self->timer != 0) {
        self->timer--;
    }
    if (self->unkTimer != 0) {
        self->unkTimer--;
    }
}

}
