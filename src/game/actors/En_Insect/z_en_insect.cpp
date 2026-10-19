extern "C" {
#include "z_en_insect.hpp"

#define FLAGS 0x00000000

void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void Math_Vec3f_Copy(Vec3f* dest, Vec3f* src);
void EffectSsBubble_Spawn(PlayState* play, Vec3f* pos, f32 a, f32 b, f32 c, f32 d);
s16 Rand_S16Offset_003702c8(s16 base, s16 range);
s32 Math_ScaledStepToS(s16* value, s16 target, s16 step);
void FUN_00352b58(EnInsect* self); // EnInsect_SetupSlowDown
void FUN_00352ce8(EnInsect* self); // EnInsect_SetupWalkOnWater
void FUN_003bcadc(EnInsect* self, PlayState* play); // EnInsect_Dig
void FUN_003be100(EnInsect* self, PlayState* play); // EnInsect_SlowDown
void FUN_003bfe54(EnInsect* self, PlayState* play); // EnInsect_WalkOnWater
void FUN_003ba2a0(EnInsect* self, PlayState* play); // EnInsect_RunFromPlayer

void EnInsect_Init(Actor* thisx, PlayState* play);
void EnInsect_Destroy(Actor* thisx, PlayState* play);
void EnInsect_Update(Actor* thisx, PlayState* play);
void EnInsect_Draw(Actor* thisx, PlayState* play);

// Statics that live in PlayState on 3DS (ROM offsets 0x7F58 / 0x7F5A).
#define EN_INSECT_S_CAUGHT_COUNT(play) (*(s16*)((char*)(play) + 0x7F58))
#define EN_INSECT_S_DROPPED_COUNT(play) (*(s16*)((char*)(play) + 0x7F5A))

void Collider_DestroyJntSph(PlayState* play, void* collider);
void Collider_UpdateSpheres(s32 index, void* collider, MTX34* mtx);

GLOBAL_ASM("data/z_En_Insect.data.s")

ActorInit En_Insect_InitVars = {
    ACTOR_EN_INSECT,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnInsect),
    (ActorFunc)EnInsect_Init,
    (ActorFunc)EnInsect_Destroy,
    (ActorFunc)EnInsect_Update,
    (ActorFunc)EnInsect_Draw,
};

GLOBAL_ASM("asm/EnInsect_Init.s")

void EnInsect_Destroy(Actor* thisx, PlayState* play) {
    EnInsect* self = (EnInsect*)thisx;
    s16 type = self->actor.params & 3;

    Collider_DestroyJntSph(play, &self->collider);
    if ((type == 2 || type == 3) && EN_INSECT_S_DROPPED_COUNT(play) > 0) {
        EN_INSECT_S_DROPPED_COUNT(play)--;
    }
    SkelAnime_Free2(&self->skelAnime);
}

GLOBAL_ASM("asm/EnInsect_Update.s")

void EnInsect_Draw(Actor* thisx, PlayState* play) {
    EnInsect* self = (EnInsect*)thisx;

    GraphicsContext_UnknownStub(play->state.gfxCtx);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, NULL, NULL, thisx, 0);
    Collider_UpdateSpheres(0, &self->collider, &self->actor.modelMtx);
    EN_INSECT_S_CAUGHT_COUNT(play) = 0;
}

void FUN_003cc0c4(EnInsect* self, PlayState* play) { // EnInsect_Caught
    if (self->actionTimer == 20 && !(self->insectFlags & INSECT_FLAG_IS_SHORT_LIVED)) {
        self->actor.draw = (ActorFunc)EnInsect_Draw;
    } else if (self->actionTimer == 0) {
        if (self->insectFlags & INSECT_FLAG_IS_SHORT_LIVED) {
            Actor_Kill(&self->actor);
        } else {
            Actor_SetScale(&self->actor, 0.01f);
            FUN_00352b58(self);
        }
    } else if ((f32)self->actionTimer < 0.5f + (3.0f / R_UPDATE_RATE) * 20.0f) {
        f32 scale = self->actor.scale.x + 0.001f;

        Actor_SetScale(&self->actor, (scale <= 0.01f) ? scale : 0.01f);
        FUN_003731e0(&self->skelAnime);
    }
}

