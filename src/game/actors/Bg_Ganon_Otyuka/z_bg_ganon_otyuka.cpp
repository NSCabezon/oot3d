extern "C" {
#include "z_bg_ganon_otyuka.hpp"

#define FLAGS 0x80400030

void BgGanonOtyuka_Init(Actor* thisx, PlayState* play2);
void BgGanonOtyuka_Destroy(Actor* thisx, PlayState* play2);
void BgGanonOtyuka_Update(Actor* thisx, PlayState* play);
void BgGanonOtyuka_Draw(Actor* actor, GameState* state);
void BgGanonOtyuka_DoNothing(Actor* thisx, PlayState* play);
void BgGanonOtyuka_WaitForFall(BgGanonOtyuka* self, PlayState* play);
void FUN_00394828(Actor* thisx, PlayState* play);

GLOBAL_ASM("data/z_Bg_Ganon_Otyuka.data.s")
GLOBAL_ASM("../src/game/actors/Bg_Ganon_Otyuka/data_syms.s")

extern InitChainEntry Bg_Ganon_Otyuka_sInitChain[];
extern u8 Bg_Ganon_Otyuka_sSides[];
extern Vec3f Bg_Ganon_Otyuka_sPosOffsets[];
extern Color_RGBA8 Bg_Ganon_Otyuka_sDustPrimColor;
extern Color_RGBA8 Bg_Ganon_Otyuka_sDustEnvColor;

ActorInit Bg_Ganon_Otyuka_InitVars = {
    ACTOR_BG_GANON_OTYUKA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GANON,
    sizeof(BgGanonOtyuka),
    (ActorFunc)BgGanonOtyuka_Init,
    (ActorFunc)BgGanonOtyuka_Destroy,
    (ActorFunc)BgGanonOtyuka_Update,
    (ActorFunc)BgGanonOtyuka_Draw,
};

// Init: near-miss, 19 words (register numbering of thisx/play/zar; decl perms and single-site
// mutations tried). Draft kept under #if 0; ROM stays asm until solved.
#if 0
void BgGanonOtyuka_Init(Actor* thisx, PlayState* play2) {
    BgGanonOtyuka* self = (BgGanonOtyuka*)thisx;
    PlayState* play = play2;
    CollisionHeader* colHeader = NULL;
    ZARInfo* zar;
    s32 i;

    Actor_ProcessInitChain(thisx, Bg_Ganon_Otyuka_sInitChain);
    DynaPolyActor_Init(&self->dyna, 0);
    zar = FUN_00372f38(thisx, play, &self->skelAnimModel[0], 1, &self->skelAnimModel[1], 0, &self->skelAnimModel[2], 0,
                       &self->skelAnimModel[3], 0, &self->skelAnimModel[4], 0, 0);
    for (i = 0; i < 4; i++) {
        SkeletonAnimationModel_unk_0C* anim = self->skelAnimModel[i + 1]->unk_0C;

        anim->FUN_00372d94(zar->GetCMABByIndex(2));
        self->skelAnimModel[i + 1]->unk_0C->animType = 1;
    }
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);

    if (thisx->params != 0x23) {
        thisx->draw = (ActorFunc)FUN_00394828;
        self->actionFunc = BgGanonOtyuka_WaitForFall;
    } else {
        thisx->update = (ActorFunc)BgGanonOtyuka_DoNothing;
    }
}
#endif

GLOBAL_ASM("asm/BgGanonOtyuka_Init.s")

void BgGanonOtyuka_Destroy(Actor* thisx, PlayState* play2) {
    BgGanonOtyuka* self = (BgGanonOtyuka*)thisx;
    PlayState* play = play2;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(thisx, &self->skelAnimModel[0], &self->skelAnimModel[1], &self->skelAnimModel[2],
                 &self->skelAnimModel[3], &self->skelAnimModel[4], 0);
}

void BgGanonOtyuka_Update(Actor* thisx, PlayState* play) {
    BgGanonOtyuka* self = (BgGanonOtyuka*)thisx;

    self->actionFunc(self, play);
    self->flashTimer++;
    if (self->dropTimer != 0) {
        self->dropTimer--;
    }
}

GLOBAL_ASM("asm/BgGanonOtyuka_Draw.s")

void BgGanonOtyuka_DoNothing(Actor* thisx, PlayState* play) {
}

// Fall: far (157 words). Draft follows the N64 logic and the right pool constants; diffs: ROM zero-stores the three
// Vec3f stack slots before loading the dust constants, ROM tail-calls Actor_Kill (else-branch laid out first), the dust
// colours are adjacent statics (ROM derives prim = env - 4). Draft kept under #if 0.
#if 0
void Math_ApproachF(f32* pValue, f32 target, f32 fraction, f32 step);
void Math_ApproachZeroF(f32* pValue, f32 fraction, f32 step);
f32 Rand_CenteredFloat(f32 f);
f32 Rand_ZeroFloat(f32 f);
void FUN_00365d20(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* acc, Color_RGBA8* c1, Color_RGBA8* c2, s16 scale,
                  s32 a, s32 b);
