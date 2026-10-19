/* Native3DS behavior reconstructed; best grouped compile3160/3164B597w.
 * Unmatched: original ASM remains active. */
#if 0
/* 3DS intro reconstruction. Native reference: tmp/sol/FUN_0019e118.decoded.txt.
 * SaveContext+EFA is eventChkInf[7]; unk_2060[8] is the native one-shot
 * player-model flag. Controller fields are observed at +3E8/+3EC.
 * This candidate is provisional until an exact compile and final link. */
#include "z_boss_dodongo.hpp"
#include "model_manager.hpp"
#include "graphics/skeleton_animation_model.hpp"

struct IntroCameraController {
    u8 pad[0x3E8];
    s32 mode;
    s32 state;
    // The native query returns one for every nonnegative state.
    s32 IsActive() const { return (state >> 31) + 1; }
};
typedef char IntroControllerStateOffset[(offsetof(IntroCameraController, state)==0x3EC)?1:-1];
extern "C" {
struct IntroSaveView { u8 pad[0xFA]; u16 event7; };
extern IntroSaveView gSaveContext_E00;
extern s32 BossDodongo_AnimTable[];
s32 Animation_GetLastFrame(SkelAnime*,s32);
static inline f32 IntroAnimationLastFrame(SkelAnime* skel, s32 animation) {
    return (f32)Animation_GetLastFrame(skel, animation);
}
void FUN_00353020(SkelAnime*,s32*,f32,f32,f32,s32,f32);
void FUN_00345a08(BossDodongo*,PlayState*,s32,f32,f32);
void FUN_0040a084(void*);
void FUN_00345590(BossDodongo*,PlayState*);
void FUN_002b6b9c(PlayState*);
void FUN_0035b1cc(PlayState*,s16,Vec3f*,Vec3f*,Vec3f*);
void FUN_0036f00c(PlayState*,Actor*,Vec3f*,f32,s16,f32,s16,s16,u8);
void Audio_SetBGM(s32,s32);
f32 fabsf(f32);
f32 sinf(f32);
void FUN_0019e118(BossDodongo* self, PlayState* play) {
    f32 phi_f0;
    Camera* mainCam;
    Player* player;
    Vec3f subCamEye;
    Vec3f subCamAt;
    Vec3f subCamUp;

    IntroCameraController* cameraController = (IntroCameraController*)((u8*)&GetModelManager()+0x59A0);
    player = GET_PLAYER(play);
    mainCam = Gameplay_GetCamera(play, 0);

    if (self->unk_766 != 0) {
        self->unk_766--;
    }

    if (self->unk_768 != 0) {
        self->unk_768--;
    }

    if (self->unk_76A != 0) {
        self->unk_76A--;
    }

    f32 zero = 0.0f;
    f32 one = 1.0f;
    switch (self->unk_76C) {
        case 0:
            if (player->actor.world.pos.y < -1220.0f) {
                self->unk_76C = 1;
                self->actor.world.pos.x = -1390.0f;
                self->actor.world.pos.z = -3374.0f;
                self->unk_770 = 1;
            }
            if (gSaveContext_E00.event7 & 2) {
                self->skelAnime.unk_28->unk_AD[0] = 1;
            } else {
                self->skelAnime.unk_28->unk_AD[0] = 0;
            }
            break;
        case 1:
            FUN_00367494(play, &play->csCtx);
            FUN_0036e980(play, &self->actor, 1);
            FUN_002b6b9c(play);
            self->unk_784 = Gameplay_CreateSubCamera(play);
            Gameplay_ChangeCameraStatus(play, 0, CAM_STAT_WAIT);
            Gameplay_ChangeCameraStatus(play, self->unk_784, CAM_STAT_ACTIVE);
            self->unk_76C = 2;
            self->unk_766 = 90;
            self->unk_768 = 240;
            player->actor.world.pos.y = -1020.0f;
            self->unk_9FC = -1450.0f;
            FUN_00345a08(self, play, 0, 80.0f, 55.0f);

        case 2:
            if (self->unk_766 > 0) {
                FUN_00345a08(self, play, 10, 60.0f, 55.0f);
            }
            if (cameraController->IsActive() && cameraController->mode == 8) {
                FUN_0040a084(cameraController);
            }
            if (player->actor.world.pos.y < -1500.0f && self->unk_2060[8] == 0) {
                FUN_0035af04(player, 1);
                FUN_0036e980(play, &self->actor, 0x67);
                self->unk_2060[8] = 1;
            }
            if (self->unk_768 > 195) {
                player->actor.world.pos.x = -890.0f;
                player->actor.world.pos.z = -2804.0f;

                player->actor.speedXZ = zero;
                player->actor.shape.rot.y = player->actor.world.rot.y = 0x3FFF;

                self->unk_9F8 = -890.0f;
                self->unk_A00 = player->actor.world.pos.z - 100.0f;

                self->unk_A04 = player->actor.world.pos.x;
                self->unk_A08 = player->actor.world.pos.y + 20.0f;
                self->unk_A0C = player->actor.world.pos.z;
            }

            if (self->unk_768 == 165) {
                FUN_0036e980(play, &self->actor, 9);
            }

            if (self->unk_768 == 8) {
                FUN_0036e980(play, &self->actor, 12);
            }

            if (self->unk_768 > 8) {
                player->actor.shape.rot.y = 0x3FFF;
            }

            if (self->unk_768 < 90) {
                self->unk_78C = 1;
            } else {
                self->unk_78C = 2;
            }

            FUN_00345590(self, play);

            if (self->unk_766 == 1) {
                Audio_SetBGM(0,1);
            }

            if (self->unk_766 == 0) {
                FUN_00345a08(self, play, 5, 105.0f, 55.0f);
                Math_SmoothStepToF(&self->unk_9F8, self->unk_9BC + 30.0f, 0.2f, self->unk_7D4 * 20.0f, zero);
                Math_SmoothStepToF(&self->unk_9FC, self->unk_9C0, 0.2f, self->unk_7D4 * 20.0f, zero);
                Math_SmoothStepToF(&self->unk_A00, self->unk_9C4 + 10.0f, 0.2f, self->unk_7D4 * 20.0f, zero);
                Math_SmoothStepToF(&self->unk_7D4, one, one, 0.02f, zero);
            } else {
                self->unk_A04 = player->actor.world.pos.x;
                self->unk_A08 = player->actor.world.pos.y + 20.0f;
                self->unk_A0C = player->actor.world.pos.z;
            }

            if ((gSaveContext_E00.event7&2)) {
                if (self->unk_768 == 150) {
                    self->actor.world.pos.x = -1214.0f;
                    self->actor.world.pos.z = -2804.0f;
                    self->actor.world.rot.y = 0x3FFF;
                    self->unk_772 = 0;
                    self->unk_770 = 2;
                    self->unk_76C = 4;
                    self->unk_766 = 45;
                    self->unk_768 = 225;
                    self->unk_7D4 = zero;
                    FUN_00353020(&self->skelAnime, &BossDodongo_AnimTable[4], one, zero,
                                     IntroAnimationLastFrame(&self->skelAnime,BossDodongo_AnimTable[4]), 2, zero);
                    FUN_00370734(&self->skelAnime);
                }
            } else if (self->unk_768 == 0) {
                self->unk_76C = 3;
                self->unk_76E = 0x14;
                self->unk_7D4 = zero;
            }
            break;
        case 3:
            FUN_00345590(self, play);
            Math_SmoothStepToF(&self->unk_7DC, sinf(self->unk_76E * 0.05f) * 0.1f, one, 0.01f, zero);
            Math_SmoothStepToF(&self->unk_9F8, self->unk_9BC + 90.0f, 0.2f, self->unk_7D4 * 20.0f, zero);
            Math_SmoothStepToF(&self->unk_9FC, self->unk_9C0 + 50.0f, 0.2f, self->unk_7D4 * 20.0f, zero);
            Math_SmoothStepToF(&self->unk_A00, self->unk_9C4, 0.2f, self->unk_7D4 * 20.0f, zero);
            Math_SmoothStepToF(&self->unk_A08, self->unk_9C0 - 10.0f, 0.2f, self->unk_7D4 * 20.0f, zero);
            Math_SmoothStepToF(&self->unk_7D4, one, one, 0.02f, zero);
            if (fabsf(player->actor.world.pos.x - self->actor.world.pos.x) < 200.0f) {
                self->unk_76C = 4;
                self->unk_766 = 45;
                self->unk_768 = 225;
                self->unk_7D4 = zero;
                FUN_00353020(&self->skelAnime, &BossDodongo_AnimTable[4], one, zero,
                                 IntroAnimationLastFrame(&self->skelAnime,BossDodongo_AnimTable[4]), 2, -5.0f);
                FUN_00345a08(self, play, 4, 38.0f, 55.0f);
                self->skelAnime.unk_28->unk_AD[0] = 1;
            }
            break;
        case 4:
            if (self->unk_766 > 0) {
                FUN_00345a08(self, play, 4, 38.0f, 55.0f);
            }
            Math_SmoothStepToF(&self->unk_7DC, zero, one, 0.01f, zero);

            if ((gSaveContext_E00.event7&2)) {
                phi_f0 = -50.0f;
            } else {
                phi_f0 = zero;
            }

            Math_SmoothStepToF(&self->unk_9F8, player->actor.world.pos.x + (phi_f0 + 70.0f), 0.2f,
                               self->unk_7D4 * 20.0f, zero);
            Math_SmoothStepToF(&self->unk_9FC, player->actor.world.pos.y + 10.0f, 0.2f, self->unk_7D4 * 20.0f,
                               zero);
            Math_SmoothStepToF(&self->unk_A00, player->actor.world.pos.z - 60.0f, 0.2f, self->unk_7D4 * 20.0f,
                               zero);

            Math_SmoothStepToF(&self->unk_A04, self->unk_9BC, 0.2f, self->unk_7D4 * 20.0f, zero);
            Math_SmoothStepToF(&self->unk_A08, self->unk_9C0, 0.2f, self->unk_7D4 * 20.0f, zero);
            Math_SmoothStepToF(&self->unk_A0C, self->unk_9C4, 0.2f, self->unk_7D4 * 20.0f, zero);
            Math_SmoothStepToF(&self->unk_7D4, one, one, 0.02f, zero);

            if (Animation_OnFrameImpl(&self->skelAnime, 4.7f, one)) {
                FUN_0036f00c(play, &self->actor, (Vec3f*)&self->unk_9E0, 25.0f, 10, 8.0f, 500, 10, 0);
                Audio_PlayActorSound2(&self->actor, 0x01000465);
            } else if (Animation_OnFrameImpl(&self->skelAnime, 11.3f, one)) {
                FUN_0036f00c(play, &self->actor, (Vec3f*)&self->unk_9D4, 25.0f, 10, 8.0f, 500, 10, 0);
                Audio_PlayActorSound2(&self->actor, 0x01000465);
            }

            if (self->unk_766 == 0) {
                FUN_00370734(&self->skelAnime);
                Math_SmoothStepToF(&self->unk_7D8, 0.05f, one, 0.005f, zero);
            }

            if (self->unk_768 == 150) {
                Audio_PlayActorSound2(&self->actor, 0x010002C2);
            }

            if (self->unk_768 == 135) {
                if (!(gSaveContext_E00.event7&2)) {
                    TitleCard_InitBossName(play, &play->actorCtx.titleCtx,
                                           self->texture, 200, 180, 256, 64,zero);
                }
                Audio_QueueSeqCmd(0,0x010005D8,0);
                if (FUN_0035b164() != 0 && FUN_0035b0a0() == 0) {
                    FUN_003542c4((u8*)&GetModelManager()+0x59A0, 1, 1);
                }
            }

            if (self->unk_768 == 0) {
                FUN_00345a08(self, play, -1, 38.0f, 55.0f);
                mainCam->eye = (*(Vec3f*)&self->unk_9F8);
                mainCam->eyeNext = (*(Vec3f*)&self->unk_9F8);
                mainCam->at = (*(Vec3f*)&self->unk_A04);
                FUN_0036e9b8(play, self->unk_784, 0);
                self->unk_784 = 0;
                FUN_00367374(play, &play->csCtx);
                FUN_0036e980(play, &self->actor, 7);
                FUN_00353020(&self->skelAnime,&BossDodongo_AnimTable[2],one,zero,IntroAnimationLastFrame(&self->skelAnime,BossDodongo_AnimTable[2]),2,-10.0f);
                self->unk_77A=0; self->actionFunc=FUN_00345590;
                self->actor.flags|=1; self->unk_7B4=zero;
                self->unk_7AA = 75;
                self->unk_78C = 0;
                player->actor.shape.rot.y = -0x4002;
                (gSaveContext_E00.event7|=2);
            }
            break;
    }

    if (self->unk_784 != 0) {
        if (self->unk_786 != 0) {
            self->unk_786--;
        }

        subCamEye.x = self->unk_9F8;
        phi_f0 = sinf((self->unk_786 * 3.1415f * 90.0f) * (one / 180.0f));
        subCamEye.y = (self->unk_786 * phi_f0 * 0.7f) + self->unk_9FC;
        subCamEye.z = self->unk_A00;

        subCamAt.x = self->unk_A04;
        phi_f0 = sinf((self->unk_786 * 3.1415f * 90.0f) * (one / 180.0f));
        subCamAt.y = (self->unk_786 * phi_f0 * 0.7f) + self->unk_A08;
        subCamAt.z = self->unk_A0C;

        subCamUp.x = self->unk_7DC;
        subCamUp.y = one;
        subCamUp.z = self->unk_7DC;

        FUN_0035b1cc(play, self->unk_784, &subCamAt, &subCamEye, &subCamUp);
    }
}

}

#endif
