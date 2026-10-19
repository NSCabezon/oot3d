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
            eff->alpha-=(s32)((20.0f*gGameInfo->data[126])/3.0f+0.5f);
            if(eff->alpha<=0) {eff->alpha=0;eff->active=0;}
        }
    }
}
void BossDodongo_Update(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    BossDodongo* self = (BossDodongo*)thisx;
    f32 temp_f0;
    s16 i;
    STACK_PAD(s32);

    thisx->targetArrowOffset=8500.0f-((self->actor.focus.pos.y-self->actor.world.pos.y)*100.0f)*0.4f;
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
    Actor_UpdateBgCheckInfo(play, thisx, 10.0f, 10.0f, 20.0f, 4);
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
        if (self->unk_798 > (s32)(30.0f/(f32)gGameInfo->data[126]+0.5f)) {
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

    Player* player=GET_PLAYER(play);
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

        if ((*(u8*)((u8*)GET_PLAYER(play)+0x12BC)) >= 10) {
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
            sp84.y = -1520.0f;
            sp84.z = (cosf(temp_f10) * temp_f12) + (-3304.0f);
            FUN_003373c8(play, &sp84, &magmaPrimColor[magma2DrawMode], &magmaEnvColor[magma2DrawMode],
                                  10 - (magma2DrawMode * 5), magma2DrawMode, magmaScale + 100,magma2DrawMode);
        }

        if ((self->unk_76E & sp90) == 0) {
            Vec3f sp6C = { 0.0f, 0.0f, 0.0f };
            Vec3f sp60 = { 0.0f, 0.0f, 0.0f };
            Vec3f sp54;
            f32 sp50 = Rand_ZeroOne() * 330.0f;
            f32 sp4C = Rand_ZeroOne() * 6.28f;

            sp54.x = sinf(sp4C) * sp50 + (-890.0f);
            sp54.y = -1520.0f;
            sp54.z = cosf(sp4C) * sp50 + (-3304.0f);
            FUN_0037100c(play, &sp54);
            for (i = 0; i < 4; i++) {
                sp60.y = 0.4f;
                sp60.x = Rand_CenteredFloat(0.5f);
                sp60.z = Rand_CenteredFloat(0.5f);
                sp50 = Rand_ZeroOne() * 330.0f;
                sp4C = Rand_ZeroOne() * 6.28f;
                sp54.x = sinf(sp4C) * sp50 + (-890.0f);
                sp54.y = -1510.0f;
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

    for (i = 0; i < 13; i++) {
        if (i == 6) {
            continue;
        }
        (*(f32*)(*(u8**)((u8*)self+0xA2C)+(i+6)*0x50+0x48)) = (self->actionFunc == FUN_001ccc78) ? 0.0f : 1.0f;
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
}

#endif