void FUN_003f87ac(Camera* camera, s32 a, s32 b);
void Audio_PlaySoundAtPosition(PlayState* play, Vec3f* pos, s32 duration, u32 sfxId);
void Audio_PlaySoundGeneral(u32 sfxId, Vec3f* a1, u8 a2, f32* a3, f32* a4, s8* a5);
void Actor_Kill(Actor* actor);

void BgGanonOtyuka_Fall(BgGanonOtyuka* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 i;
    Vec3f pos;
    Vec3f velocity;
    Vec3f accel;

    if (self->flashState == 1) {
        Math_ApproachF(&self->flashPrimColorB, 170.0f, 1.0f, 8.5f);
        Math_ApproachF(&self->flashEnvColorR, 120.0f, 1.0f, 13.5f);
        Math_ApproachF(&self->flashYScale, 2.5f, 1.0f, 0.25f);
        if (self->flashYScale == 2.5f) {
            self->flashState = 2;
        }
    } else if (self->flashState == 2) {
        Math_ApproachF(&self->flashPrimColorG, 0.0f, 1.0f, 25.5f);
        Math_ApproachF(&self->flashEnvColorR, 0.0f, 1.0f, 12.0f);
        Math_ApproachF(&self->flashEnvColorG, 0.0f, 1.0f, 25.5f);
        Math_ApproachZeroF(&self->flashYScale, 1.0f, 0.25f);
        if (self->flashYScale == 0.0f) {
            self->flashState = 0;
        }
    }
    if (self->dropTimer == 0) {
        self->flashYScale = 0.0f;
        Math_ApproachF(&self->dyna.actor.world.pos.y, -1000.0f, 1.0f, self->dyna.actor.speedXZ);
        Math_ApproachF(&self->dyna.actor.speedXZ, 100.0f, 1.0f, 2.0f);
        if (!(self->unwalledSides & 1)) {
            self->dyna.actor.shape.rot.z -= (s16)(self->dyna.actor.speedXZ * 30.0f);
        }
        if (!(self->unwalledSides & 2)) {
            self->dyna.actor.shape.rot.z += (s16)(self->dyna.actor.speedXZ * 30.0f);
        }
        if (!(self->unwalledSides & 4)) {
            self->dyna.actor.shape.rot.x += (s16)(self->dyna.actor.speedXZ * 30.0f);
        }
        if (!(self->unwalledSides & 8)) {
            self->dyna.actor.shape.rot.x -= (s16)(self->dyna.actor.speedXZ * 30.0f);
        }
        if (self->dyna.actor.world.pos.y < -750.0f) {
            if (player->actor.world.pos.y < -400.0f) {
                accel.x = accel.z = 0.0f;
                accel.y = 0.1f;
                velocity.x = velocity.y = velocity.z = 0.0f;

                for (i = 0; i < 30; i++) {
                    pos.x = Rand_CenteredFloat(150.0f) + self->dyna.actor.world.pos.x;
                    pos.y = Rand_ZeroFloat(60.0f) + -750.0f;
                    pos.z = Rand_CenteredFloat(150.0f) + self->dyna.actor.world.pos.z;
                    FUN_00365d20(play, &pos, &velocity, &accel, &Bg_Ganon_Otyuka_sDustPrimColor,
                                 &Bg_Ganon_Otyuka_sDustEnvColor, (s16)Rand_ZeroFloat(100.0f) + 250, 5,
                                 (s16)Rand_ZeroFloat(5.0f) + 15);
                }

                FUN_003f87ac((Camera*)play, 10, 15);
                Audio_PlaySoundAtPosition(play, &self->dyna.actor.world.pos, 40, 0x01000190);
            }
            Actor_Kill(&self->dyna.actor);
        }
    } else {
        if (self->dropTimer == 1) {
            Audio_PlaySoundGeneral(0x010001EA, &self->dyna.actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                   &DAT_0054ac24.unk_0);
        } else if (*(u8*)((u8*)play + 0x7FC6) < 2) {
            Audio_PlaySoundGeneral(0x01000268, &self->dyna.actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                   &DAT_0054ac24.unk_0);
            (*(u8*)((u8*)play + 0x7FC6))++;
        }
        Math_ApproachF(&self->dyna.actor.world.pos.y, -1000.0f, 1.0f, self->dyna.actor.speedXZ);
        Math_ApproachF(&self->dyna.actor.speedXZ, 100.0f, 1.0f, 0.1f);
    }
}

