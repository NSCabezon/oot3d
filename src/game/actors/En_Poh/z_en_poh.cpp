extern "C" {
#include "z_en_poh.hpp"

#define FLAGS 0x00001015

void EnPoh_Init(Actor* actor, GameState* state);
void EnPoh_Destroy(Actor* actor, GameState* state);
void EnPoh_Update(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Poh.data_0052E0C0.s")
GLOBAL_ASM("data/z_En_Poh.data_0052E1C0.s")

GLOBAL_ASM("../src/game/actors/En_Poh/data_syms.s")

extern u16 EnPoh_sRupeeCount;

ActorInit En_Poh_InitVars = {
    ACTOR_EN_POH,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnPoh),
    (ActorFunc)EnPoh_Init,
    (ActorFunc)EnPoh_Destroy,
    (ActorFunc)EnPoh_Update,
    NULL,
};

f32* EnPoh_CurFramePtr(SkelAnime* skelAnime);
f32* EnPoh_PlaySpeedPtr(SkelAnime* skelAnime);

void Animation_PlayLoop(SkelAnime* skelAnime, void* anim);
void LightContext_RemoveLight(PlayState* play, void* lightCtx, void* lightNode);
void Collider_DestroyJntSph(PlayState* play, void* collider);
s32 Math_ScaledStepToS(s16* value, s16 target, s16 step);
void FUN_00353804(EnPoh* self, s32 arg);
void FUN_003e3f70(EnPoh* self, PlayState* play);
void FUN_003c70bc(EnPoh* self, PlayState* play);
void FUN_00107ae4(EnPoh* self, PlayState* play);
void FUN_0036f32c(EnPoh* self);
void FUN_0036f364(EnPoh* self, PlayState* play);

void EnPoh_Destroy(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnPoh* self = (EnPoh*)actor;

    LightContext_RemoveLight(play, (u8*)play + 0xA70, self->lightNode);
    Collider_DestroyJntSph(play, &self->unk_9CC);
    Collider_DestroyCylinder(play, &self->colliderCylinder);
    if (self->actor.params == 1) {
        EnPoh_sRupeeCount--;
    }
    SkelAnime_Free2(&self->skelAnime);
    FUN_00350f34(actor, &self->skelAnimModel[0], &self->skelAnimModel[1], 0);
}

GLOBAL_ASM("asm/EnPoh_Init.s")

void FUN_001cd5f8(EnPoh* self, PlayState* play) {
    FUN_00353804(self, -13);
    if (self->lightColorA == 0) {
        Actor_Kill(&self->actor);
    }
}

void FUN_001807a0(EnPoh* self, PlayState* play) {
    Animation_PlayLoop(&self->skelAnime, self->unk_93C);
    self->unk_920 = Rand_S16Offset_003702c8(2, 3);
    self->actor.speedXZ = 0.0f;
    self->actionFunc = FUN_003e3f70;
}

// FUN_00238c38 (post-appear settle). `s16* p = &self->unk_920` declared after the (unused) Rand call gives the
// ROM order: fn literal in r1, 0x920 offset in r0.
void FUN_00238c38(EnPoh* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime) != 0) {
        self->actor.world.rot.y = self->actor.shape.rot.y;
        Animation_PlayLoop(&self->skelAnime, self->unk_940);
        Rand_S16Offset_003702c8(15, 3);
        s16* p = &self->unk_920;
        self->actionFunc = FUN_003c70bc;
        *p = 0x23;
    } else {
        Math_StepToF(&self->actor.speedXZ, 0.0f, 0.5f);
        self->actor.shape.rot.y += 0x1000;
    }
}

void FUN_0013f9c4(EnPoh* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (Math_ScaledStepToS(&self->actor.world.rot.y, self->unk_924, 0x71C) != 0) {
        FUN_0036f32c(self);
    }
    if (self->actor.xzDistToPlayer < 200.0f) {
        Animation_PlayLoop(&self->skelAnime, self->unk_940);
        self->actionFunc = FUN_00107ae4;
        self->unk_920 = 0;
        self->actor.speedXZ = 2.0f;
    }
    FUN_0036f364(self, play);
}

void FUN_0021d620(EnPoh* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime) != 0) {
        self->lightColorA = 0xFF;
        self->unk_922 = Rand_S16Offset_003702c8(700, 300);
        self->actor.flags |= 1;
        Animation_PlayLoop(&self->skelAnime, self->unk_940);
        self->unk_920 = Rand_S16Offset_003702c8(15, 3);
        self->actionFunc = FUN_003c70bc;
    } else {
        s32 alpha = (s32)(*EnPoh_CurFramePtr(&self->skelAnime) * 25.5f);
        self->lightColorA = (alpha > 0xFF) ? 0xFF : alpha;
    }
}

