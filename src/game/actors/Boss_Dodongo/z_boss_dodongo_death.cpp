/* Unmatched native 3DS candidate. Original ASM remains active. */
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
void FUN_00181770(BossDodongo*);
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
void FUN_003373c8(PlayState*,Vec3f*,Color_RGBA8*,Color_RGBA8*,s16,s16,s16,s16);
void FUN_00357a50(SkelAnime*,s32,s32,void*,s32);
s16 FUN_00257108(s16*,s16,s16,s16,s16,s32);
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
    sp2C = rotation * xDiff;
    sp2C += Math_SinS(-self->actor.world.rot.y) * zDiff;
    rotation = Math_CosS(-self->actor.world.rot.y);
    temp_f2 = rotation * zDiff;
    temp_f2 -= Math_SinS(-self->actor.world.rot.y) * xDiff;

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
    sp2C = rotation * xDiff;
    sp2C += Math_SinS(-0x8000 - self->actor.world.rot.y) * zDiff;
    rotation = Math_CosS(-0x8000 - self->actor.world.rot.y);
    temp_f2 = rotation * zDiff;
    temp_f2 -= Math_SinS(-0x8000 - self->actor.world.rot.y) * xDiff;

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

    static Vector3f dustVel(0.0f,0.0f,0.0f);
    static Vector3f dustAcell(0.0f,1.0f,0.0f);
    FUN_00370734(&self->skelAnime);
    f32 deathLastFrame=(f32)Animation_GetLastFrame(&self->skelAnime,BossDodongo_AnimTable[12]);
    f32 fallLastFrame=(f32)Animation_GetLastFrame(&self->skelAnime,BossDodongo_AnimTable[13]);

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
            self->unk_766=10;
            self->unk_768=0;
            self->unk_7A4=self->actor.xzDistToPlayer<100.0f ? 1:0;
            break;
        case 5:
            tempSin = Math_SinS(self->actor.shape.rot.y - 0x1388) * 150.0f;
            tempCos = Math_CosS(self->actor.shape.rot.y - 0x1388) * 150.0f;
            self->unk_818=tempSin; self->unk_81C=tempCos;
            Math_SmoothStepToF(&self->unk_7D8, 0.07f, 1.0f, 0.005f, 0.0f);
            tempSin = Math_SinS(self->actor.world.rot.y) * 230.0f;
            tempCos = Math_CosS(self->actor.world.rot.y) * 230.0f;
            Math_SmoothStepToF(&self->unk_9F8, self->actor.world.pos.x + tempSin, 0.2f, 50.0f, 0.1f);
            Math_SmoothStepToF(&self->unk_9FC, self->actor.world.pos.y + 20.0f, 0.2f, 50.0f, 0.1f);
            Math_SmoothStepToF(&self->unk_A00, self->actor.world.pos.z + tempCos, 0.2f, 50.0f, 0.1f);
            Math_SmoothStepToF(&self->unk_A04, self->actor.world.pos.x, 0.2f, 30.0f, 0.1f);
            Math_SmoothStepToF(&self->unk_A08, self->actor.focus.pos.y - 70.0f, 0.2f, 30.0f, 0.1f);
            Math_SmoothStepToF(&self->unk_A0C, self->actor.world.pos.z, 0.2f, 30.0f, 0.1f);
            if(self->unk_7A4==1) FUN_00315350(self,play,60,self->unk_818,self->unk_81C);
            if (FUN_0036e5e0(&self->skelAnime,deathLastFrame,2.0f/3.0f)) {
                FUN_00353020(&self->skelAnime, &BossDodongo_AnimTable[13], 1.0f, 0.0f,
                                 deathLastFrame, 2, -1.0f);
                self->unk_76C = 6;
                Actor_Spawn(&play->actorCtx, play, ACTOR_BG_BREAKWALL, -890.0f, -1520.0f, -3304.0f, 0, 0, 0, 0x6000,1);
            }
            break;
        case 6:
            FUN_00315350(self,play,60,self->unk_818,self->unk_81C);
            Math_SmoothStepToF(&self->unk_A04, self->actor.world.pos.x, 0.2f, 30.0f, 0.1f);
            Math_SmoothStepToF(&self->unk_A08, (self->actor.world.pos.y - 70.0f) + 130.0f, 0.2f, 20.0f, 0.1f);
            Math_SmoothStepToF(&self->unk_A0C, self->actor.world.pos.z, 0.2f, 30.0f, 0.1f);

            if(self->skelAnime.curFrame>fallLastFrame-10.0f) self->unk_794+=(s16)(190.0f*(10.0f-(fallLastFrame-self->skelAnime.curFrame)));
            if (FUN_0036e5e0(&self->skelAnime,fallLastFrame-3.0f,2.0f/3.0f)) {
                FUN_00353020(&self->skelAnime, &BossDodongo_AnimTable[6], 1.0f, 30.0f, 59.0f, 2,
                                 0.0f);
                self->unk_76C = 7;
                self->unk_7F8 = 7700.0f;
                self->unk_7D4 = 0.0f;
                self->unk_7B4 = 0.0f;
                self->unk_778 = 0;
                self->unk_76E = 0;
            }
            break;
        case 7:
            FUN_00315350(self,play,60,self->unk_818,self->unk_81C);
            self->unk_794 += 0x7D0;
            Math_SmoothStepToF(&self->unk_A04, self->actor.world.pos.x, 0.2f, 30.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_A08, (self->actor.world.pos.y - 70.0f) + 130.0f, 0.2f, 20.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_A0C, self->actor.world.pos.z, 0.2f, 30.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_9F8, -890.0f, 0.1f, self->unk_7D4 * 5.0f, 0.1f);
            Math_SmoothStepToF(&self->unk_A00, -3304.0f, 0.1f, self->unk_7D4 * 5.0f, 0.1f);
            Math_SmoothStepToF(&self->unk_7D4, 1.0f, 1.0f, 0.1f, 0.0f);
            if (self->unk_7AA == 1) {
                self->unk_76C = 8;
                self->actor.speedXZ = self->unk_7B4 * (2.0f/3.0f);
                if (self->unk_772 == 0) {
                    self->unk_808 = 166.6666717529296875f;
                } else {
                    self->unk_808 = -166.6666717529296875f;
                }
                self->unk_7AA = 1500;
                self->unk_794=0;
                self->unk_804 = 1333.3333740234375f;
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
            if (self->unk_7AA == 1381) {
                s32 deathAnim=BossDodongo_AnimTable[14];
                FUN_00353020(&self->skelAnime,&deathAnim,1.0f,0.0f,(f32)Animation_GetLastFrame(&self->skelAnime,deathAnim),2,10.0f);
                tempSin = self->unk_9F8 - self->actor.world.pos.x;
                tempCos = self->unk_A00 - self->actor.world.pos.z;
                self->unk_7FC = sqrtf(SQ(tempSin) + SQ(tempCos));
                self->unk_800 = Math_FAtan2F(tempSin, tempCos);
                self->unk_7AC = 525;
                self->unk_76C = 9;
            }
            if(self->unk_7AA<1350) {
                f32 fadeColor[4]={1.0f,1.0f,1.0f,1.0f};
                fadeColor[3]=1.0f-(1350-self->unk_7AA)*(1.0f/60.0f);
                if(fadeColor[3]<=0.0f) fadeColor[3]=0.0f;
                FUN_00357a50(&self->skelAnime,0,4,fadeColor,2);
                FUN_00357a50(&self->skelAnime,1,4,fadeColor,2);
                FUN_00357a50(&self->skelAnime,2,4,fadeColor,2);
            }

            if (self->unk_7AA == 1440) {
                Audio_PlayActorSound2(&self->actor, 0x010002C5);
            }
            if (self->unk_7AA <= 1280) {
                Audio_PlayActorSound2(&self->actor, 0x010002C4);
            }
            if (self->unk_7AA == 1356) {
                Audio_PlayActorSound2(&self->actor, 0x010002C3);
            }
            if (self->unk_7AA < 1440) {
                Math_SmoothStepToF(&self->actor.shape.shadowScale, 0.0f, 1.0f, 10.0f, 0.0f);
                if (self->unk_7AA >= 1065) {

                    if (self->unk_7AA == 1065) {
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
                    if (self->unk_7AA >= 1327) {
                        Math_SmoothStepToF(&self->unk_7F8, 200.0, 0.2f, 100.0f, 0.0f);
                    } else {
                        Math_SmoothStepToF(&self->unk_7F8, -6600.0f, 0.2f, 30.0f, 0.0f);
                    }
                    {
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
                        effectPos.y = -1495.0f;
                        effectPos.z = Rand_CenteredFloat(120.0f) + self->actor.focus.pos.z;
                        colorIndex = (Rand_ZeroOne() * 1.9f);
                        FUN_003373c8(play, &effectPos, &magmaPrimColor2[colorIndex],
                                              &magmaEnvColor2[colorIndex], 10 - (colorIndex * 5), colorIndex,
                                              (s16)(Rand_ZeroOne() * 100.0f) + 100,colorIndex);
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
            if(self->unk_7AA<1526) FUN_00257108(&self->unk_794,0,10,1000,0,1);
            self->unk_794 += (s16)self->unk_804;
            if (self->unk_7AA > 1305) {
                if (self->unk_772 == 0) {
                    if (self->unk_808 < 300.0f) {
                        self->unk_808 += 6.666666507720947f;
                    }
                } else if (-300.0f < self->unk_808) {
                    self->unk_808 -= 6.666666507720947f;
                }
            } else {
                Math_SmoothStepToF(&self->unk_808, 0.0f, 0.05f, 27.0f, 0.0f);
            }
            Math_SmoothStepToF(&self->unk_804, 0.0f, 0.2f, 21.333332061767578f, 0.0f);
            Math_SmoothStepToF(&self->unk_A04, self->actor.world.pos.x, 0.2f, 30.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_A08, (self->actor.world.pos.y - 70.0f) + 130.0f, 0.2f, 20.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_A0C, self->actor.world.pos.z, 0.2f, 30.0f, 0.0f);
            if (self->unk_76C == 9) {
                if (self->unk_7AA < 1065) {
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
                    Math_SmoothStepToF(&self->unk_9FC, self->actor.world.pos.y + 40.0f, 0.2f, 50.0f, 0.0f);
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

            if (self->unk_7AA == 1230) {
                Audio_QueueSeqCmd(0,0x0100058F,0);
                if (FUN_0035b164()!=1) {
                    f32 z=Math_CosS(self->actor.shape.rot.y)*-50.0f+self->actor.world.pos.z;
                    f32 x=Math_SinS(self->actor.shape.rot.y)*-50.0f+self->actor.world.pos.x;
                    Actor_Spawn(&play->actorCtx,play,ACTOR_ITEM_B_HEART,x,self->actor.world.pos.y,z,0,0,0,0,1);
                } else if (FUN_0035b0a0()!=0) {
                    s16 yaw=Math_Atan2S(self->actor.world.pos.z-(-3304.0f),self->actor.world.pos.x-(-890.0f));
                    s16 left=yaw+0x8C00;
                    f32 x1=-890.0f+Math_SinS(left)*200.0f;
                    f32 z1=-3304.0f+Math_CosS(left)*200.0f;
                    s16 right=yaw+0x7400;
                    f32 x2=-890.0f+Math_SinS(right)*200.0f;
                    f32 z2=-3304.0f+Math_CosS(right)*200.0f;
                    FUN_0035af20(play,1,(u16)(yaw+0x8000),(u16)(yaw+0x8000),x1,-1523.0f,z1,x2,-1523.0f,z2);
                }
            }
            if (self->unk_7AA == 900) {
                mainCam = Gameplay_GetCamera(play, 0);
                mainCam->eye = (*(Vec3f*)&self->unk_9F8);
                mainCam->eyeNext = (*(Vec3f*)&self->unk_9F8);
                mainCam->at = (*(Vec3f*)&self->unk_A04);
                FUN_0036e9b8(play, self->unk_784, 0);
                self->unk_78C = 0;
                self->unk_784 = 0;
                self->unk_76C = 100;
                Gameplay_ChangeCameraStatus(play, 0, CAM_STAT_ACTIVE);
                FUN_0035af04(player,0);
                FUN_00367374(play, &play->csCtx);
                FUN_0036e980(play, &self->actor, 7);
                Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, ACTOR_DOOR_WARP1, -890.0f, -1520.0f, -3304.0f,
                                   0, 0, 0, 0);
                self->skelAnime.playSpeed = 0.0f;
                Flags_SetClear(play, (*(s8*)((u8*)play+0x4C30)));
            }

        case 100:
            if ((self->unk_7AA < 1065) && (Rand_ZeroOne() < 0.5f)) {
                Vec3f sp68;
                Color_RGBA8 D_808CA568 = { 0, 0, 0, 100 };
                Color_RGBA8 D_808CA56C = { 0, 0, 0, 0 };

                sp68.x = Rand_CenteredFloat(60.0f) + self->actor.focus.pos.x;
                sp68.y = (Rand_ZeroOne() * 50.0f) + -1495.0f;
                sp68.z = Rand_CenteredFloat(60.0f) + self->actor.focus.pos.z;
                FUN_003373c8(play, &sp68, &D_808CA568, &D_808CA56C, 5, 1, (s16)(Rand_ZeroOne() * 50.0f) + 50,1);
            }
            break;
    }
    if (self->unk_784 != 0) {
        Gameplay_CameraSetAtEye(play, self->unk_784, &(*(Vec3f*)&self->unk_A04), &(*(Vec3f*)&self->unk_9F8));
    }
}
}

#endif