// Draft, NOT matched (Sonnet G34): FUN_0021ea58 (N64 EnInsect_UpdateCrawlSfx), 9 insns off, ~60 variants. Structure is
// right (float select `delay = MAX((f32)delay, 0.5+(3/R)*2)` via vmovlo; the ROM recomputes 3/R in the else path).
// Only difference: ROM hoists the 2.0f literal to the top block as s3 (right after the 3.0f literal) and keeps the 0.5
// working copy in s2; ours loads 2.0f later as s2 and the copy lands in s3. Locals for 3.0f/2.0f/0.5f, MAX/ternary
// forms, operand orders and inline FRAMES helpers did not move it.
#if 0
void FUN_0021ea58(EnInsect* self) {
    if (self->crawlSoundDelay > 0) {
        self->crawlSoundDelay--;
        return;
    }
    Audio_PlayActorSound2(&self->actor, 0x01000460); // NA_SE_EN_MUSI_WALK
    self->crawlSoundDelay = (3.0f / CLAMP_MIN(self->skelAnime.playSpeed, 0.1f)) * (3.0f / R_UPDATE_RATE);
    self->crawlSoundDelay = (self->crawlSoundDelay > (0.5f + (3.0f / R_UPDATE_RATE) * 2.0f)) ? (f32)self->crawlSoundDelay : (0.5f + (3.0f / R_UPDATE_RATE) * 2.0f);
}
#endif

// Draft, NOT matched (Sonnet G34): EnInsect_Update, 14 insns off. Everything except the inlined N64
// EnInsect_InBottleRange matches (ROM: `mov r5,r4` copies `this` into r5 and the player lands in r8; ours keeps `this` in
// r4 and puts the player in r5). The dead alias `s2` (only used for the helper call) fixed the zero-constant VFP number
// (s16). `thisx->` on the Actor_HasParent/xzDistToPlayer accesses gives the ROM's `mov r0,r4` base. A helper taking
// `Actor* thisx` DOES produce the `mov rN,r4` copy but then play/zero swap r6/r7 (34 insns off).
// Inlined (not called): EnInsect_SetupCaught (FUN_003cc0c4 is the Caught action), InBottleRange.
#if 0
void FUN_0021ea58(EnInsect* self);
void FUN_003cc0c4(EnInsect* self, PlayState* play);


static f32 EnInsect_XZDistanceSquared(Vec3f* v1, Vec3f* v2) {
    return SQ(v1->x - v2->x) + SQ(v1->z - v2->z);
}

static s32 EnInsect_InBottleRange(EnInsect* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Vec3f pos;

    if (self->actor.xzDistToPlayer < 32.0f) {
        pos.x = Math_SinS(self->actor.yawTowardsPlayer + 0x8000) * 16.0f + player->actor.world.pos.x;
        pos.y = player->actor.world.pos.y;
        pos.z = Math_CosS(self->actor.yawTowardsPlayer + 0x8000) * 16.0f + player->actor.world.pos.z;

        if (EnInsect_XZDistanceSquared(&pos, &self->actor.world.pos) <= SQ(20.0f)) {
            return 1;
        }
    }

    return 0;
}