void FUN_003e3f70(EnPoh* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f) != 0 && self->unk_920 != 0) {
        self->unk_920--;
    }
    FUN_0036f364(self, play);
    if (self->actor.xzDistToPlayer < 200.0f) {
        Animation_PlayLoop(&self->skelAnime, self->unk_940);
        self->actionFunc = FUN_00107ae4;
        self->unk_920 = 0;
        self->actor.speedXZ = 2.0f;
    } else if (self->unk_920 == 0) {
        Animation_PlayLoop(&self->skelAnime, self->unk_940);
        self->unk_920 = Rand_S16Offset_003702c8(15, 3);
        self->actionFunc = FUN_003c70bc;
    }
    if (self->lightColorA == 0xFF) {
        FUN_00373264(&self->actor, 0x010002E1);
    }
}

void FUN_0036f4e4(SkelAnime* skelAnime, f32 speed);

void FUN_0010abf8(EnPoh* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime) != 0) {
        self->lightColorA = 0xFF;
        self->unk_922 = Rand_S16Offset_003702c8(700, 300);
        self->actor.flags |= 1;
        Animation_PlayLoop(&self->skelAnime, self->unk_940);
        self->unk_920 = Rand_S16Offset_003702c8(15, 3);
        self->actionFunc = FUN_003c70bc;
    } else if (*EnPoh_CurFramePtr(&self->skelAnime) > 10.0f) {
        self->lightColorA = (u8)(((*EnPoh_CurFramePtr(&self->skelAnime) - 10.0f) * 0.05f) * 255.0f);
    }
    if (*EnPoh_PlaySpeedPtr(&self->skelAnime) < 0.5f && self->actor.xzDistToPlayer < 280.0f) {
        Audio_PlayActorSound2(&self->actor, 0x010002E3);
        FUN_0036f4e4(&self->skelAnime, 1.0f);
    }
}

// N64 EnPoh_Appear. The 2/3 phase select as an early-return helper keeps the zero in a callee-saved reg shared with
// the counter reset (ROM r6). Constants declared (half, third, two), assigned (two, third, half).
static inline s32 EnPoh_Phase(u32 timer) {
    f32 half, third, two;
    two = 2.0f;
    third = 1.0f / 3.0f;
    half = 0.5f;
    if (timer == 0) {
        return 0;
    }
    return (s32)(half + (timer * two) * third);
}

void FUN_0011b150(EnPoh* self, PlayState* play) {
    self->unk_91C++;
    self->actor.world.rot.y -= 0x1000;
    FUN_0036f364(self, play);
    s32 phase = EnPoh_Phase(self->unk_91C);
    self->lightColorA = (u8)(phase * 7.96875f);
    if (self->unk_91C == 0x30) {
        self->unk_922 = Rand_S16Offset_003702c8(700, 300);
        self->unk_91C = 0;
        Animation_PlayLoop(&self->skelAnime, self->unk_940);
        self->unk_920 = Rand_S16Offset_003702c8(15, 3);
        self->actionFunc = FUN_003c70bc;
    }
}

// N64 EnPoh_Disappear. Phase constants: declared (half, third, two), assigned (two, third, half) -> ROM VFP regs + pool order.
void FUN_00175170(EnPoh* self, PlayState* play) {
    s32 phase;

    if (self->unk_91C != 0) {
        self->unk_91C--;
    }
    self->actor.world.rot.y += 0x1000;
    FUN_0036f364(self, play);
    f32 half, third, two;
    two = 2.0f;
    third = 1.0f / 3.0f;
    half = 0.5f;
    u32 timer = self->unk_91C;
    phase = (timer != 0) ? (s32)(half + (timer * two) * third) : 0;
    self->lightColorA = (u8)(phase * 7.96875f);
    if (self->unk_91C == 0) {
        self->unk_922 = Rand_S16Offset_003702c8(100, 50);
        Animation_PlayLoop(&self->skelAnime, self->unk_940);
        self->unk_920 = Rand_S16Offset_003702c8(15, 3);
        self->actionFunc = FUN_003c70bc;
    }
}

void Animation_MorphToLoop(SkelAnime* skelAnime, void* anim, f32 morphFrames);
void FUN_001234c4(EnPoh* self, PlayState* play);
void FUN_003d64a4(EnPoh* self, PlayState* play);

// N64 func_80ADEECC (hurt/knockback state): ROM Math_StepToF + SkelUpdate, then flee or give-up branch.
void FUN_00120388(EnPoh* self, PlayState* play) {
    Math_StepToF(&self->actor.speedXZ, 0.0f, 0.5f);
    if (FUN_003731e0(&self->skelAnime) != 0) {
        if (self->actor.colChkInfo.health != 0) {
            Animation_MorphToLoop(&self->skelAnime, self->unk_948, -5.0f);
            self->actor.speedXZ = 5.0f;
            self->actor.world.rot.y = self->actor.shape.rot.y + 0x8000;
            self->colliderCylinder.base.acFlags |= AC_ON;
            self->unk_920 = 0x12C;
            self->actionFunc = FUN_001234c4;
        } else {
            self->unk_920 = 0;
            self->actor.speedXZ = 0.0f;
            self->actor.world.rot.y = self->actor.shape.rot.y;
            self->actor.naviEnemyId = 0xFF;
            self->actor.flags &= ~1;
            self->actionFunc = FUN_003d64a4;
        }
    }
}

