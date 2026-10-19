/* Inactive full N64 control-flow ports: the 3DS intro/death additions and timers
 * remain incompletely reconciled. These are NOT complete target reconstructions.
 * Update also remains unmatched. Original ASM owns every active target.
 * Reference instruction decoding: tmp/sol/*.decoded.txt. */
#if 0
#include "z_boss_dodongo.hpp"

#include "model_manager.hpp"
#include "graphics/skeleton_animation_model.hpp"
#include "mtx34_trans.hpp"
extern "C" {
extern s32 BossDodongo_AnimTable[];
extern s32 BossDodongo_Anim1, BossDodongo_Anim3, BossDodongo_Anim4, BossDodongo_Anim5, BossDodongo_Anim6, BossDodongo_Anim7, BossDodongo_Anim11;
extern Vec3f BossDodongo_Corners[4];
extern InitChainEntry BossDodongo_InitChain[];
extern u8 BossDodongo_ColliderInit[];
extern u8 BossDodongo_DustPrim[], BossDodongo_DustEnv[];
extern s16 BossDodongo_ExitParams;
s32 Animation_GetLastFrame(SkelAnime*,s32);
void FUN_00353020(SkelAnime*,s32*,f32,f32,f32,s32,f32);
void FUN_003490e0(SkelAnime*,s32*);
void FUN_00181770(BossDodongo*,PlayState*);
void FUN_0019e118(BossDodongo*,PlayState*);
void FUN_003da7ac(BossDodongo*,PlayState*);
void FUN_003dc268(BossDodongo*,PlayState*);
void FUN_0013cb28(BossDodongo*,PlayState*);
void FUN_0013ff98(BossDodongo*,PlayState*);
void FUN_001e81fc(BossDodongo*,PlayState*);
void FUN_001ccc78(BossDodongo*,PlayState*);
void FUN_00345590(BossDodongo*,PlayState*);
void FUN_003309e0(Actor*,PlayState*,s32);
void FUN_0036627c(void*,s16,s16,s16);
void FUN_0036fca8(Actor*,PlayState*,s16,s16);
void FUN_0036f00c(PlayState*,Actor*,Vec3f*,f32,s16,f32,s16,s16,u8);
void FUN_00365d20(PlayState*,Vec3f*,Vec3f*,Vec3f*,void*,void*,s16,s16,s16);
void FUN_00365560(PlayState*,s32,s32,Vec3f*,s16);
void FUN_00358778(void*,s32,s32,void*,s32);
void FUN_00371fac(MTX34*,MTX34*);
void FUN_003815f0(SkelAnime*,s32,MTX34*,Actor*);
void FUN_00389970(SkelAnime*,s32,MTX34*,Actor*);
ZARInfo* FUN_00352ee0(Actor*,PlayState*,s32,SkeletonAnimationModel**,s32*);
void* ZAR_GetCTXBByIndex(ZARInfo*,s32);
void Collider_UpdateSpheres(s32,void*,MTX34*);
void Collider_InitJntSph(PlayState*,void*);
void Collider_SetJntSph(PlayState*,void*,Actor*,void*,void*);
void Actor_SetColorFilter(Actor*,s32,s16,s16,s16);
s32 Flags_GetClear(PlayState*,s16);
void Flags_SetClear(PlayState*,s16);
f32 fabsf(f32); f32 sinf(f32); f32 cosf(f32);




#define BOSS_MREG(n) (*(s16*)((u8*)gGameInfo+0x454+2*((n)-64)))
#define STACK_PAD(x)
#define UNUSED
#define SQ(x) ((x)*(x))

#define RAD_TO_BINANG(x) ((s16)((x)*10430.3779296875f))
#define BINANG_TO_RAD(x) ((x)*(3.14159265358979323846f/32768.0f))
void FUN_002b6b9c(PlayState*);
void FUN_0035b1cc(PlayState*,s16,Vec3f*,Vec3f*,Vec3f*);
void FUN_00315350(BossDodongo*,PlayState*,s32,f32,f32);
void FUN_00345a08(BossDodongo*,PlayState*,s32,f32,f32);

void FUN_0040a084(void*);
void FUN_00345590(BossDodongo*,PlayState*);
void FUN_0029e828(BossDodongo*,PlayState*);
void FUN_0037100c(PlayState*,Vec3f*);
void FUN_003373c8(PlayState*,Vec3f*,Color_RGBA8*,Color_RGBA8*,s16,s16,s16);
void FUN_00357a50(void*,s16);
void Audio_SetBGM(s32,s32);
s16 Actor_WorldYawTowardActor(Actor*,Actor*);
extern u8 BossDodongo_DissolveTexture[];
static inline f32 func_808C4F6C(BossDodongo* self, PlayState* play) {
    f32 xDiff;
    f32 zDiff;
    f32 sp2C;
    STACK_PAD(s32);
    f32 temp_f2;
    f32 rotation;
    Player* player = GET_PLAYER(play);

    xDiff = player->actor.world.pos.x - self->actor.world.pos.x;
    zDiff = player->actor.world.pos.z - self->actor.world.pos.z;

    rotation = Math_CosS(-self->actor.world.rot.y);
    sp2C = (Math_SinS(-self->actor.world.rot.y) * zDiff) + (rotation * xDiff);
    rotation = Math_SinS(-self->actor.world.rot.y);
    temp_f2 = (Math_CosS(-self->actor.world.rot.y) * zDiff) + (-rotation * xDiff);

    if ((fabsf(sp2C) < 150.0f) && (temp_f2 >= 100.0f) && (temp_f2 <= 2000.0f)) {
        return temp_f2;
    }
    return -1.0f;
}
static inline f32 func_808C50A8(BossDodongo* self, PlayState* play) {
    f32 xDiff;
    f32 zDiff;
    f32 sp2C;
    STACK_PAD(s32);
    f32 temp_f2;
    f32 rotation;
    Player* player = GET_PLAYER(play);

    xDiff = player->actor.world.pos.x - self->actor.world.pos.x;
    zDiff = player->actor.world.pos.z - self->actor.world.pos.z;

    rotation = Math_CosS(-0x8000 - self->actor.world.rot.y);
    sp2C = (Math_SinS(-0x8000 - self->actor.world.rot.y) * zDiff) + (rotation * xDiff);
    rotation = Math_SinS(-0x8000 - self->actor.world.rot.y);
    temp_f2 = (Math_CosS(-0x8000 - self->actor.world.rot.y) * zDiff) + (-rotation * xDiff);

    if ((fabsf(sp2C) < 150.0f) && (100.0f <= temp_f2) && (temp_f2 <= 2000.0f)) {
        return temp_f2;
    }

    return -1.0f;
}
static inline void BossDodongo_PlayerYawCheck(BossDodongo* self, PlayState* play) {
    s16 yawDiff = Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(play)->actor) - self->actor.world.rot.y;

    if ((yawDiff < 0x38E3) && (-0x38E3 < yawDiff)) {
        self->unk_788 = 1;
    } else {
        self->unk_788 = 0;
    }
}
static inline void BossDodongo_PlayerPosCheck(BossDodongo* self, PlayState* play) {
    Vec3f* temp_v1;
    s16 i;

    self->unk_78A = 0;

    for (i = 0; i < 4; i++) {
        temp_v1 = &BossDodongo_Corners[i];

        if ((fabsf(self->actor.world.pos.x - temp_v1->x) < 200.0f) &&
            (fabsf(self->actor.world.pos.z - temp_v1->z) < 200.0f)) {
            self->unk_78A = 1;
            break;
        }
    }
}