void EnInsect_Update(Actor* thisx, PlayState* play) {
    EnInsect* self = (EnInsect*)thisx;
    s32 tmp;
    EnInsect* s2 = self;

    if (self->actor.child != NULL) {
        if (self->actor.child->update == NULL) {
            if (self->actor.child != &self->actor) {
                self->actor.child = NULL;
            }
        }
    }

    if (self->actionTimer > 0) {
        self->actionTimer--;
    }

    if (self->lifeTimer > 0) {
        self->lifeTimer--;
    }

    self->actionFunc(self, play);

    if (self->actor.update != NULL) {
        Actor_MoveForward(&self->actor);
        if (self->insectFlags & INSECT_FLAG_CRAWLING) {
            if (self->insectFlags & INSECT_FLAG_0) {
                if (self->actor.bgCheckFlags & 1) {
                    FUN_0021ea58(self);
                }
            } else {
                FUN_0021ea58(self);
            }
        }

        tmp = 0;

        if (self->insectFlags & INSECT_FLAG_0) {
            tmp = 4;
        }

        if (self->insectFlags & INSECT_FLAG_1) {
            tmp |= 1;
        }

        if (tmp != 0) {
            tmp |= 0x40;
            Actor_UpdateBgCheckInfo(play, &self->actor, 8.0f, 5.0f, 0.0f, tmp);
        }

        if (Actor_HasParent(&self->actor, play)) {
            thisx->parent = NULL;
            tmp = self->actor.params;

            if ((tmp & 3) == 2 || (tmp & 3) == 3) {
                Actor_Kill(&self->actor);
            } else {
                self->actionTimer = EN_INSECT_FRAMES(200.0f);
                Actor_SetScale(&self->actor, 0.001f);
                self->actor.draw = NULL;
                self->actor.speedXZ = 0.0f;
                Animation_Change_00375c08(&self->skelAnime, 10, 1.0f, 0.0f, 0.0f, 1, 0.0f);
                self->skelAnime.playSpeed = 0.3f;
                self->actionFunc = FUN_003cc0c4;
                self->insectFlags &= ~INSECT_FLAG_CRAWLING;
            }
        } else if (thisx->xzDistToPlayer < 50.0f && self->actionFunc != FUN_003cc0c4) {
            if (!(self->insectFlags & INSECT_FLAG_SOIL_CLOSE) && self->lifeTimer < 180) {
                CollisionCheck_SetOC(play, &play->colChkCtx, (Collider*)&self->collider);
            }

            if (!(self->insectFlags & INSECT_FLAG_UNCATCHABLE) && EN_INSECT_S_CAUGHT_COUNT(play) < 4 &&
                EnInsect_InBottleRange(s2, play) && GiveItem(&self->actor, play, 0x7E, 60.0f, 30.0f)) {
                EN_INSECT_S_CAUGHT_COUNT(play)++;
            }
        }

        Actor_SetFocus(&self->actor, 0.0f);
    }
}
#endif

// Draft, NOT matched (Sonnet G34): EnInsect_Init, 14 insns off (3 VFP-numbering clusters: lifeTimer 200.0f literal is
// hoisted in ours; the 0.5 working copy / 10.0f literal swap s1<->s2 in the crawl arm). Data symbols needed when enabling:
// EnInsect_sInitChain EQU 0x0052CEC8, EnInsect_sInitInsectFlags EQU 0x0052CEB0, EnInsect_sColliderJntSphInit EQU 0x0052CEB8
// (new data_syms.s in this dir). Findings: `Dist2DSq(x0,z0,x1,z1)` float-arg helper reproduces the soil-search loads;
// `thisx->gravity/minVelocityY` keeps the insectFlags load alive; `(EnInsect*)thisx` for SetupCrawl and play2 for
// Actor_Spawn fixed the callee-saved regs; run-from-player timer `{ s16 b = FRAMES(40); Rand(FRAMES(10), b) }`.
// D_80A7DEB0 lives in PlayState (+0x7F54). Actor_Spawn takes 11 args here (last = 1).
#if 0
extern InitChainEntry EnInsect_sInitChain[];
extern u16 EnInsect_sInitInsectFlags[];
extern char EnInsect_sColliderJntSphInit[];
void Collider_InitJntSph(PlayState* play, void* collider);
void Collider_SetJntSph(PlayState* play, void* collider, Actor* actor, void* init, void* elements);
void FUN_003aea04(EnInsect* self, PlayState* play);
void FUN_0015d708(EnInsect* self, PlayState* play);
#define EN_INSECT_D_80A7DEB0(play) (*(f32*)((char*)(play) + 0x7F54))

static __inline f32 Dist2DSq(f32 x0, f32 z0, f32 x1, f32 z1) {
    f32 dx = x0 - x1;
    f32 dz = z0 - z1;
    return SQ(dx) + SQ(dz);
}