#endif

GLOBAL_ASM("asm/BgGanonOtyuka_Fall.s")

// WaitForFall: far (39 words after the sides pointer fix; ~14 variants). Structure matches (fabsf compares fold
// into the predicated int-bit compares via an _fabsf wrapper); remaining diff = callee-saved numbering
// (ROM r8 play+0x2000 / ip 0x106 / r7 10.0f bits, `sub r6,r5,#4` placed after `mov r3,#0`) and sSides derived from
// sPosOffsets (adjacent statics, EQU symbols here). Draft kept under #if 0; ROM stays asm.
#if 0
s32 FUN_0034c3b8(CollisionContext* colCtx, Vec3f* pos, f32 radius);
extern __pure float _fabsf(float);
static __inline __pure float BgGanonOtyuka_Fabsf(float f) {
    return _fabsf(f);
}

void BgGanonOtyuka_Fall(BgGanonOtyuka* self, PlayState* play);

void BgGanonOtyuka_WaitForFall(BgGanonOtyuka* self, PlayState* play) {
    Actor* thisx = &self->dyna.actor;
    Actor* prop;
    s16 i;
    f32 dx;
    f32 dy;
    f32 dz;
    Vec3f center;
    u8* sides = (u8*)Bg_Ganon_Otyuka_sPosOffsets - 4;

    if (self->isFalling || ((play->actorCtx.unk_02 != 0) && (self->dyna.actor.xyzDistToPlayerSq < SQ(70.0f)))) {
        for (i = 0; i < 4; i++) {
            prop = play->actorCtx.actorLists[ACTORCAT_PROP].head;
            while (prop != NULL) {
                if ((prop == thisx) || (prop->id != ACTOR_BG_GANON_OTYUKA)) {
                    prop = prop->next;
                    continue;
                } else {
                    BgGanonOtyuka* platform = (BgGanonOtyuka*)prop;

                    dx = platform->dyna.actor.world.pos.x - self->dyna.actor.world.pos.x + Bg_Ganon_Otyuka_sPosOffsets[i].x;
                    dy = platform->dyna.actor.world.pos.y - self->dyna.actor.world.pos.y;
                    dz = platform->dyna.actor.world.pos.z - self->dyna.actor.world.pos.z + Bg_Ganon_Otyuka_sPosOffsets[i].z;

                    if ((BgGanonOtyuka_Fabsf(dx) < 10.0f) && (BgGanonOtyuka_Fabsf(dy) < 10.0f) && (BgGanonOtyuka_Fabsf(dz) < 10.0f)) {
                        platform->visibleSides |= sides[i];
                        break;
                    }
                }
                prop = prop->next;
            }
        }

        for (i = 0; i < 4; i++) {
            center.x = self->dyna.actor.world.pos.x + Bg_Ganon_Otyuka_sPosOffsets[i].x;
            center.y = self->dyna.actor.world.pos.y;
            center.z = self->dyna.actor.world.pos.z + Bg_Ganon_Otyuka_sPosOffsets[i].z;
            if (FUN_0034c3b8(&play->colCtx, &center, 50.0f)) {
                self->unwalledSides |= sides[i];
            }
        }

        self->actionFunc = BgGanonOtyuka_Fall;
        self->isFalling = true;
        self->dropTimer = 30;
        self->flashState = 1;
        self->flashTimer = 0;
        self->flashPrimColorR = 255.0f;
        self->flashPrimColorG = 255.0f;
        self->flashPrimColorB = 255.0f;
        self->flashEnvColorR = 255.0f;
        self->flashEnvColorG = 255.0f;
        self->flashEnvColorB = 0.0f;
    }
}
#endif

GLOBAL_ASM("asm/BgGanonOtyuka_WaitForFall.s")

void FUN_00394828(Actor* thisx, PlayState* play) {
}

s32 Quake_Add(Camera* camera, s32 type);
void Quake_SetSpeed(s32 quakeIndex, s32 speed);
void Quake_SetQuakeValues(s32 quakeIndex, s32 a, s32 b, s32 c, s32 d);
void Quake_SetCountdown(s32 quakeIndex, s32 countdown);

void FUN_003f87ac(Camera* camera, s32 a, s32 b) {
    s32 quakeIndex = Quake_Add((Camera*)((u8*)camera + 0x364), 3);

    Quake_SetSpeed(quakeIndex, 0x4E20);
    Quake_SetQuakeValues(quakeIndex, a, 0, 0, 0);
    Quake_SetCountdown(quakeIndex, b);
}

GLOBAL_ASM("../split/asm/FUN_003f87fc.s")

}