// N64 func_80ADF894 (flee after hurt). Phase constants: declared (half, two, third), assigned (two, third, half).
void FUN_001234c4(EnPoh* self, PlayState* play) {
    f32 multiplier;
    s32 phase;

    FUN_003731e0(&self->skelAnime);
    {
        f32 half, two, third;
        two = 2.0f;
        third = 1.0f / 3.0f;
        half = 0.5f;
        u32 timer = self->unk_91D;

        phase = (timer != 0) ? (s32)(half + (timer * two) * third) : 0;
    }
    multiplier = Math_SinS((s16)(phase * 0x800)) * 3.0f;
    self->actor.world.pos.x -= multiplier * Math_CosS(self->actor.shape.rot.y);
    self->actor.world.pos.z += multiplier * Math_SinS(self->actor.shape.rot.y);
    Math_ScaledStepToS(&self->actor.world.rot.y, (s16)(self->actor.yawTowardsPlayer + 0x8000), 0x71C);
    FUN_0036f364(self, play);
    if (self->unk_920 == 0 || self->actor.xzDistToPlayer > 250.0f) {
        self->actor.world.rot.y = self->actor.shape.rot.y;
        Animation_PlayLoop(&self->skelAnime, self->unk_940);
        self->unk_920 = Rand_S16Offset_003702c8(15, 3);
        self->actionFunc = FUN_003c70bc;
    }
    FUN_00373264(&self->actor, 0x010002E2);
}

// N64 EnPoh_Attack. Timer scale 3DS: N64 `>= 10` -> `> 14`, `== 9` -> `== 14`, reset 23 -> 0x23.
void FUN_0011c3f4(EnPoh* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f) != 0) {
        Audio_PlayActorSound2(&self->actor, 0x010002E0);
        if (self->unk_920 != 0) {
            self->unk_920--;
        }
    }
    FUN_0036f364(self, play);
    if (self->unk_920 > 14) {
        Math_ScaledStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 0xE38);
    } else if (self->unk_920 == 14) {
        self->actor.speedXZ = 5.0f;
        FUN_0036f4e4(&self->skelAnime, 2.0f);
    } else if (self->unk_920 == 0) {
        char* flag = &self->skelAnime.unk_80[1];
        *flag = 0;
        FUN_0036f32c(self);
        self->unk_920 = 0x23;
    }
}

void FUN_001e8ef4(EnPoh* self, PlayState* play);
s32 FUN_00369a48(Actor* actor, PlayState* play);
void FUN_003ff758(Vec3f* pos, s32 sfxId);
s32 Inventory_HasEmptyBottle(void);
void Item_Give(PlayState* play, s32 itemId);

// N64 func_80ADE950: stop the big cry, laugh, go back to the idle/flee action.
static inline void EnPoh_StopCryAndLaugh(EnPoh* self, s32 laugh) {
    if (laugh) {
        FUN_003ff758(&self->actor.world.pos, 0x01000458);
        Audio_PlayActorSound2(&self->actor, 0x0100035C);
    }
    self->actionFunc = FUN_001cd5f8;
}

// N64 EnPoh_TalkComposer (literal N64 control flow + the func_80ADE950 helper).
void FUN_003e2570(EnPoh* self, PlayState* play) {
    FUN_00373264(&self->actor, 0x01000458);
    if (Message_GetState(&play->msgCtx) == 4) {
        if (Message_ShouldAdvance(play)) {
            if (FUN_00369f3c(play) == 0) {
                if (!Flags_GetSwitch(play, 0xB) && !Flags_GetSwitch(play, 0xA)) {
                    self->actor.textId = 0x5010;
                } else {
                    self->actor.textId = 0x5014;
                }
                Message_ContinueTextbox(play, self->actor.textId);
            } else {
                if (self->actor.params == 2) {
                    Flags_SetSwitch(play, 0xB);
                } else {
                    Flags_SetSwitch(play, 0xA);
                }
                EnPoh_StopCryAndLaugh(self, 1);
            }
        }
    } else if (FUN_00369a48(&self->actor, play)) {
        if (self->actor.textId == 0x5000) {
            Flags_SetSwitch(play, 9);
        }
        EnPoh_StopCryAndLaugh(self, 1);
    }
}

void Lights_PointNoGlowSetInfo(void* light, f32 x, f32 y, f32 z, u8 r, u8 g, u8 b, s16 radius, s32 pad);