static s32 EnInsect_TryFindNearbySoil(EnInsect* self, PlayState* play) {
    Actor* currentActor = play->actorCtx.actorLists[ACTORCAT_ITEMACTION].head;
    f32 currentDistanceSq;
    f32 bestDistanceSq = SQ(80.0f);
    s32 ret = 0;

    self->soilActor = NULL;

    while (currentActor != NULL) {
        if (currentActor->id == ACTOR_OBJ_MAKEKINSUTA) {
            currentDistanceSq = Dist2DSq(self->actor.world.pos.x, self->actor.world.pos.z, currentActor->world.pos.x, currentActor->world.pos.z);

            if (currentDistanceSq < bestDistanceSq && currentActor->room == self->actor.room) {
                ret = 1;
                bestDistanceSq = currentDistanceSq;
                self->soilActor = (struct ObjMakekinsuta*)currentActor;
            }
        }
        currentActor = currentActor->next;
    }
    return ret;
}

static void EnInsect_SetupDropped(EnInsect* self) {
    Animation_Change_00375c08(&self->skelAnime, 10, 1.0f, 0.0f, 0.0f, 1, 0.0f);
    self->actionTimer = 100;
    self->unk_324 = 1.5f;
    self->unk_328 = Rand_ZeroOne() * (0xFFFF + 0.5f);
    self->unk_316 = (Rand_ZeroOne() - 0.5f) * 1500.0f;
    self->actor.world.rot.y = Rand_ZeroOne() * (0xFFFF + 0.5f);
    Actor_SetScale(&self->actor, 0.003f);
    self->actionFunc = FUN_0015d708;
    self->insectFlags |= INSECT_FLAG_CRAWLING;
}

static void EnInsect_SetupCrawl(EnInsect* self) {
    self->actionTimer = Rand_S16Offset_003702c8(EN_INSECT_FRAMES(10.0f), EN_INSECT_FRAMES(45.0f));
    Animation_Change_00375c08(&self->skelAnime, 10, 1.0f, 0.0f, 0.0f, 1, 0.0f);
    self->actionFunc = FUN_003aea04;
    self->insectFlags |= INSECT_FLAG_CRAWLING;
}

static void EnInsect_SetupRunFromPlayer(EnInsect* self) {
    { s16 b = EN_INSECT_FRAMES(40.0f); self->actionTimer = Rand_S16Offset_003702c8(EN_INSECT_FRAMES(10.0f), b); }
    Animation_Change_00375c08(&self->skelAnime, 10, 1.0f, 0.0f, 0.0f, 1, 0.0f);
    self->actionFunc = FUN_003ba2a0;
    self->insectFlags |= INSECT_FLAG_CRAWLING;
}

void EnInsect_Init(Actor* thisx, PlayState* play2) {
    EnInsect* self = (EnInsect*)thisx;
    PlayState* play = play2;
    f32 rand;
    s16 type;
    s32 count;

    Actor_ProcessInitChain(&self->actor, EnInsect_sInitChain);
    self->insectFlags = EnInsect_sInitInsectFlags[self->actor.params & 3];

    type = self->actor.params & 3;

    FUN_00353c9c(&self->actor, play, &self->skelAnime, 19, 10, &self->unk_298[0], &self->unk_298[0x1D4], 9);
    self->actor.unk_19A = 1;
    Collider_InitJntSph(play, &self->collider);
    Collider_SetJntSph(play, &self->collider, &self->actor, EnInsect_sColliderJntSphInit, &self->collider[0x20]);

    self->actor.colChkInfo.mass = 30;

    if (self->insectFlags & INSECT_FLAG_0) {
        thisx->gravity = -0.2f;
        thisx->minVelocityY = -2.0f;
    }

    if (self->insectFlags & INSECT_FLAG_IS_SHORT_LIVED) {
        self->lifeTimer = Rand_S16Offset_003702c8(EN_INSECT_FRAMES(200.0f), EN_INSECT_FRAMES(40.0f));
        self->actor.flags |= 0x10;
    }

    if (type == 2 || type == 3) {
        if (EnInsect_TryFindNearbySoil(self, play)) {
            self->insectFlags |= 0x18;
            EN_INSECT_D_80A7DEB0(play) = 0.0f;
        }

        if (type == 2) {
            self->actor.world.rot.z = 0;
            self->actor.shape.rot.z = self->actor.world.rot.z;

            for (count = 0; count < 2; count++) {
                Actor_Spawn(&play2->actorCtx, play2, ACTOR_EN_INSECT, thisx->world.pos.x, thisx->world.pos.y,
                            thisx->world.pos.z, thisx->shape.rot.x, thisx->shape.rot.y,
                            thisx->shape.rot.z, 3, 1);
            }
        }

        EnInsect_SetupDropped(self);

        EN_INSECT_S_DROPPED_COUNT(play)++;
    } else {
        rand = Rand_ZeroOne();

        if (rand < 0.3f) {
            FUN_00352b58(self);
        } else if (rand < 0.4f) {
            EnInsect_SetupCrawl((EnInsect*)thisx);
        } else {
            EnInsect_SetupRunFromPlayer(self);
        }
    }
}
#endif