static inline void BossDodongo_SpawnEffect(PlayState* play, Vec3f* p, Vec3f* v, Vec3f* a,f32 scale,s16 limit) {
    BossDodongoEffect* eff=*(BossDodongoEffect**)((u8*)play+0x5C28);
    for(s16 i=0;i<limit;i++,eff++) if(eff->active==0) {
        eff->active=1;eff->pos=*p;eff->velocity=*v;eff->accel=*a;
        eff->scale=scale*0.001f;eff->alpha=255;eff->timer=(s16)Rand_ZeroFloat(10.0f);break;
    }
}
static inline void BossDodongo_UpdateEffects(PlayState* play) {
    BossDodongoEffect* eff=*(BossDodongoEffect**)((u8*)play+0x5C28);
    Color_RGB8 effectColors[4]={{255,128,0},{255,0,0},{255,255,0},{255,0,0}};
    for(s16 i=0;i<80;i++,eff++) if(eff->active!=0) {
        eff->timer++;
        eff->pos.x+=eff->velocity.x*(gGameInfo->data[126]/3.0f);
        eff->pos.y+=eff->velocity.y*(gGameInfo->data[126]/3.0f);
        eff->pos.z+=eff->velocity.z*(gGameInfo->data[126]/3.0f);
        eff->velocity.x+=eff->accel.x*(gGameInfo->data[126]/3.0f);
        eff->velocity.y+=eff->accel.y*(gGameInfo->data[126]/3.0f);
        eff->velocity.z+=eff->accel.z*(gGameInfo->data[126]/3.0f);
        if(eff->active==1) {
            eff->color[0]=effectColors[eff->timer&3].r;
            eff->color[1]=effectColors[eff->timer&3].g;
            eff->color[2]=effectColors[eff->timer&3].b;
            eff->alpha-=(s32)(20.0f*(gGameInfo->data[126]/3.0f)+0.5f);
            if(eff->alpha<=0) {eff->alpha=0;eff->active=0;}
        }
    }
}
void FUN_0019e118(BossDodongo* self, PlayState* play) {
    f32 phi_f0;
    Camera* mainCam;
    Player* player = GET_PLAYER(play);
    Vec3f subCamEye;
    Vec3f subCamAt;
    Vec3f subCamUp;

    void* cameraController=(u8*)&GetModelManager()+0x59A0;
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

    switch (self->unk_76C) {
        case 0:
            if (player->actor.world.pos.y < -1220.0f) {
                self->unk_76C = 1;
                self->actor.world.pos.x = -1390.0f;
                self->actor.world.pos.z = -3374.0f;
                self->unk_770 = 1;
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
            self->unk_9FC = player->actor.world.pos.y - 480.0f + 50.0f;

        case 2:
            if (self->unk_768 >= 196) {
                player->actor.world.pos.x = -890.0f;
                player->actor.world.pos.z = -2804.0f;

                player->actor.speedXZ = 0.0f;
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

            if (self->unk_768 <= 8) {
                player->actor.shape.rot.y = -0x4001;
            } else {
                player->actor.shape.rot.y = 0x3FFF;
            }

            if (self->unk_768 <= 80) {
                self->unk_78C = 1;
            } else {
                self->unk_78C = 2;
            }

            FUN_00345590(self, play);

            if (self->unk_766 == 1) {
                Audio_SetBGM(0,1);
            }

            if (self->unk_766 == 0) {
                Math_SmoothStepToF(&self->unk_9F8, self->unk_9BC + 30.0f, 0.2f, self->unk_7D4 * 20.0f, 0.0f);
                Math_SmoothStepToF(&self->unk_9FC, self->unk_9C0, 0.2f, self->unk_7D4 * 20.0f, 0.0f);
                Math_SmoothStepToF(&self->unk_A00, self->unk_9C4 + 10.0f, 0.2f, self->unk_7D4 * 20.0f, 0.0f);
                Math_SmoothStepToF(&self->unk_7D4, 1.0f, 1.0f, 0.02f, 0.0f);
            } else {
                self->unk_A04 = player->actor.world.pos.x;
                self->unk_A08 = player->actor.world.pos.y + 20.0f;
                self->unk_A0C = player->actor.world.pos.z;
            }

            if ((gSaveContext.eventChkInf[7]&2)) {
                if (self->unk_768 == 150) {
                    self->actor.world.pos.x = -1214.0f;
                    self->actor.world.pos.z = -2804.0f;
                    self->actor.world.rot.y = 0x3FFF;
                    self->unk_772 = 0;
                    self->unk_770 = 2;
                    self->unk_76C = 4;
                    self->unk_766 = 45;
                    self->unk_768 = 225;
                    self->unk_7D4 = 0.0f;
                    FUN_00353020(&self->skelAnime, &BossDodongo_AnimTable[4], 1.0f, 0.0f,
                                     Animation_GetLastFrame(&self->skelAnime,BossDodongo_AnimTable[4]), 2, 0.0f);
                    FUN_00370734(&self->skelAnime);
                }
            } else if (self->unk_768 == 0) {
                self->unk_76C = 3;
                self->unk_76E = 0x14;
                self->unk_7D4 = 0.0f;
            }
            break;
        case 3:
            FUN_00345590(self, play);
            Math_SmoothStepToF(&self->unk_7DC, sinf(self->unk_76E * 0.05f) * 0.1f, 1.0f, 0.01f, 0.0f);
            Math_SmoothStepToF(&self->unk_9F8, self->unk_9BC + 90.0f, 0.2f, self->unk_7D4 * 20.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_9FC, self->unk_9C0 + 50.0f, 0.2f, self->unk_7D4 * 20.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_A00, self->unk_9C4, 0.2f, self->unk_7D4 * 20.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_A08, self->unk_9C0 - 10.0f, 0.2f, self->unk_7D4 * 20.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_7D4, 1.0f, 1.0f, 0.02f, 0.0f);
            if (fabsf(player->actor.world.pos.x - self->actor.world.pos.x) < 200.0f) {
                self->unk_76C = 4;
                self->unk_766 = 45;
                self->unk_768 = 225;
                self->unk_7D4 = 0.0f;
                FUN_00353020(&self->skelAnime, &BossDodongo_AnimTable[4], 1.0f, 0.0f,
                                 Animation_GetLastFrame(&self->skelAnime,BossDodongo_AnimTable[4]), 2, -5.0f);
            }
            break;
        case 4:
            Math_SmoothStepToF(&self->unk_7DC, 0.0f, 1.0f, 0.01f, 0.0f);

            if ((gSaveContext.eventChkInf[7]&2)) {
                phi_f0 = -50.0f;
            } else {
                phi_f0 = 0.0f;
            }

            Math_SmoothStepToF(&self->unk_9F8, player->actor.world.pos.x + phi_f0 + 70.0f, 0.2f,
                               self->unk_7D4 * 20.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_9FC, player->actor.world.pos.y + 10.0f, 0.2f, self->unk_7D4 * 20.0f,
                               0.0f);
            Math_SmoothStepToF(&self->unk_A00, player->actor.world.pos.z - 60.0f, 0.2f, self->unk_7D4 * 20.0f,
                               0.0f);

            Math_SmoothStepToF(&self->unk_A04, self->unk_9BC, 0.2f, self->unk_7D4 * 20.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_A08, self->unk_9C0, 0.2f, self->unk_7D4 * 20.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_A0C, self->unk_9C4, 0.2f, self->unk_7D4 * 20.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_7D4, 1.0f, 1.0f, 0.02f, 0.0f);

            if (self->unk_766 == 0) {
                FUN_00370734(&self->skelAnime);
                Math_SmoothStepToF(&self->unk_7D8, 0.05f, 1.0f, 0.005f, 0.0f);
            }

            if (self->unk_768 == 150) {
                Audio_PlayActorSound2(&self->actor, 0x010002C2);
            }

            if (self->unk_768 == 135) {
                if (!(gSaveContext.eventChkInf[7]&2)) {
                    TitleCard_InitBossName(play, &play->actorCtx.titleCtx,
                                           self->texture, 200, 180, 256, 64,0.0f);
                }
                Audio_QueueSeqCmd(0,0x010005D8,0);
            }

            if (self->unk_768 == 0) {
                mainCam->eye = (*(Vec3f*)&self->unk_9F8);
                mainCam->eyeNext = (*(Vec3f*)&self->unk_9F8);
                mainCam->at = (*(Vec3f*)&self->unk_A04);
                FUN_0036e9b8(play, self->unk_784, 0);
                self->unk_784 = 0;
                FUN_00367374(play, &play->csCtx);
                FUN_0036e980(play, &self->actor, 7);
                FUN_00353020(&self->skelAnime,&BossDodongo_AnimTable[2],1.0f,0.0f,(f32)Animation_GetLastFrame(&self->skelAnime,BossDodongo_AnimTable[2]),2,-10.0f);
                self->unk_77A=0; self->actionFunc=FUN_00345590; self->unk_7AA=0;
                self->actor.flags|=1; self->unk_7B4=0.0f;
                self->unk_7AA = 75;
                self->unk_78C = 0;
                player->actor.shape.rot.y = -0x4002;
                (gSaveContext.eventChkInf[7]|=2);
            }
            break;
    }

    if (self->unk_784 != 0) {
        if (self->unk_786 != 0) {
            self->unk_786--;
        }

        subCamEye.x = self->unk_9F8;
        phi_f0 = sinf((self->unk_786 * 3.1415f * 90.0f) / 180.0f);
        subCamEye.y = (self->unk_786 * phi_f0 * 0.7f) + self->unk_9FC;
        subCamEye.z = self->unk_A00;

        subCamAt.x = self->unk_A04;
        phi_f0 = sinf((self->unk_786 * 3.1415f * 90.0f) / 180.0f);
        subCamAt.y = (self->unk_786 * phi_f0 * 0.7f) + self->unk_A08;
        subCamAt.z = self->unk_A0C;

        subCamUp.x = self->unk_7DC;
        subCamUp.y = 1.0f;
        subCamUp.z = self->unk_7DC;

        FUN_0035b1cc(play, self->unk_784, &subCamAt, &subCamEye, &subCamUp);
    }
}

void BossDodongo_Update(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    BossDodongo* self = (BossDodongo*)thisx;
    f32 temp_f0;
    s16 i;
    Player* player = GET_PLAYER(play);
    Player* player2 = GET_PLAYER(play);
    STACK_PAD(s32);

    self->unk_7B2 = 0;
    self->unk_76E++;

    if (self->unk_7AA != 0) {
        self->unk_7AA--;
    }

    if (self->unk_7AC != 0) {
        self->unk_7AC--;
    }

    if (self->unk_7AE != 0) {
        self->unk_7AE--;
    }

    if (self->unk_790 != 0) {
        self->unk_790--;
    }

    if (self->unk_798 != 0) {
        self->unk_798--;
    }

    temp_f0 = func_808C4F6C(self, play);

    if (temp_f0 > 0.0f) {
        self->unk_774 = temp_f0;
    } else {
        self->unk_774 = 0;
    }

    temp_f0 = func_808C50A8(self, play);

    if (temp_f0 > 0.0f) {
        self->unk_776 = temp_f0;
    } else {
        self->unk_776 = 0;
    }

    BossDodongo_PlayerYawCheck(self, play);
    BossDodongo_PlayerPosCheck(self, play);

    self->actionFunc(self, play);

    thisx->shape.rot.y = thisx->world.rot.y;

    Math_SmoothStepToF(&thisx->shape.yOffset, self->unk_7F8, 1.0f, 100.0f, 0.0f);
    Actor_MoveForward(thisx);
    FUN_0029e828(self, play);
    Actor_UpdateBgCheckInfo(play, thisx, 10.0f, 10.0f, 20.0f, 2);
    Math_SmoothStepToF(&self->unk_7D8, 0, 1, 0.001f, 0.0);
    Math_SmoothStepToF(&self->unk_7DC, 0, 1, 0.001f, 0.0);

    if ((self->unk_76E % 128) == 0) {
        for (i = 0; i < 50; i++) {
            self->unk_8F4[i] = (Rand_ZeroOne() * 0.25f) + 0.5f;
        }
    }

    for (i = 0; i < 50; i++) {
        self->unk_82C[i] += self->unk_8F4[i];
    }

    if (self->unk_798 != 0) {
        if (self->unk_798 >= 11) {
            Math_SmoothStepToF(&self->unk_810, (self->unk_798 & 1) ? (40.0f) : (60.0f), 1.0f, 50.0f, 0.0f);
        } else {
            Math_SmoothStepToF(&self->unk_810, 0.0f, 1, 10.0f, 0.0);
        }

        if ((((s16*)((u8*)play+0x3202))[2] == 0) && (((s16*)((u8*)play+0x31FC))[2] == 0)) {
            ((s16*)((u8*)play+0x3202))[0] = (u8)self->unk_810;
            ((s16*)((u8*)play+0x3202))[1] = (u8)(self->unk_810 * 0.1f);
            ((s16*)((u8*)play+0x31FC))[0] = (u8)self->unk_810;
            ((s16*)((u8*)play+0x31FC))[1] = (u8)(self->unk_810 * 0.1f);
        }
    }

    if (self->unk_78E != 0) {
        if (self->unk_78E >= 1000) {
            Math_SmoothStepToF(&self->unk_7E0, 30.0f, 1, 20.0f, 0.0);
            Math_SmoothStepToF(&self->unk_7E4, 10.0f, 1, 20.0f, 0.0);
        } else {
            self->unk_78E--;
            Math_SmoothStepToF(&self->unk_7E0, 255.0f, 1, 20.0f, 0.0);
            Math_SmoothStepToF(&self->unk_7E4, 0.0f, 1, 20.0f, 0.0);
        }

        Math_SmoothStepToF(&self->unk_7E8, 0.0f, 1, 20.0f, 0.0);
        Math_SmoothStepToF(&self->unk_7EC, 900.0f, 1, 10.0f, 0.0);
        Math_SmoothStepToF(&self->unk_7F0, 1099.0f, 1, 10.0f, 0.0);
    } else {
        self->unk_7E0=0.0f;
        self->unk_7E4=0.0f;
        self->unk_7E8=0.0f;
        Math_SmoothStepToF(&self->unk_7EC, (*(u16*)((u8*)play+0xA7C)), 1.0, 5.0f, 0.0);
        Math_SmoothStepToF(&self->unk_7F0, 1000.0f, 1, 5.0f, 0.0);
    }

    if (player->actor.world.pos.y < -1000.0f) {
        s16 phi_s0_3;
        s16 sp90;
        s16 magma2DrawMode;
        s16 magmaScale = 0;

        if (self->unk_7F4 > 1.9f) {
            phi_s0_3 = 1;
            magma2DrawMode = 0;
            sp90 = 0;
        } else if (self->unk_7F4 > 1.7f) {
            phi_s0_3 = 3;
            sp90 = 1;
            magma2DrawMode = 0;
        } else if (self->unk_7F4 > 1.4f) {
            phi_s0_3 = 7;
            sp90 = 3;
            magma2DrawMode = Rand_ZeroOne() * 1.9f;
        } else if (self->unk_7F4 > 1.1f) {
            phi_s0_3 = 7;
            sp90 = 4095;
            magma2DrawMode = Rand_ZeroOne() * 1.9f;
        } else {
            phi_s0_3 = 1;
            sp90 = -1;
            magma2DrawMode = 1;
            magmaScale = ((s16)(Rand_ZeroOne() * 50)) - 50;
        }

        if ((*(u8*)((u8*)player2+0x21D4)) >= 10) {
            phi_s0_3 = -1;
        }

        if ((self->unk_76E & phi_s0_3) == 0) {
            static Color_RGBA8 magmaPrimColor[] = { { 255, 255, 0, 255 }, { 0, 0, 0, 150 } };
            static Color_RGBA8 magmaEnvColor[] = { { 255, 0, 0, 255 }, { 0, 0, 0, 0 } };
            Vec3f sp84;
            f32 temp_f12;
            f32 temp_f10;

            temp_f12 = Rand_ZeroOne() * 330.0f;
            temp_f10 = Rand_ZeroOne() * 6.28f;
            sp84.x = (sinf(temp_f10) * temp_f12) + (-890.0f);
            sp84.y = -1523.76f;
            sp84.z = (cosf(temp_f10) * temp_f12) + (-3304.0f);
            FUN_003373c8(play, &sp84, &magmaPrimColor[magma2DrawMode], &magmaEnvColor[magma2DrawMode],
                                  10 - (magma2DrawMode * 5), magma2DrawMode, magmaScale + 100);
        }

        if ((self->unk_76E & sp90) == 0) {
            Vec3f sp6C = { 0.0f, 0.0f, 0.0f };
            Vec3f sp60 = { 0.0f, 0.0f, 0.0f };
            Vec3f sp54;
            f32 sp50 = Rand_ZeroOne() * 330.0f;
            f32 sp4C = Rand_ZeroOne() * 6.28f;

            sp54.x = sinf(sp4C) * sp50 + (-890.0f);
            sp54.y = -1523.76f;
            sp54.z = cosf(sp4C) * sp50 + (-3304.0f);
            FUN_0037100c(play, &sp54);
            for (i = 0; i < 4; i++) {
                sp60.y = 0.4f;
                sp60.x = Rand_CenteredFloat(0.5f);
                sp60.z = Rand_CenteredFloat(0.5f);
                sp50 = Rand_ZeroOne() * 330.0f;
                sp4C = Rand_ZeroOne() * 6.28f;
                sp54.x = sinf(sp4C) * sp50 + (-890.0f);
                sp54.y = -1513.76f;
                sp54.z = cosf(sp4C) * sp50 + (-3304.0f);
                BossDodongo_SpawnEffect(play, &sp54, &sp6C, &sp60, ((s16)Rand_ZeroFloat(2.0f)) + 6, 80);
            }
        }
    }

    if (self->unk_796 != 0) {
        FUN_003309e0(&self->actor,play,0);
        for(s16 i2=0;i2<20;i2+=2) self->unk_792+=74;
        Math_SmoothStepToF(&self->unk_7F4, 0.0f, 1.0f, 0.01f, 0.0f);
    }

    if (self->unk_78C == 0) {
        if (self->actionFunc != FUN_003da7ac) {
            CollisionCheck_SetAC(play, &play->colChkCtx, (Collider*)self->collider);
        }

        CollisionCheck_SetOC(play, &play->colChkCtx, (Collider*)self->collider);

        if (self->actionFunc == FUN_001ccc78) {
            CollisionCheck_SetAT(play, &play->colChkCtx, (Collider*)self->collider);
        }
    }

    (*(f32*)(*(u8**)((u8*)self+0xA2C)+(0)*0x50+0x48)) = (self->actionFunc == FUN_001e81fc) ? 0.0f : 1.0f;

    for (i = 6; i < 19; i++) {
        if (i == 12) {
            continue;
        }
        (*(f32*)(*(u8**)((u8*)self+0xA2C)+(i)*0x50+0x48)) = (self->actionFunc == FUN_001ccc78) ? 0.0f : 1.0f;
    }

    if (self->unk_814 != 0) {
        BOSS_MREG(64) = 1;
        BOSS_MREG(65) = 255;
        BOSS_MREG(66) = 80;
        BOSS_MREG(67) = 0;
        BOSS_MREG(68) = (u8)self->unk_814;
    } else {
        BOSS_MREG(64) = 0;
    }

    Math_SmoothStepToF(&self->unk_814, 0.0f, 1.0f, 2.0f, 0.0f);
    BossDodongo_UpdateEffects(play);
}

void FUN_003da7ac(BossDodongo* self, PlayState* play) {
    Vec3f* cornerPos;
    Vec3f sp198;
    Vec3f sp184;
    f32 tempSin;
    f32 tempCos;
    f32 sp178;
    s16 i;
    Vec3f effectPos;
    Camera* mainCam;
    Player* player = GET_PLAYER(play);

    FUN_00370734(&self->skelAnime);

    switch (self->unk_76C) {
        case 0:
            self->unk_76C = 5;
            FUN_00367494(play, &play->csCtx);
            FUN_0036e980(play, &self->actor, 1);
            self->unk_784 = Gameplay_CreateSubCamera(play);
            Gameplay_ChangeCameraStatus(play, 0, CAM_STAT_UNK3);
            Gameplay_ChangeCameraStatus(play, self->unk_784, CAM_STAT_ACTIVE);
            mainCam = Gameplay_GetCamera(play, 0);
            self->unk_9F8 = mainCam->eye.x;
            self->unk_9FC = mainCam->eye.y;
            self->unk_A00 = mainCam->eye.z;
            self->unk_A04 = mainCam->at.x;
            self->unk_A08 = mainCam->at.y;
            self->unk_A0C = mainCam->at.z;
            break;
        case 5:
            tempSin = Math_SinS(self->actor.shape.rot.y - 0x1388) * 150.0f;
            tempCos = Math_CosS(self->actor.shape.rot.y - 0x1388) * 150.0f;
            Math_SmoothStepToF(&player->actor.world.pos.x, self->actor.world.pos.x + tempSin, 0.5f, 5.0f, 0.0f);
            Math_SmoothStepToF(&player->actor.world.pos.z, self->actor.world.pos.z + tempCos, 0.5f, 5.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_7D8, 0.07f, 1.0f, 0.005f, 0.0f);
            tempSin = Math_SinS(self->actor.world.rot.y) * 230.0f;
            tempCos = Math_CosS(self->actor.world.rot.y) * 230.0f;
            Math_SmoothStepToF(&self->unk_9F8, self->actor.world.pos.x + tempSin, 0.2f, 50.0f, 0.1f);
            Math_SmoothStepToF(&self->unk_9FC, self->actor.world.pos.y + 20.0f, 0.2f, 50.0f, 0.1f);
            Math_SmoothStepToF(&self->unk_A00, self->actor.world.pos.z + tempCos, 0.2f, 50.0f, 0.1f);
            Math_SmoothStepToF(&self->unk_A04, self->actor.world.pos.x, 0.2f, 30.0f, 0.1f);
            Math_SmoothStepToF(&self->unk_A08, self->actor.focus.pos.y - 70.0f, 0.2f, 30.0f, 0.1f);
            Math_SmoothStepToF(&self->unk_A0C, self->actor.world.pos.z, 0.2f, 30.0f, 0.1f);
            if (FUN_0036e5e0(&self->skelAnime,Animation_GetLastFrame(&self->skelAnime,BossDodongo_AnimTable[12]),1.0f)) {
                FUN_00353020(&self->skelAnime, &BossDodongo_AnimTable[13], 1.0f, 0.0f,
                                 Animation_GetLastFrame(&self->skelAnime,BossDodongo_AnimTable[13]), 2, -1.0f);
                self->unk_76C = 6;
                Actor_Spawn(&play->actorCtx, play, ACTOR_BG_BREAKWALL, -890.0f, -1523.76f, -3304.0f, 0, 0, 0, 0x6000,1);
            }
            break;
        case 6:
            Math_SmoothStepToF(&self->unk_A04, self->actor.world.pos.x, 0.2f, 30.0f, 0.1f);
            Math_SmoothStepToF(&self->unk_A08, (self->actor.world.pos.y - 70.0f) + 130.0f, 0.2f, 20.0f, 0.1f);
            Math_SmoothStepToF(&self->unk_A0C, self->actor.world.pos.z, 0.2f, 30.0f, 0.1f);

            if (FUN_0036e5e0(&self->skelAnime,Animation_GetLastFrame(&self->skelAnime,BossDodongo_AnimTable[13]),1.0f)) {
                FUN_00353020(&self->skelAnime, &BossDodongo_AnimTable[6], 1.0f, 30.0f, 59.0f, 2,
                                 -1.0f);
                self->unk_76C = 7;
                self->unk_7F8 = 7700.0f;
                self->unk_7D4 = 0.0f;
                self->unk_7B4 = 0.0f;
                self->unk_778 = 0;
                self->unk_76E = 0;
            }
            break;
        case 7:
            self->unk_794 += 0x7D0;
            Math_SmoothStepToF(&self->unk_A04, self->actor.world.pos.x, 0.2f, 30.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_A08, (self->actor.world.pos.y - 70.0f) + 130.0f, 0.2f, 20.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_A0C, self->actor.world.pos.z, 0.2f, 30.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_9F8, -890.0f, 0.1f, self->unk_7D4 * 5.0f, 0.1f);
            Math_SmoothStepToF(&self->unk_A00, -3304.0f, 0.1f, self->unk_7D4 * 5.0f, 0.1f);
            Math_SmoothStepToF(&self->unk_7D4, 1.0f, 1.0f, 0.1f, 0.0f);
            if (self->unk_7AA == 1) {
                self->unk_76C = 8;
                self->actor.speedXZ = self->unk_7B4 / 1.5f;
                if (self->unk_772 == 0) {
                    self->unk_808 = 250.0f;
                } else {
                    self->unk_808 = -250.0f;
                }
                self->unk_7AA = 1000;
                self->unk_804 = 2000.0f;
            } else {
                cornerPos = &BossDodongo_Corners[self->unk_770];
                self->unk_7BC = 3.0f;
                Math_SmoothStepToF(&self->unk_7B4, self->unk_7BC * 5.0f, 1.0f, self->unk_7BC * 0.25f, 0.0f);
                tempSin = cornerPos->x - self->actor.world.pos.x;
                tempCos = cornerPos->z - self->actor.world.pos.z;
                sp178 = sqrtf(SQ(tempSin) + SQ(tempCos)) - 200.0f;
                if ((sqrtf(SQ(tempSin) + SQ(tempCos)) < 200.0f) || (self->unk_7AA != 0)) {
                    sp178 = 0.0f;
                }
                sp178 = CLAMP_MAX(sp178, 70.0f);
                self->unk_80C = (Math_SinS(self->unk_76E * 1000) * -50.0f) / 100.0f;

                sp198.x = Math_SinS(self->unk_76E * 1000) * sp178;
                sp198.y = sp198.z = 0.0f;

                { MTX34 m; f32 angle=BINANG_TO_RAD(self->actor.shape.rot.y); f32 sine=sinf(angle),cosine=cosf(angle);
                  m.data[0][0]=cosine;m.data[0][1]=0;m.data[0][2]=sine;m.data[0][3]=0;
                  m.data[1][0]=0;m.data[1][1]=1;m.data[1][2]=0;m.data[1][3]=0;
                  m.data[2][0]=-sine;m.data[2][1]=0;m.data[2][2]=cosine;m.data[2][3]=0;
                  VEC3TransformAsm((Vector3f*)&sp184,&m,(Vector3f*)&sp198); }

                Math_SmoothStepToF(&self->actor.world.pos.x, cornerPos->x + sp184.x, 1.0f, self->unk_7B4, 0.0f);
                Math_SmoothStepToF(&self->actor.world.pos.z, cornerPos->z + sp184.z, 1.0f, self->unk_7B4, 0.0f);
                Audio_PlayActorSound2(&self->actor, 0x010002BE);
                if ((self->unk_76E & 7) == 0) {
                    FUN_0036627c((u8*)play+0x364, 2, 1, 8);
                }
                if (!(self->unk_76E & 1)) {
                    FUN_0036f00c(play, &self->actor, &self->actor.world.pos, 40.0f, 3, 8.0f, 500, 10,
                                             0);
                }
                tempSin = cornerPos->x - self->actor.world.pos.x;
                tempCos = cornerPos->z - self->actor.world.pos.z;
                Math_SmoothStepToF(&self->unk_7B8, 1500.0f, 1.0f, self->unk_7BC * 100.0f, 0.0f);
                Math_SmoothStepToS(&self->actor.world.rot.y, RAD_TO_BINANG(Math_FAtan2F(tempSin, tempCos)), 5,
                                   (self->unk_7BC * self->unk_7B8), 0);

                if ((fabsf(tempSin) <= 15.0f) && (fabsf(tempCos) <= 15.0f)) {
                    Vec3f dustPos;

                    self->actor.velocity.y = 15.0f;
                    Audio_PlayActorSound2(&self->actor, 0x010002BD);
                    if (self->unk_772 == 0) {
                        self->unk_770++;
                        if (self->unk_770 >= 4) {
                            self->unk_770 = 0;
                        }
                    } else {
                        self->unk_770--;
                        if (self->unk_770 < 0) {
                            self->unk_770 = 3;
                        }
                    }
                    self->unk_7AA = 0xA;
                    dustPos.x = self->actor.world.pos.x;
                    dustPos.y = self->actor.world.pos.y + 60.0f;
                    dustPos.z = self->actor.world.pos.z;
                    FUN_0037378c(play, &dustPos, 250.0f, 0x28, 0x320, 0xA, 0);
                }
            }
            break;
        case 8:
        case 9:
            if (self->unk_7AA == 884) {
                FUN_00353020(&self->skelAnime, &BossDodongo_AnimTable[9], 1.0f, 0.0f,
                                 (f32)Animation_GetLastFrame(&self->skelAnime,BossDodongo_AnimTable[9]), 0, -20.0f);
                tempSin = self->unk_9F8 - self->actor.world.pos.x;
                tempCos = self->unk_A00 - self->actor.world.pos.z;
                self->unk_7FC = sqrtf(SQ(tempSin) + SQ(tempCos));
                self->unk_800 = Math_FAtan2F(tempSin, tempCos);
                self->unk_7AC = 350;
                self->unk_76C = 9;
            }
            if (self->unk_7AA < 854) {
                for (i = 0; i < 2; i++) {
                    FUN_00357a50(BossDodongo_DissolveTexture, self->unk_79C);
                    if (self->unk_79C < 256) {
                        self->unk_79C++;
                    }
                }
            }
            if (self->unk_7AA < 984) {
                Math_SmoothStepToS(&self->unk_794, -0x4000, 0xA, 0x12C, 0);
            }
            if (self->unk_7AA == 904) {
                Audio_PlayActorSound2(&self->actor, 0x010002C5);
            }
            if (self->unk_7AA < 854) {
                Audio_PlayActorSound2(&self->actor, 0x010002C4);
            }
            if (self->unk_7AA == 960) {
                Audio_PlayActorSound2(&self->actor, 0x010002C3);
            }
            if (self->unk_7AA < 960) {
                Math_SmoothStepToF(&self->actor.shape.shadowScale, 0.0f, 1.0f, 10.0f, 0.0f);
                if (self->unk_7AA >= 710) {

                    if (self->unk_7AA == 710) {
                        Vec3f sp124[] = {
                            { -440.0f, 0.0f, -3304.0f },
                            { -890.0f, 0.0f, -3754.0f },
                            { -1340.0f, 0.0f, -3304.0f },
                            { -890.0f, 0.0f, -2854.0f },
                        };
                        Vec3f spF4[] = {
                            { -890.0f, 0.0f, -2854.0f },
                            { -440.0f, 0.0f, -3304.0f },
                            { -890.0f, 0.0f, -3754.0f },
                            { -1340.0f, 0.0f, -3304.0f },
                        };
                        Vec3f* phi_v0_2;

                        self->unk_796 = 1;
                        if (self->unk_772 == 0) {
                            phi_v0_2 = &sp124[self->unk_770];
                        } else {
                            phi_v0_2 = &spF4[self->unk_770];
                        }
                        player->actor.world.pos.x = phi_v0_2->x;
                        player->actor.world.pos.z = phi_v0_2->z;
                        self->unk_7D4 = 0.0f;
                    }
                    if (self->unk_7AA >= 885) {
                        Math_SmoothStepToF(&self->unk_7F8, 200.0, 0.2f, 100.0f, 0.0f);
                    } else {
                        Math_SmoothStepToF(&self->unk_7F8, -6600.0f, 0.2f, 30.0f, 0.0f);
                    }
                    {
                        static Vector3f dustVel(0.0f,0.0f,0.0f);
                        static Vector3f dustAcell(0.0f,1.0f,0.0f);
                        static Color_RGBA8 dustPrimColor = { 255, 255, 100, 255 };
                        static Color_RGBA8 dustEnvColor = { 255, 100, 0, 255 };
                        s16 colorIndex;
                        Color_RGBA8 magmaPrimColor2[] = { { 255, 255, 0, 255 }, { 0, 0, 0, 100 } };
                        Color_RGBA8 magmaEnvColor2[] = { { 255, 0, 0, 255 }, { 0, 0, 0, 0 } };

                        effectPos.x = Rand_CenteredFloat(120.0f) + self->actor.focus.pos.x;
                        effectPos.y = Rand_ZeroFloat(50.0f) + self->actor.world.pos.y;
                        effectPos.z = Rand_CenteredFloat(120.0f) + self->actor.focus.pos.z;
                        FUN_00365d20(play, &effectPos, (Vec3f*)&dustVel, (Vec3f*)&dustAcell, &dustPrimColor, &dustEnvColor, 0x1F4, 0xA,
                                      0xA);
                        effectPos.x = Rand_CenteredFloat(120.0f) + self->actor.focus.pos.x;
                        effectPos.y = -1498.76f;
                        effectPos.z = Rand_CenteredFloat(120.0f) + self->actor.focus.pos.z;
                        colorIndex = (Rand_ZeroOne() * 1.9f);
                        FUN_003373c8(play, &effectPos, &magmaPrimColor2[colorIndex],
                                              &magmaEnvColor2[colorIndex], 10 - (colorIndex * 5), colorIndex,
                                              (s16)(Rand_ZeroOne() * 100.0f) + 100);
                    }
                }
            } else {
                Audio_PlayActorSound2(&self->actor, 0x010002BE);
                if (!(self->unk_76E & 1)) {
                    FUN_0036f00c(play, &self->actor, &self->actor.world.pos, 40.0f, 3, 8.0f, 500, 10,
                                             0);
                }
            }
            Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.2f, 0.1f, 0.0f);
            self->actor.world.rot.y += (s16)self->unk_808;
            self->unk_794 += (s16)self->unk_804;
            if (self->unk_7AA >= 0x367) {
                if (self->unk_772 == 0) {
                    if (self->unk_808 < 450.0f) {
                        self->unk_808 += 10.0f;
                    }
                } else if (-450.0f < self->unk_808) {
                    self->unk_808 -= 10.0f;
                }
            } else {
                Math_SmoothStepToF(&self->unk_808, 0.0f, 0.05f, 40.0f, 0.0f);
            }
            Math_SmoothStepToF(&self->unk_804, 0.0f, 0.2f, 17.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_A04, self->actor.world.pos.x, 0.2f, 30.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_A08, (self->actor.world.pos.y - 70.0f) + 130.0f, 0.2f, 20.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_A0C, self->actor.world.pos.z, 0.2f, 30.0f, 0.0f);
            if (self->unk_76C == 9) {
                if (self->unk_7AA < 0x2C6) {
                    Vec3f spAC[] = { { -390.0f, 0.0f, -3304.0f },
                                     { -890.0f, 0.0f, -3804.0f },
                                     { -1390.0f, 0.0f, -3304.0f },
                                     { -890.0f, 0.0f, -2804.0f } };

                    Vec3f sp7C[] = { { -890.0f, 0.0f, -2804.0f },
                                     { -390.0f, 0.0f, -3304.0f },
                                     { -890.0f, 0.0f, -3804.0f },
                                     { -1390.0f, 0.0f, -3304.0f } };
                    Vec3f* sp78;
                    STACK_PAD(s32);

                    if (self->unk_772 == 0) {
                        sp78 = &spAC[self->unk_770];
                    } else {
                        sp78 = &sp7C[self->unk_770];
                    }

                    Math_SmoothStepToF(&self->unk_9F8, sp78->x, 0.2f, self->unk_7D4 * 20.0f, 0.0f);
                    Math_SmoothStepToF(&self->unk_9FC, player->actor.world.pos.y + 30.0f, 0.1f,
                                       self->unk_7D4 * 20.0f, 0.0f);
                    Math_SmoothStepToF(&self->unk_A00, sp78->z, 0.1f, self->unk_7D4 * 20.0f, 0.0f);
                    Math_SmoothStepToF(&self->unk_7D4, 1.0f, 1.0f, 0.02f, 0.0f);
                } else {
                    if (self->unk_772 == 0) {
                        self->unk_800 += 0.01f;
                    } else {
                        self->unk_800 -= 0.01f;
                    }
                    Math_SmoothStepToF(&self->unk_7FC, 220.0f, 0.1f, 5.0f, 0.1f);
                    tempSin = sinf(self->unk_800) * self->unk_7FC;
                    tempCos = cosf(self->unk_800) * self->unk_7FC;
                    Math_SmoothStepToF(&self->unk_9F8, self->actor.world.pos.x + tempSin, 0.2f, 50.0f, 0.0f);
                    Math_SmoothStepToF(&self->unk_9FC, self->actor.world.pos.y + 20.0f, 0.2f, 50.0f, 0.0f);
                    Math_SmoothStepToF(&self->unk_A00, self->actor.world.pos.z + tempCos, 0.2f, 50.0f, 0.0f);
                    Math_SmoothStepToF(&self->unk_80C, 0.0f, 0.2f, 0.01f, 0.0f);
                }
            } else {

                if (self->unk_772 == 0) {
                    Math_SmoothStepToF(&self->unk_80C, -0.5f, 0.2f, 0.05f, 0.0f);
                } else {
                    Math_SmoothStepToF(&self->unk_80C, 0.5f, 0.2f, 0.05f, 0.0f);
                }

                Math_SmoothStepToF(&self->unk_9F8, -890.0f, 0.1f, self->unk_7D4 * 5.0f, 0.1f);
                Math_SmoothStepToF(&self->unk_A00, -3304.0f, 0.1f, self->unk_7D4 * 5.0f, 0.1f);
                Math_SmoothStepToF(&self->unk_7D4, 1.0f, 1.0f, 0.05f, 0.0f);
            }

            if (self->unk_7AA == 820) {
                Audio_QueueSeqCmd(0,0x0100058F,0);
                Actor_Spawn(&play->actorCtx, play, ACTOR_ITEM_B_HEART,
                            Math_SinS(self->actor.shape.rot.y) * -50.0f + self->actor.world.pos.x,
                            self->actor.world.pos.y,
                            Math_CosS(self->actor.shape.rot.y) * -50.0f + self->actor.world.pos.z, 0, 0, 0, 0,1);
            }
            if (self->unk_7AA == 600) {
                mainCam = Gameplay_GetCamera(play, 0);
                mainCam->eye = (*(Vec3f*)&self->unk_9F8);
                mainCam->eyeNext = (*(Vec3f*)&self->unk_9F8);
                mainCam->at = (*(Vec3f*)&self->unk_A04);
                FUN_0036e9b8(play, self->unk_784, 0);
                self->unk_78C = 0;
                self->unk_784 = 0;
                self->unk_76C = 100;
                Gameplay_ChangeCameraStatus(play, 0, CAM_STAT_ACTIVE);
                FUN_00367374(play, &play->csCtx);
                FUN_0036e980(play, &self->actor, 7);
                Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, ACTOR_DOOR_WARP1, -890.0f, -1523.76f, -3304.0f,
                                   0, 0, 0, 0);
                self->skelAnime.playSpeed = 0.0f;
                Flags_SetClear(play, (*(s8*)((u8*)play+0x4C30)));
            }

        case 100:
            if ((self->unk_7AA < 0x2C6) && (Rand_ZeroOne() < 0.5f)) {
                Vec3f sp68;
                Color_RGBA8 D_808CA568 = { 0, 0, 0, 100 };
                Color_RGBA8 D_808CA56C = { 0, 0, 0, 0 };

                sp68.x = Rand_CenteredFloat(60.0f) + self->actor.focus.pos.x;
                sp68.y = (Rand_ZeroOne() * 50.0f) + -1498.76f;
                sp68.z = Rand_CenteredFloat(60.0f) + self->actor.focus.pos.z;
                FUN_003373c8(play, &sp68, &D_808CA568, &D_808CA56C, 5, 1, (s16)(Rand_ZeroOne() * 50.0f) + 50);
            }
            break;
    }
    if (self->unk_784 != 0) {
        Gameplay_CameraSetAtEye(play, self->unk_784, &(*(Vec3f*)&self->unk_A04), &(*(Vec3f*)&self->unk_9F8));
    }
}
}

#endif