// N64 func_80ADFA90 (alpha step, scale/light update). Info colours are read from the 3DS info block.
void FUN_00353804(EnPoh* self, s32 arg) {
    f32 multiplier;
    s32 alpha = self->lightColorA + arg;

    self->lightColorA = (alpha < 0) ? 0 : ((alpha > 255) ? 255 : alpha);
    if (arg < 0) {
        multiplier = self->lightColorA * (1.0f / 255);
        self->actor.scale.x = self->actor.scale.z = 0.0056000002f * multiplier + 0.0014000001f;
        self->actor.scale.y = (0.007f - 0.007f * multiplier) + 0.007f;
    } else {
        multiplier = 1.0f;
        self->actor.scale.x = self->actor.scale.y = self->actor.scale.z = self->lightColorA * (0.007f / 0xFF);
        self->actor.world.pos.y = self->actor.home.pos.y + (1.0f / 17.0f) * self->lightColorA;
    }
    self->lightColorR = self->infoLightR * multiplier;
    self->lightColorG = self->infoLightG * multiplier;
    self->lightColorB = self->infoLightB * multiplier;
    Lights_PointNoGlowSetInfo(self->lightInfo, self->actor.world.pos.x, self->actor.world.pos.y,
                              self->actor.world.pos.z, self->infoLightR, self->infoLightG, self->infoLightB,
                              self->lightColorA * (200.0f / 255), 0);
}

// N64 func_80AE067C (light colour step, 5 per frame, clamped per action state).
void FUN_0033d000(EnPoh* self) {
    if (self->actionFunc == FUN_0011c3f4) {
        {
            s32 r = self->lightColorR + 5;

            self->lightColorR = (r > 0xFF) ? 0xFF : r;
        }
        {
            s32 g = self->lightColorG - 5;

            self->lightColorG = (g < 0x32) ? 0x32 : g;
        }
        {
            s32 b = self->lightColorB - 5;

            self->lightColorB = (b < 0) ? 0 : b;
        }
    } else if (self->actionFunc == FUN_001234c4) {
        {
            s32 r = self->lightColorR + 5;

            self->lightColorR = (r > 0x50) ? 0x50 : r;
        }
        {
            s32 g = self->lightColorG + 5;

            self->lightColorG = (g > 0xFF) ? 0xFF : g;
        }
        {
            s32 b = self->lightColorB + 5;

            self->lightColorB = (b > 0xE1) ? 0xE1 : b;
        }
    } else if (self->actionFunc == FUN_00120388) {
        if (*(u16*)((u8*)self + 0x11A) & 2) {
            self->lightColorR = 0;
            self->lightColorG = 0;
            self->lightColorB = 0;
        } else {
            self->lightColorR = 0x50;
            self->lightColorG = 0xFF;
            self->lightColorB = 0xE1;
        }
    } else {
        {
            s32 r = self->lightColorR + 5;

            self->lightColorR = (r > 0xFF) ? 0xFF : r;
        }
        {
            s32 g = self->lightColorG + 5;

            self->lightColorG = (g > 0xFF) ? 0xFF : g;
        }
        s32 b;

        if (self->lightColorB <= 0xD2) {
            goto add_b;
        }
        b = self->lightColorB - 5;
        if (b >= 0xD2) {
            goto store_b;
        }
clamp_b:
        b = 0xD2;
        goto store_b;
add_b:
        b = self->lightColorB + 5;
        if (b > 0xD2) {
            goto clamp_b;
        }
store_b:
        self->lightColorB = b;
    }
}

f32 Actor_WorldDistXZToPoint(Actor* actor, Vec3f* point);
s16 Actor_WorldYawTowardPoint(Actor* actor, Vec3f* point);

// N64 EnPoh_Idle (3DS). Home-yaw recentre (func_80ADEA5C) is inlined.
void FUN_003c70bc(EnPoh* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    Math_StepToF(&self->actor.speedXZ, 1.0f, 0.2f);
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f) != 0 && self->unk_920 != 0) {
        self->unk_920--;
    }
    if (Actor_WorldDistXZToPoint(&self->actor, &self->actor.home.pos) > 400.0f) {
        self->unk_924 = Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos);
    }
    Math_ScaledStepToS(&self->actor.world.rot.y, self->unk_924, 0x71C);
    FUN_0036f364(self, play);
    if (self->actor.xzDistToPlayer < 200.0f && self->unk_920 < 29) {
        Animation_PlayLoop(&self->skelAnime, self->unk_940);
        self->actionFunc = FUN_00107ae4;
        self->unk_920 = 0;
        self->actor.speedXZ = 2.0f;
    } else if (self->unk_920 == 0) {
        if (Rand_ZeroOne() < 0.1f) {
            Animation_PlayLoop(&self->skelAnime, self->unk_93C);
            self->unk_924 = self->actor.world.rot.y + 0x8000;
            self->actor.speedXZ = 0.0f;
            self->actionFunc = FUN_0013f9c4;
        } else {
            ((void (*)(EnPoh*))FUN_001807a0)(self);
        }
    }
    if (self->lightColorA == 0xFF) {
        FUN_00373264(&self->actor, 0x010002E1);
    }
}