// Draft, NOT matched (Sonnet G34): FUN_003be100 (EnInsect_SlowDown), 2 words off (ROM uses r1 as the temp for the
// `shape.rot.y = world.rot.y` copy right after the SkelAnime update call; ours r0). Findings (reusable): the playSpeed
// store `add r0,r4,#0x244; vstr s0,[r0,#0x10]` = block-scoped pointer to the sub-object at SkelAnime+0x30
// (`AnimCtl* ac = (AnimCtl*)&self->skelAnime.animationType; ac->playSpeed = v;`), `f32* sp = &speedXZ; SmoothStepToF(sp,..)`
// fixes the prologue VFP numbering, `EnInsectActionFunc* pf = &self->actionFunc; *pf = F` fixes the r0/r1 pool-load.
#if 0
void FUN_00352c20(EnInsect* self);
void FUN_00317908(EnInsect* self);
void FUN_003aea04(EnInsect* self, PlayState* play);
struct AnimCtl { s32 animationType; char p1[8]; f32 curFrame; f32 playSpeed; };
static void EnInsect_SetupCrawl(EnInsect* self) {
    self->actionTimer = Rand_S16Offset_003702c8(EN_INSECT_FRAMES(10.0f), EN_INSECT_FRAMES(45.0f));
    Animation_Change_00375c08(&self->skelAnime, 10, 1.0f, 0.0f, 0.0f, 1, 0.0f);
    { EnInsectActionFunc* pf = &self->actionFunc; *pf = FUN_003aea04; }
    self->insectFlags |= INSECT_FLAG_CRAWLING;
}
void FUN_003be100(EnInsect* self, PlayState* play) { // EnInsect_SlowDown
    s16 type = self->actor.params & 3;
    f32 playSpeed;

    { f32* sp = &self->actor.speedXZ; Math_SmoothStepToF(sp, 0.0f, 0.1f, 0.5f, 0.0f); }

    playSpeed = (Rand_ZeroOne() * 0.8f) + (1.2f * self->actor.speedXZ);
    { AnimCtl* ac = (AnimCtl*)&self->skelAnime.animationType; ac->playSpeed = CLAMP(playSpeed, 0.0f, 1.9f); }

    FUN_003731e0(&self->skelAnime);
    self->actor.shape.rot.y = self->actor.world.rot.y;
    if (self->actionTimer <= 0) {
        EnInsect_SetupCrawl(self);
    }

    if (((self->insectFlags & INSECT_FLAG_IS_SHORT_LIVED) && self->lifeTimer <= 0) ||
        ((type == 2 || type == 3) && (self->insectFlags & INSECT_FLAG_0) &&
         (self->actor.bgCheckFlags & 0x1) && EN_INSECT_S_DROPPED_COUNT(play) > 3)) {
        FUN_00352c20(self);
    } else if ((self->insectFlags & INSECT_FLAG_0) && (self->actor.bgCheckFlags & 0x40)) {
        FUN_00352ce8(self);
    } else if (self->actor.xzDistToPlayer < 40.0f) {
        FUN_00317908(self);
    }
}
#endif