// N64 EnPoh_TalkRegular (3DS: textId 0x5005 plays the cry sfx, otherwise func_80ADFA90 path).
void FUN_001589a8(EnPoh* self, PlayState* play) {
    if (self->actor.textId != 0x5005) {
        FUN_00353804(self, -13);
    } else {
        FUN_00373264(&self->actor, 0x01000458);
    }
    if (Message_GetState(&play->msgCtx) == 4) {
        if (Message_ShouldAdvance(play) != 0) {
            FUN_003ff758(&self->actor.world.pos, 0x01000458);
            if (FUN_00369f3c(play) == 0) {
                if (Inventory_HasEmptyBottle()) {
                    self->actor.textId = 0x5008;
                    Item_Give(play, 0x20);
                    Audio_PlayActorSound2(&self->actor, 0x010002F3);
                } else {
                    self->actor.textId = 0x5006;
                    Audio_PlayActorSound2(&self->actor, 0x0100035C);
                }
            } else {
                self->actor.textId = 0x5007;
                Audio_PlayActorSound2(&self->actor, 0x0100035C);
            }
            Message_ContinueTextbox(play, self->actor.textId);
        }
    } else if (FUN_00369a48(&self->actor, play) != 0) {
        self->actionFunc = FUN_001cd5f8;
    }
}

void FUN_003e0e38(EnPoh* self, PlayState* play);

// N64 EnPoh_Talk (literal N64 else-if chain; must stay a separate static inline helper for the block layout).
static inline void EnPoh_Talk(EnPoh* self, PlayState* play) {
    self->actor.home.pos.y = self->actor.world.pos.y;
    Actor_SetFocus(&self->actor, -10.0f);
    self->colliderCylinder.dim.radius = 13.0f;
    self->colliderCylinder.dim.height = 30.0f;
    self->colliderCylinder.dim.yShift = 0.0f;
    self->colliderCylinder.dim.pos.x = self->actor.world.pos.x;
    self->colliderCylinder.dim.pos.y = self->actor.world.pos.y - 20.0f;
    self->colliderCylinder.dim.pos.z = self->actor.world.pos.z;
    self->colliderCylinder.base.ocFlags1 = OC1_ON | OC1_TYPE_PLAYER;
    if (self->actor.params == 3 || self->actor.params == 2) {
        if (CHECK_QUEST_ITEM(QUEST_SONG_SUN)) {
            self->actor.textId = 0x5000;
        } else if (!Flags_GetSwitch(play, 0xA) && !Flags_GetSwitch(play, 0xB)) {
            self->actor.textId = 0x500F;
        } else if ((self->actor.params == 3 && Flags_GetSwitch(play, 0xA)) ||
                   (self->actor.params == 2 && Flags_GetSwitch(play, 0xB))) {
            self->actor.textId = 0x5013;
        } else {
            self->actor.textId = 0x5012;
        }
    } else {
        self->actor.textId = 0x5005;
    }
    self->unk_920 = 300;
    self->unk_91D = 48;
    self->actor.flags |= 1;
    self->actionFunc = FUN_003e0e38;
}

// N64 func_80ADFE28 (home rise, alpha step) + EnPoh_Talk.
void FUN_001cd374(EnPoh* self, PlayState* play) {
    self->actor.home.pos.y += 2.0f;
    FUN_00353804(self, 20);
    if (self->lightColorA == 0xFF) {
        EnPoh_Talk(self, play);
    }
}

void Lights_PointGlowSetInfo(void* light, f32 x, f32 y, f32 z, u8 r, u8 g, u8 b, s16 radius, s32 pad);

// FUN_00256980 (N64 EnPoh_PostLimbDraw): still asm, draft kept off. 79 words off: the Matrix_Scale copies
// (ROM: vmov s1,s0 before each of the 6 column-0/1 products, elem in s2; none for column 2) do not reproduce; ~30 helper
// forms (inline/static, param perms, Vec3f/chain/3x-expr args) all give 79. Same blocker as En_Hata Draw.
#if 0
void Collider_UpdateSpheres(s32 index, void* collider, MTX34* mtx);

static inline void EnPoh_MtxScale(MTX34* m, f32 x, f32 y, f32 z) {
    m->data[0][0] *= x;
    m->data[1][0] *= x;
    m->data[2][0] *= x;
    m->data[0][1] *= y;
    m->data[1][1] *= y;
    m->data[2][1] *= y;
    m->data[0][2] *= z;
    m->data[1][2] *= z;
    m->data[2][2] *= z;
}

// N64 EnPoh_PostLimbDraw (3DS PostLimbDrawOpa). Matrix_Scale is applied to the limb matrix in place
// (Matrix_Scale is inlined: column-wise), then the limb matrix is copied to limbMtxCopy (N64 unk_368).
void FUN_00256980(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg) {
    MTX34* m = mtx;
    EnPoh* self = (EnPoh*)arg;

    Collider_UpdateSpheres(limbIndex, &self->unk_9CC, m);
    if (self->limbIndexToGlow == limbIndex) {
        if (self->actionFunc == FUN_003d64a4 && self->unk_920 > 27 && self->actor.scale.x != 0.0f) {
            f32 mtxScale = 0.01f / self->actor.scale.x;

            EnPoh_MtxScale(mtx, mtxScale, mtxScale, mtxScale);
        }
        {
            MTX34* src = mtx;

            self->limbMtxCopy = *src;
        }
        if (self->actionFunc == FUN_003d64a4 && self->unk_920 == 41) {
            self->actor.world.pos.x = self->limbMtxCopy.data[0][3];
            self->actor.world.pos.y = self->limbMtxCopy.data[1][3];
            self->actor.world.pos.z = self->limbMtxCopy.data[2][3];
        }
        {
            EnPohJntSphElement* elem = self->jntSphElements;

            Lights_PointGlowSetInfo(self->lightInfo, elem->worldCenter.x, elem->worldCenter.y, elem->worldCenter.z,
                                    self->envColorR, self->envColorG, self->envColorB,
                                    self->envColorA * (200.0f / 255), 0);
        }
    }
}
#endif

s32 FUN_0036f2f8(EnPoh* self, s32 angle, PlayState* play);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 index, f32 speed, f32 startFrame, f32 endFrame, s32 mode,
                               f32 morphFrames);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 index);

// N64 func_80ADEC9C (approach the player, turn to face, attack when close). Attack setup is inlined.
void FUN_00107ae4(EnPoh* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 facingDiff;

    FUN_003731e0(&self->skelAnime);
    if (self->unk_920 != 0) {
        self->unk_920--;
    }
    facingDiff = self->actor.yawTowardsPlayer - player->actor.shape.rot.y;
    if (facingDiff > 0x3000) {
        Math_ScaledStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer + 0x3000, 0x71C);
    } else if (facingDiff < -0x3000) {
        Math_ScaledStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer - 0x3000, 0x71C);
    } else {
        Math_ScaledStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 0x71C);
    }
    FUN_0036f364(self, play);
    if (self->actor.xzDistToPlayer > 280.0f) {
        FUN_0036f32c(self);
    } else if (self->unk_920 == 0 && self->actor.xzDistToPlayer < 140.0f && FUN_0036f2f8(self, 0x2AAA, play) == 0) {
        if (self->infoIdx == 0) {
            Animation_Change_00375c08(&self->skelAnime, 3, 1.5f, 0.0f, 0.0f, 0, -6.0f);
        } else {
            Animation_Change_00375c08(&self->skelAnime, 0, 1.5f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 0),
                                      0, 0.0f);
        }
        {
            char* flag = &self->skelAnime.unk_80[1];

            *flag = 1;
        }
        self->unk_920 = 18;
        self->actor.speedXZ = 0.0f;
        Audio_PlayActorSound2(&self->actor, 0x0100035C);
        self->actionFunc = FUN_0011c3f4;
    }
    if (self->lightColorA == 0xFF) {
        FUN_00373264(&self->actor, 0x010002E1);
    }
}

void FUN_00363cb8(Actor* actor, PlayState* play);
void FUN_003e2570(EnPoh* self, PlayState* play);

// N64 func_80ADFE80 (talk wait: bob while the offer is open, accept/cancel, light update).
void FUN_003e0e38(EnPoh* self, PlayState* play) {
    if (self->unk_920 != 0) {
        self->unk_920--;
    }
    if (Actor_ProcessTalkRequest(&self->actor, play) != 0) {
        if (self->actor.params >= 2) {
            self->actionFunc = FUN_003e2570;
        } else {
            self->actor.home.pos.y = self->actor.world.pos.y - 15.0f;
            self->actionFunc = FUN_001589a8;
        }
        return;
    }
    if (self->unk_920 == 0) {
        FUN_003ff758(&self->actor.world.pos, 0x01000458);
        Audio_PlayActorSound2(&self->actor, 0x0100035C);
        self->actionFunc = FUN_001cd5f8;
        self->actor.flags &= ~0x10000;
        return;
    }
    if (self->colliderCylinder.base.ocFlags1 & 2) {
        self->actor.flags |= 0x10000;
        FUN_00363cb8(&self->actor, play);
    } else {
        self->actor.flags &= ~0x10000;
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->colliderCylinder.base);
    }
    {
        f32 half, third, two;
        two = 2.0f;
        third = 1.0f / 3.0f;
        half = 0.5f;
        u32 timer = self->unk_91D;
        s32 phase = timer;

        if (timer != 0) {
            phase = (s32)(half + (timer * two) * third);
        }
        self->actor.world.pos.y = Math_SinS(phase * 0x800) * 5.0f + self->actor.home.pos.y;
    }
    if (self->unk_91D != 0) {
        self->unk_91D--;
    }
    if (self->unk_91D == 0) {
        self->unk_91D = 0x30;
    }
    self->colliderCylinder.dim.pos.y = self->actor.world.pos.y - 20.0f;
    Actor_SetFocus(&self->actor, -10.0f);
    Lights_PointNoGlowSetInfo(self->lightInfo, self->actor.world.pos.x, self->actor.world.pos.y,
                              self->actor.world.pos.z, self->infoLightR, self->infoLightG, self->infoLightB,
                              self->lightColorA * (200.0f / 255), 0);
}

typedef struct EnPohColor4f {
    f32 r, g, b, a;
} EnPohColor4f;