// Draft, NOT matched (Sonnet G34): FUN_003ba2a0 (EnInsect_RunFromPlayer), ~22 insns off: the XZ distance loads
// (ROM: wx=s3, hx=s5, wz=s2, hz=s4 all loaded first, then reused by Math_Vec3f_Yaw) need the right Dist2DSq param
// order; the playSpeed store uses the same AnimCtl idiom as SlowDown; the final `(flags&1) && (bgCheck&0x40)` is a real
// branch in the ROM (ours predicated).
#if 0
void FUN_00352c20(EnInsect* self);
void FUN_00317908(EnInsect* self);
struct AnimCtl { s32 animationType; char p1[8]; f32 curFrame; f32 playSpeed; };
static __inline f32 Dist2DSq(f32 z0, f32 x1, f32 z1, f32 x0) {
    f32 dz = z0 - z1;
    f32 dx = x0 - x1;
    return SQ(dx) + SQ(dz);
}

static __inline f32 EnInsect_XZDistanceSquared(Vec3f* v1, Vec3f* v2) {
    return Dist2DSq(v1->z, v2->x, v2->z, v1->x);
}

void FUN_003ba2a0(EnInsect* self, PlayState* play) { // EnInsect_RunFromPlayer
    s16 playerIsClose = self->actor.xzDistToPlayer < 40.0f;
    s16 frames;
    s16 yaw;

    { f32* sp = &self->actor.speedXZ; Math_SmoothStepToF(sp, 1.8f, 0.1f, 0.5f, 0.0f); }

    if (EnInsect_XZDistanceSquared(&self->actor.world.pos, &self->actor.home.pos) > 25600.0f ||
        (f32)self->actionTimer < 0.5f + (3.0f / R_UPDATE_RATE) * 4.0f) {
        yaw = Math_Vec3f_Yaw(&self->actor.world.pos, &self->actor.home.pos);
        Math_ScaledStepToS(&self->actor.world.rot.y, yaw, 2000);
    } else if (playerIsClose) {
        frames = play->state.frames;
        yaw = self->actor.yawTowardsPlayer + 0x8000;

        if (frames & 0x10) {
            if (frames & 0x20) {
                yaw += 0x2000;
            }
        } else {
            if (frames & 0x20) {
                yaw -= 0x2000;
            }
        }
        Math_ScaledStepToS(&self->actor.world.rot.y, yaw, 2000);
    }
    self->actor.shape.rot.y = self->actor.world.rot.y;
    { AnimCtl* ac = (AnimCtl*)&self->skelAnime.animationType; ac->playSpeed = CLAMP(self->actor.speedXZ * 1.6f, 0.8f, 1.9f); }
    FUN_003731e0(&self->skelAnime);

    if (self->actionTimer <= 0 || !playerIsClose) {
        FUN_00352b58(self);
    } else if ((self->insectFlags & INSECT_FLAG_0) && (self->actor.bgCheckFlags & 0x40)) {
        FUN_00352ce8(self);
    }
}
#endif

// Draft, NOT matched (Haiku; Sonnet G34 tried FRAMES-constant locals in all 120 declaration orders + operand orders: 17 insns off, no gain): 19 words off (rot.x/rot.y step expression and the scale clamp register order).
#if 0
void FUN_003bd1ec(EnInsect* self, PlayState* play) { // EnInsect_Drown
    self->actor.shape.rot.x = (s16)((f32)self->actor.shape.rot.x - (0.5f + R_UPDATE_RATE * (1.0f / 3.0f)) * 500.0f);
    self->actor.shape.rot.y = (s16)((f32)self->actor.shape.rot.y + (0.5f + R_UPDATE_RATE * (1.0f / 3.0f)) * 200.0f);
    Actor_SetScale(&self->actor, CLAMP_MIN(self->actor.scale.x - 0.00005f, 0.001f));

    if (self->actor.yDistToWater > 5.0f && self->actor.yDistToWater < 30.0f && Rand_ZeroOne() < 0.3f) {
        EffectSsBubble_Spawn(play, &self->actor.world.pos, -5.0f, 5.0f, 5.0f, (Rand_ZeroOne() * 0.04f) + 0.02f);
    }

    if (self->actionTimer <= 0) {
        Actor_Kill(&self->actor);
    }
}
#endif

GLOBAL_ASM("asm/FUN_0021ea58.s")

}