extern EnPohColor4f EnPoh_sSoulColor;
void FUN_00357388(Actor* actor, EnPohColor4f* color, s32 arg2, s32 arg3);
s16 Camera_GetCamDirYaw(Camera* camera);
f32 __hardfp_sinf(f32);
f32 __hardfp_cosf(f32);

// N64 EnPoh_DrawSoul (3DS). Death: glow + death model. Otherwise: soul sprite, then the soul model on a copy of
// the limb matrix turned by the camera yaw (Matrix_RotateY inlined, angle 0 skipped).
void FUN_00282cc8(EnPoh* self, PlayState* play) {
    if (self->actionFunc == FUN_001e8ef4) {
        Lights_PointGlowSetInfo(self->lightInfo, self->actor.world.pos.x, self->actor.world.pos.y,
                                self->actor.world.pos.z, self->envColorR, self->envColorG, self->envColorB, 200, 0);
        self->skelAnimModelDeath->SetUnkAC(1);
        self->skelAnimModelDeath->FUN_003721e0(&self->actor.modelMtx);
        self->skelAnimModelDeath->FUN_00372170(0);
    } else {
        EnPohColor4f color = EnPoh_sSoulColor;
        MTX34 mtx;
        s16 yaw;
        f32 angle;

        color.a = self->lightColorA * (1.0f / 255);
        FUN_00357388(&self->actor, &color, 4, 2);
        MTX34CopyAsm(&mtx, &self->actor.modelMtx);
        {
            PlayState* play2 = play;

            yaw = Camera_GetCamDirYaw(play2->cameraPtrs[play2->activeCamera]) + 0x8000;
        }
        angle = yaw * 9.58738e-05f;
        if (angle != 0.0f) {
            f32 s = __hardfp_sinf(angle);
            f32 c = __hardfp_cosf(angle);
            f32 x0;
            f32 z0;

            x0 = mtx.data[0][0];
            z0 = mtx.data[0][2];
            mtx.data[0][0] = x0 * c - z0 * s;
            mtx.data[0][2] = x0 * s + z0 * c;
            x0 = mtx.data[1][0];
            z0 = mtx.data[1][2];
            mtx.data[1][0] = x0 * c - z0 * s;
            mtx.data[1][2] = x0 * s + z0 * c;
            x0 = mtx.data[2][0];
            z0 = mtx.data[2][2];
            mtx.data[2][0] = x0 * c - z0 * s;
            mtx.data[2][2] = x0 * s + z0 * c;
        }
        self->skelAnimModelSoul->SetUnkAC(1);
        self->skelAnimModelSoul->FUN_003721e0(&mtx);
        self->skelAnimModelSoul->FUN_00372170(1);
    }
}

void FUN_001cd374(EnPoh* self, PlayState* play);

// N64 EnPoh_Death. Ground contact: burst, then func_80ADE6D4 (inlined) and wait for the next action.
// A `thisx` alias on the light call makes the pos floats load straight into s2/s1/s0 (no vmov copies); the zero
// stores follow ROM order (yOffset first, home.y before the scale zeros).
void FUN_001e8ef4(EnPoh* self, PlayState* play) {
    if (self->unk_920 != 0) {
        self->unk_920--;
    }
    if (self->actor.bgCheckFlags & 1) {
        s32 objectId = (self->infoIdx == 1) ? 0x6E : 9;

        EffectSsHahen_SpawnBurst(play, &self->actor.world.pos, 6.0f, 0, 1, 1, 15, objectId, 10,
                                 self->lanternDisplayList);
        Actor* thisx = &self->actor;
        Lights_PointNoGlowSetInfo(self->lightInfo, thisx->world.pos.x, thisx->world.pos.y, thisx->world.pos.z, 0, 0,
                                  0, 0, 0);
        self->actor.shape.yOffset = 0.0f;
        self->unk_922 = 0;
        self->actor.shape.rot.y = 0;
        self->actor.gravity = 0.0f;
        self->actor.velocity.y = 0.0f;
        self->lightColorR = 0;
        self->lightColorA = 0;
        if (self->actor.params >= 2) {
            self->lightColorG = 200;
            self->lightColorB = 0;
        } else {
            self->lightColorG = 0;
            self->lightColorB = 200;
        }
        self->actor.home.pos.y = self->actor.world.pos.y;
        self->actor.scale.x = 0.0f;
        self->actor.scale.y = 0.0f;
        self->actor.shape.rot.x = 0;
        Audio_PlayActorSound2(&self->actor, 0x01000237);
        self->actionFunc = FUN_001cd374;
    } else if (self->unk_920 == 0) {
        Actor_Kill(&self->actor);
        return;
    }
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play, &self->actor, 10.0f, 10.0f, 10.0f, 4);
}

// N64 EnPoh_UpdateLiving tail: action call, visibility counter, then func_80AE089C (env colour).
void FUN_00282e88(EnPoh* self, PlayState* play) {
    self->actionFunc(self, play);
    if (self->actionFunc != FUN_001e8ef4) {
        EnPoh* s2 = self;

        s2->unk_922 = s2->unk_922 + 1;
    }
    if ((self->actionFunc == FUN_0010abf8 || self->actionFunc == FUN_0021d620) &&
        *EnPoh_CurFramePtr(&self->skelAnime) < 12.0f) {
        self->envColorR = self->envColorG = self->envColorB = (s16)(*EnPoh_CurFramePtr(&self->skelAnime) * 16.66f) + 55;
        self->envColorA = *EnPoh_CurFramePtr(&self->skelAnime) * (100.0f / 6.0f);
    } else {
        f32 rand = Rand_ZeroOne();

        self->envColorR = (s16)(rand * 30.0f) + 225;
        self->envColorG = (s16)(rand * 100.0f) + 155;
        self->envColorB = (s16)(rand * 160.0f) + 95;
        self->envColorA = 200;
    }
}

GLOBAL_ASM("asm/EnPoh_Update.s")
}

// EnPoh_Update (3DS object-loaded gate + model/anim setup, N64 EnPoh_Init tail): still asm, draft kept off. 121 of 157
// words off (size 604 vs 628). FUN_00372f38 here is the 7-arg form (alias via EQU EnPoh_SetupModels in data_syms.s
// would be needed); ROM keeps play in r7 / 1 in r6 / unk_0C ptr in r5, flags RMW reloaded around the lightColorA store.
#if 0
ZARInfo* EnPoh_SetupModels(Actor* actor, PlayState* play, SkeletonAnimationModel** model0, s32 arg3,
                           SkeletonAnimationModel** model1, s32 arg5, s32 arg6);
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 bankIndex);
void Animation_PlayOnceSetSpeed(SkelAnime* skelAnime, s32 idx, f32 speed);
void FUN_001dd974(Actor* actor, GameState* state);
void FUN_001dd6c0(Actor* actor, GameState* state);
void FUN_00297e98(Actor* actor, GameState* state);
void FUN_0010abf8(EnPoh* self, PlayState* play);
void FUN_0021d620(EnPoh* self, PlayState* play);

void EnPoh_Update(Actor* actor, GameState* state) {
    EnPoh* self = (EnPoh*)actor;
    PlayState* play = (PlayState*)state;

    if (Object_IsLoaded(&play->objectCtx, self->unk_91E)) {
        ZARInfo* zarInfo;
        SkeletonAnimationModel_unk_0C* animModel;

        self->actor.objBankIndex = self->unk_91E;
        self->actor.update = FUN_001dd974;
        if (self->infoIdx == 0) {
            zarInfo = EnPoh_SetupModels(&self->actor, play, &self->skelAnimModel[0], 1, &self->skelAnimModel[1], 2, 0);
            animModel = self->skelAnimModel[1]->unk_0C;
            animModel->FUN_00372d94(zarInfo->GetCMABByIndex(0));
            animModel->SetAnimType(1);
            animModel->SetAnimSpeed(2.0f);
            FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 2, &self->unk_228[0], &self->unk_228[0x374], 11);
            self->actor.draw = FUN_001dd6c0;
        } else {
            zarInfo = EnPoh_SetupModels(&self->actor, play, &self->skelAnimModel[0], (s32)self->lanternDisplayList,
                                        &self->skelAnimModel[1], 1, 0);
            animModel = self->skelAnimModel[1]->unk_0C;
            animModel->FUN_00372d94(zarInfo->GetCMABByIndex(0));
            animModel->SetAnimType(1);
            animModel->SetAnimSpeed(2.0f);
            FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 3, &self->unk_228[0], &self->unk_228[0x374], 17);
            self->actor.draw = FUN_00297e98;
            ((u8*)self->jntSphElements)[0x4C] = 14;
            ((f32*)self->jntSphElements)[0x2C / 4] = -((f32*)self->jntSphElements)[0x2C / 4];
            self->actor.world.rot.y = -0x4000;
            self->actor.shape.rot.y = -0x4000;
            {
                f32* cyl = (f32*)((u8*)self + 0x9B4);

                cyl[0] = 20.0f;
                cyl[1] = 55.0f;
                cyl[2] = 15.0f;
            }
        }
        self->skelAnimModelDeath = self->skelAnimModel[0];
        self->skelAnimModelSoul = self->skelAnimModel[1];
        self->actor.flags &= ~0x10;
        self->lightColorA = 0;
        self->actor.flags &= ~1;
        if (self->infoIdx == 0) {
            Animation_PlayOnceSetSpeed(&self->skelAnime, 1, 1.0f);
            self->actionFunc = FUN_0010abf8;
        } else {
            Animation_PlayOnceSetSpeed(&self->skelAnime, 4, 1.0f);
            self->actor.world.pos.y = self->actor.home.pos.y + 20.0f;
            Audio_PlayActorSound2(&self->actor, 0x0100035C);
            Audio_PlayActorSound2(&self->actor, 0x010002E3);
            self->actionFunc = FUN_0021d620;
        }
    }
}
#endif
