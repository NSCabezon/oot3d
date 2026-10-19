extern "C" {
#include "z_en_arrow.hpp"
#include "model_manager.hpp"
#define R_UPDATE_RATE SREG(30)

#define FLAGS 0x00000030

void EnArrow_Init(Actor* actor, GameState* state);
void EnArrow_Destroy(Actor* actor, GameState* state);
void EnArrow_Update(Actor* actor, GameState* state);
void EnArrow_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Arrow.data.s")

ActorInit En_Arrow_InitVars = {
    ACTOR_EN_ARROW,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnArrow),
    (ActorFunc)EnArrow_Init,
    (ActorFunc)EnArrow_Destroy,
    (ActorFunc)EnArrow_Update,
    (ActorFunc)EnArrow_Draw,
};

#if 0 // Full Sol draft:1308/1316B126words; statics unpinned, see STATUS.
struct EnArrowBlureInit2 {
    u8 calcMode;
    u16 flags;
    s16 addAngleChange;
    u8 p1StartColor[4];
    u8 p2StartColor[4];
    u8 p1EndColor[4];
    u8 p2EndColor[4];
    u8 elemDuration;
    u8 unkFlag;
    u8 drawMode;
    u8 mode4Param;
    Color_RGBA8 altPrimColor;
    Color_RGBA8 altEnvColor;
    void* context;
    u8 trailLength;
};
typedef char EnArrowBlureSizeCheck[(sizeof(EnArrowBlureInit2) == 0x2C) ? 1 : -1];
typedef char EnArrowBlureDurationOffsetCheck[(offsetof(EnArrowBlureInit2, elemDuration) == 0x16) ? 1 : -1];
typedef char EnArrowBlurePrimOffsetCheck[(offsetof(EnArrowBlureInit2, altPrimColor) == 0x1A) ? 1 : -1];
typedef char EnArrowBlureContextOffsetCheck[(offsetof(EnArrowBlureInit2, context) == 0x24) ? 1 : -1];
extern "C++" {
class EnGanonMantFreer {
public:
    virtual void vf00();
    virtual void vf04();
    virtual void vf08();
    virtual void* Alloc(u32, const char*, s32);
    virtual void Free(void*);
};
}
extern EnGanonMantFreer* gUnk_0055A1F8;
extern InitChainEntry EnArrow_sInitChain[];
extern u8 EnArrow_sQuadInit[];
extern u8 EnArrow_sTextureModelData[];
extern u32 EnArrow_sDmgFlags[10];
void EnArrow_Shoot(EnArrow*, PlayState*);
void* ZAR_GetCTXBByIndex(ZARInfo*, s32);
void* ZAR_GetCMBByIndex(ZARInfo*, s32);
void* FUN_00348f34(void*, void*);
void FUN_00348be4(void*);
void FUN_00348a64(void*, s32, void*, s32, s32, s32, s32);
void* FUN_0034897c(void*, void*, void*, s32);
void FUN_00358ea8(ZARInfo*, PlayState*, SkelAnime*, void*, void*, s32, void*, void*, s32);
void Collider_InitQuad(PlayState*, void*);
void Collider_SetQuad(PlayState*, void*, Actor*, void*);
s32 Effect_Add(PlayState*, s32*, s32, u8, u8, void*);
struct EnArrowManagedModelState { char pad[0x170]; s32 enabled; };
struct EnArrowSkelModelState { char pad[0xAD]; u8 flagAD; };

void EnArrow_Init(Actor* actor, GameState* state) {
    EnArrow* self = (EnArrow*)actor;
    PlayState* play = (PlayState*)state;
    Actor_ProcessInitChain(&self->actor, EnArrow_sInitChain);
    if (self->actor.params == -10) {
        self->isCsNut = true;
        actor->params = 10;
    }
    self->drawEnabled = true;
    self->hitSuppressed = false;
    actor->unk_19A = 1;
    self->textureModel = NULL;
    self->managedModel = NULL;
    if (self->actor.params >= 9) {
        void* ctxb = ZAR_GetCTXBByIndex(&play->GetObjectStatus(self->actor.objBankIndex)->zarInfo, 5);
        {
            const char* file = "d:\\home\\queen\\dailyBuild\\game_us\\sources\\z_en_arrow.cpp";
            void* mem = gUnk_0055A1F8->Alloc(0x1B8, file, 243);
            self->textureModel = mem != NULL ? FUN_00348f34(mem, EnArrow_sTextureModelData) : NULL;
        }
        FUN_00348be4(self->textureModel);
        FUN_00348a64(self->textureModel, 0, ctxb, 0x2601, 0x2601, 0x8370, 0x8370);
        self->managedModel = FUN_0034897c(GetModelManager().unk_247C, self->textureModel, self->actor.unk_178, 0);
        ((EnArrowManagedModelState*)self->managedModel)->enabled = 0;
    }
    if (self->actor.params < 10) {
        if (self->actor.params < 9) {
            ZARInfo* zar = &play->GetObjectStatus(self->actor.objBankIndex)->zarInfo;
            void* cmb = ZAR_GetCMBByIndex(zar, 0);
            FUN_00358ea8(zar, play, &self->skelAnime, cmb, self->actor.unk_178, 1, NULL, NULL, 0);
            ((EnArrowSkelModelState*)self->skelAnime.unk_28)->flagAD = 0;
        }
        if (self->actor.params <= 2) {
            static EnArrowBlureInit2 blureNormal = {
                0, 4, 0, {0,255,200,255}, {0,255,255,255}, {0,255,200,0}, {0,255,255,0},
                (u8)(48.0f / R_UPDATE_RATE + 0.5f), 0, 1, 0, {255,255,170,255}, {0,150,0,0}, NULL, 13
            };
            if (self->actor.params == 1) { blureNormal.elemDuration = 4; }
            else { blureNormal.elemDuration = 16; }
            Effect_Add(play, &self->effectIndex, 2, 0, 0, &blureNormal);
        } else if (self->actor.params == 3) {
            static EnArrowBlureInit2 blureFire = {
                0, 4, 0, {0,255,200,255}, {0,255,255,255}, {0,255,200,0}, {0,255,255,0},
                (u8)(48.0f / R_UPDATE_RATE + 0.5f), 0, 1, 0, {255,127,0,255}, {255,0,0,0}, NULL, 13
            };
            Effect_Add(play, &self->effectIndex, 2, 0, 0, &blureFire);
        } else if (self->actor.params == 4) {
            static EnArrowBlureInit2 blureIce = {
                0, 4, 0, {0,255,200,255}, {0,255,255,255}, {0,255,200,0}, {0,255,255,0},
                (u8)(48.0f / R_UPDATE_RATE + 0.5f), 0, 1, 0, {51,251,255,255}, {0,100,255,0}, NULL, 13
            };
            Effect_Add(play, &self->effectIndex, 2, 0, 0, &blureIce);
        } else if (self->actor.params == 5) {
            static EnArrowBlureInit2 blureLight = {
                0, 4, 0, {0,255,200,255}, {0,255,255,255}, {0,255,200,0}, {0,255,255,0},
                (u8)(48.0f / R_UPDATE_RATE + 0.5f), 0, 1, 0, {204,255,102,255}, {255,255,0,0}, NULL, 13
            };
            Effect_Add(play, &self->effectIndex, 2, 0, 0, &blureLight);
        }
        Collider_InitQuad(play, &self->collider);
        Collider_SetQuad(play, &self->collider, &self->actor, EnArrow_sQuadInit);
        if (self->actor.params <= 2) { self->collider.info.toucherFlags &= ~0x18; }
        if (self->actor.params < 0) { self->collider.base.atFlags = 0x11; }
        else if (self->actor.params <= 9) { self->collider.info.toucher.dmgFlags = EnArrow_sDmgFlags[self->actor.params]; }
    }
    self->actionFunc = EnArrow_Shoot;
}


#endif
GLOBAL_ASM("asm/EnArrow_Init.s")

GLOBAL_ASM("asm/EnArrow_Destroy.s")

GLOBAL_ASM("asm/EnArrow_Update.s")

void FUN_003cb1b8(EnArrow* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (self->timer != 0) {
        self->timer--;
        if (self->timer != 0) {
            return;
        }
    }
    Actor_Kill(&self->actor);
}

void FUN_0010d850(EnArrow* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    Actor_MoveForward(&self->actor);
    if (self->timer != 0) {
        self->timer--;
        if (self->timer != 0) {
            return;
        }
    }
    Actor_Kill(&self->actor);
}

#if 0 // Candidate wrapper: the observed callee signature is unresolved.
s32 BgCheck_LineTestImpl(CollisionContext*, s32, Vec3f*, Vec3f*, Vec3f*, f32, CollisionPoly**, s32*, Actor*, s32);
s32 FUN_003aa580(void* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** poly, s32 chkWall,
                 s32 chkFloor, s32 chkCeil, s32 chkOneFace, s32* bgId) {
    s32 flags = 0;
    if (chkWall) flags |= 1;
    if (chkFloor) flags |= 2;
    if (chkCeil) flags |= 4;
    if (chkOneFace) flags |= 8;
    return BgCheck_LineTestImpl((CollisionContext*)colCtx, 4, posA, posB, posResult, 1.0f, poly, bgId, NULL,
                                flags | 0x10);
}
#endif

#if 0 // Full Sol draft:1796/1812B385words; guarded vectors unpinned.
struct EnArrowPlayerView {
    Actor actor;
    char pad[0x247E - sizeof(Actor)];
    u8 projectileState;
};
struct EnArrowPlayFrameView { char pad[0x5BF4]; u32 gameplayFrames; };
typedef char EnArrowPlayerProjectileOffsetCheck[(offsetof(EnArrowPlayerView, projectileState) == 0x247E) ? 1 : -1];
typedef char EnArrowFramesOffsetCheck[(offsetof(EnArrowPlayFrameView, gameplayFrames) == 0x5BF4) ? 1 : -1];
struct EnArrowPackedRGBA {
    u32 r : 8;
    u32 g : 8;
    u32 b : 8;
    u32 a : 8;
    void Set(u8 red, u8 green, u8 blue, u8 alpha) { r = red; g = green; b = blue; a = alpha; }
};
typedef char EnArrowColorSizeCheck[(sizeof(EnArrowPackedRGBA) == 4) ? 1 : -1];
struct EnArrowManagedModel {
    char pad_000[0x3C];
    Vec3f pos;
    Vec3f scale;
    MTX34 rotation;
    char pad_084[0x6C];
    f32 primColor[4];
    char pad_100[0x70];
    s32 enabled;
};
typedef char EnArrowManagedModelEnabledCheck[(offsetof(EnArrowManagedModel, enabled) == 0x170) ? 1 : -1];
struct EnArrowBlureEffect { char pad[0x282]; u8 trailLength; };
f32 __hardfp_sinf(f32);
f32 __hardfp_cosf(f32);
void FUN_003429c8(void*, s32, f32*);
void FUN_00371eac(void*, s32);
void* FUN_00362384(s32);
void FUN_003620f0(void*, Vec3f*, Vec3f*);
s32 FUN_0033a480(PlayState*, void*, void*, Vec3f*, Vec3f*);
void EnArrow_Fly(EnArrow*, PlayState*);

static inline void EnArrow_RotateY(MTX34* m, f32 angle) {
    if (angle != 0.0f) {
        f32 sn = __hardfp_sinf(angle);
        f32 cs = __hardfp_cosf(angle);
        f32 a,b;
        a=m->data[0][0]; b=m->data[0][2]; m->data[0][0]=a*cs-b*sn; m->data[0][2]=a*sn+b*cs;
        a=m->data[1][0]; b=m->data[1][2]; m->data[1][0]=a*cs-b*sn; m->data[1][2]=a*sn+b*cs;
        a=m->data[2][0]; b=m->data[2][2]; m->data[2][0]=a*cs-b*sn; m->data[2][2]=a*sn+b*cs;
    }
}
static inline MTX34 EnArrow_RotationZ(f32 angle) {
    f32 sn,cs;
    if (angle != 0.0f) { sn=__hardfp_sinf(angle); cs=__hardfp_cosf(angle); }
    else { sn=0.0f; cs=1.0f; }
    MTX34 m;
    m.data[0][0]=cs; m.data[0][1]=-sn; m.data[0][2]=0.0f; m.data[0][3]=0.0f;
    m.data[1][0]=sn; m.data[1][1]=cs; m.data[1][2]=0.0f; m.data[1][3]=0.0f;
    m.data[2][0]=0.0f; m.data[2][1]=0.0f; m.data[2][2]=1.0f; m.data[2][3]=0.0f;
    return m;
}
static inline void EnArrow_AddBlureVertex(EnArrow* self, PlayState* play, MTX34* mtx) {
    static Vector3f posAOffset(0.0f, 400.0f, 1500.0f);
    static Vector3f posBOffset(0.0f, -400.0f, 1500.0f);
    static Vector3f flameOffset(0.0f, 0.0f, -300.0f);
    Vec3f posA;
    Vec3f posB;
    s32 addVertex;
    VEC3TransformAsm((Vector3f*)&self->flamePos, mtx, &flameOffset);
    if (EnArrow_Fly == self->actionFunc) {
        VEC3TransformAsm((Vector3f*)&posA, mtx, &posAOffset);
        VEC3TransformAsm((Vector3f*)&posB, mtx, &posBOffset);
        if (self->actor.params <= 9) {
            addVertex = self->actor.params <= 5;
            if (self->hitActor == NULL) {
                addVertex &= FUN_0033a480(play, &self->collider, &self->weaponInfo, &posA, &posB);
            } else if (addVertex) {
                if (posA.x == self->weaponInfo.posA.x && posA.y == self->weaponInfo.posA.y &&
                    posA.z == self->weaponInfo.posA.z && posB.x == self->weaponInfo.posB.x &&
                    posB.y == self->weaponInfo.posB.y && posB.z == self->weaponInfo.posB.z) { addVertex = false; }
            }
            if (addVertex) {
                EnArrowBlureEffect* effect = (EnArrowBlureEffect*)FUN_00362384(self->effectIndex);
                effect->trailLength = 13;
                FUN_003620f0(effect, &posA, &posB);
            }
        }
    }
}
void EnArrow_Draw(Actor* actor, GameState* state) {
    EnArrow* self = (EnArrow*)actor;
    PlayState* play = (PlayState*)state;
    EnArrowPlayerView* player = (EnArrowPlayerView*)GET_PLAYER(play);
    u8 alpha;
    f32 scale;
    MTX34 mtx;
    if (!(((Player*)player)->stateFlags1 & 0x40) || self->actor.parent != NULL) {
        if (!(((Player*)player)->stateFlags2 & 0x8000000) || self->actor.parent != NULL) {
            goto regularDraw;
        }
    }
    {
        if (self->actor.params == 10) { goto drawProjectile; }
        if (player->projectileState == 0 && self->actionFunc == EnArrow_Shoot) { self->drawEnabled = false; }
    }
regularDraw:
    if (self->actor.params < 9) {
        if (self->drawEnabled) { SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, NULL, NULL, &self->actor, 0); }
        ((EnArrowSkelModelState*)self->skelAnime.unk_28)->flagAD = 1;
        MTX34CopyAsm(&mtx, &self->actor.modelMtx);
    } else {
    drawProjectile:
        if (self->actor.speedXZ != 0.0f) {
            alpha = Math_CosS(self->timer * 5000) * 127.5f + 127.5f;
            EnArrowPackedRGBA prim;
            EnArrowPackedRGBA env;
            if (self->actor.params == 9) {
                prim.Set(255,255,255,255);
                env.Set(0,255,255,alpha);
                scale = 50.0f;
            } else {
                prim.Set(12,0,0,255);
                env.Set(250,250,0,alpha);
                scale = 150.0f;
            }
            MTX34CopyAsm(&mtx, &self->actor.modelMtx);
            EnArrow_RotateY(&mtx, self->actor.world.rot.y * 9.58738E-5f);
            Vec3f pos;
            pos.x=self->actor.modelMtx.data[0][3];
            pos.y=self->actor.modelMtx.data[1][3];
            pos.z=self->actor.modelMtx.data[2][3];
            ((EnArrowManagedModel*)self->managedModel)->pos=pos;
            f32 angle = self->actor.speedXZ == 0.0f ? 0.0f :
                (((EnArrowPlayFrameView*)play)->gameplayFrames & 0xFF) * 4000 * 9.58738E-5f;
            ((EnArrowManagedModel*)self->managedModel)->rotation=EnArrow_RotationZ(angle);
            Vec3f s = self->actor.scale;
            f32 factor=scale*24.0f;
            s.x*=factor; s.y*=factor; s.z*=factor;
            ((EnArrowManagedModel*)self->managedModel)->scale=s;
            struct Color4f {f32 c[4];};
            Color4f difference={{(prim.r-env.r)/255.0f,(prim.g-env.g)/255.0f,(prim.b-env.b)/255.0f,1.0f}};
            Color4f color={{env.r/255.0f,env.g/255.0f,env.b/255.0f,env.a/255.0f}};
            Color4f* output=(Color4f*)((EnArrowManagedModel*)self->managedModel)->primColor;
            *output=difference;
            FUN_003429c8(self->managedModel,1,color.c);
            ((EnArrowManagedModel*)self->managedModel)->enabled=1;
            if (self->drawEnabled) { FUN_00371eac(self->managedModel,0); }
        }
    }
    EnArrow_AddBlureVertex(self,play,&mtx);
}


#endif
GLOBAL_ASM("asm/EnArrow_Draw.s")

#if 0 // Full Sol draft:2580/2580B327words.

#define R_UPDATE_RATE SREG(30)
void FUN_0010d850(EnArrow*, PlayState*);
void FUN_003cb1b8(EnArrow*, PlayState*);
void Animation_PlayOnce(SkelAnime*, s32);
void EffectSsStone1_Spawn(PlayState*, Vec3f*, s32, s32);
void EffectSsHitMark_SpawnCustomScale(PlayState*, s32, s32, s32, Vec3f*);
s32 BgCheck_EntityLineTest1(void*, Vec3f*, Vec3f*, Vec3f*, CollisionPoly**, s32, s32, s32, s32, s32*);
s32 FUN_003aa580(void*, Vec3f*, Vec3f*, Vec3f*, CollisionPoly**, s32, s32, s32, s32, s32*);
s32 FUN_00314c68(PlayState*, Actor*, CollisionPoly*, s32, Vec3f*);
DynaPolyActor* DynaPoly_GetActor(void*, s32);
s32 FUN_00314c14(void*, CollisionPoly*, s32);

static inline void EnArrow_Diff(Vec3f* a, Vec3f* b, Vec3f* out) {
    out->x = a->x - b->x;
    out->y = a->y - b->y;
    out->z = a->z - b->z;
}
static inline void EnArrow_Sum(Vec3f* a, Vec3f* b, Vec3f* out) {
    out->x = a->x + b->x;
    out->y = a->y + b->y;
    out->z = a->z + b->z;
}
static inline void EnArrow_CarryActor(EnArrow* self, PlayState* play) {
    CollisionPoly* hitPoly;
    Vec3f delta;
    Vec3f actorNextPos;
    Vec3f hitPos;
    f32 dot;
    f32 scale;
    s32 bgId;
    EnArrow_Diff(&self->actor.world.pos, &self->previousPos, &delta);
    dot = ((self->actor.world.pos.x - self->hitActor->world.pos.x) * delta.x) +
          ((self->actor.world.pos.y - self->hitActor->world.pos.y) * delta.y) +
          ((self->actor.world.pos.z - self->hitActor->world.pos.z) * delta.z);
    if (!(dot < 0.0f)) {
        scale = SQ(delta.x) + SQ(delta.y) + SQ(delta.z);
        if (!(scale < 1.0f)) {
            scale = dot / scale;
            delta.x *= scale;
            delta.y *= scale;
            delta.z *= scale;
            EnArrow_Sum(&self->hitActor->world.pos, &delta, &actorNextPos);
            if (BgCheck_EntityLineTest1(&play->colCtx, &self->hitActor->world.pos, &actorNextPos, &hitPos,
                                      &hitPoly, 1, 1, 1, 1, &bgId)) {
                self->hitActor->world.pos.x = hitPos.x + ((hitPos.x - actorNextPos.x < 0.0f) ? -1.0f : 1.0f);
                self->hitActor->world.pos.y = hitPos.y + ((hitPos.y - actorNextPos.y < 0.0f) ? -1.0f : 1.0f);
                self->hitActor->world.pos.z = hitPos.z + ((hitPos.z - actorNextPos.z < 0.0f) ? -1.0f : 1.0f);
            } else {
                Math_Vec3f_Copy(&self->hitActor->world.pos, &actorNextPos);
            }
        }
    }
}
static inline void EnArrow_Bounce(PlayState* play, EnArrow* self) {
    self->actionFunc = FUN_0010d850;
    Animation_PlayOnce(&self->skelAnime, 2);
    self->actor.world.rot.y += (s32)(24576.0f * (Rand_ZeroOne() - 0.5f)) + 0x8000;
    self->actor.velocity.y += self->actor.speedXZ * (0.4f + 0.4f * Rand_ZeroOne());
    self->actor.speedXZ *= 0.04f + 0.3f * Rand_ZeroOne();
    self->timer = (u8)(50.0f * (3.0f / R_UPDATE_RATE) + 0.5f);
    self->actor.gravity = -1.5f;
}

void EnArrow_Fly(EnArrow* self, PlayState* play) {
    CollisionPoly* hitPoly;
    s32 bgId;
    Vec3f hitPoint;
    Vec3f posCopy;
    s32 atTouched;
    u32 sfxId;
    Actor* hitActor;
    if (DECR(self->timer) == 0) {
        Actor_Kill(&self->actor);
        return;
    }
    if (self->timer < (7.2000003f * (3.0f / R_UPDATE_RATE) + 0.5f)) {
        self->actor.gravity = -0.4f;
    }
    atTouched = self->actor.params != 0 && self->actor.params < 10 &&
                (self->collider.base.atFlags & 2) && self->hitSuppressed == 0;
    if (atTouched || self->touchedPoly) {
        if (self->actor.params >= 9) {
            if (atTouched) {
                self->actor.world.pos.x = (self->actor.world.pos.x + self->actor.prevPos.x) * 0.5f;
                self->actor.world.pos.y = (self->actor.world.pos.y + self->actor.prevPos.y) * 0.5f;
                self->actor.world.pos.z = (self->actor.world.pos.z + self->actor.prevPos.z) * 0.5f;
            }
            if (self->actor.params == 10) {
                iREG(50) = -1;
                Actor_Spawn(&play->actorCtx, play, 0x56, self->actor.world.pos.x, self->actor.world.pos.y,
                            self->actor.world.pos.z, 0, 0, 0, 0, 1);
                sfxId = 0x0100011D;
            } else { sfxId = 0x01000117; }
            if (self->actor.params == 9 && self->touchedPoly != 0) {
                switch (self->touchedPoly) {
                    case 1: EffectSsStone1_Spawn(play, &self->actor.world.pos, 0, 2); break;
                    case 2: EffectSsStone1_Spawn(play, &self->actor.world.pos, 0, 1); break;
                    case 3: EffectSsStone1_Spawn(play, &self->actor.world.pos, 0, 0); break;
                }
            }
            Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 20, sfxId);
            Actor_Kill(&self->actor);
        } else {
            if (atTouched && self->collider.info.atHitInfo->elemType != 4) {
                hitActor = self->collider.base.at;
                if (hitActor->update != NULL && !(self->collider.base.atFlags & 4) && hitActor->flags & 0x4000) {
                    self->hitActor = hitActor;
                    EnArrow_CarryActor(self, play);
                    EnArrow_Diff(&hitActor->world.pos, &self->actor.world.pos, &self->hitOffset);
                    hitActor->flags |= 0x8000;
                    self->collider.base.atFlags &= ~2;
                    self->actor.speedXZ = self->actor.speedXZ * R_UPDATE_RATE / 6.0f;
                    self->actor.velocity.y = self->actor.velocity.y * R_UPDATE_RATE / 6.0f;
                } else {
                    self->hitFlags |= 1;
                    self->hitFlags |= 2;
                    if (self->collider.info.atHitInfo->bumperFlags & 2) {
                        self->actor.world.pos.x = self->collider.info.atHitInfo->bumper.hitPos.x;
                        self->actor.world.pos.y = self->collider.info.atHitInfo->bumper.hitPos.y;
                        self->actor.world.pos.z = self->collider.info.atHitInfo->bumper.hitPos.z;
                    }
                    EnArrow_Bounce(play, self);
                    Audio_PlayActorSound2(&self->actor, 0x01000106);
                }
            } else if (self->touchedPoly) {
                self->actionFunc = FUN_003cb1b8;
                Animation_PlayOnce(&self->skelAnime, 1);
                if (self->actor.params >= 0) {
                    self->timer = (u8)(60.0f * (3.0f / R_UPDATE_RATE) + 0.5f);
                } else {
                    self->timer = (u8)(20.0f * (3.0f / R_UPDATE_RATE) + 0.5f);
                }
                switch (self->touchedPoly) {
                    case 1: EffectSsHitMark_SpawnCustomScale(play, 0, 2, 150, &self->actor.world.pos); break;
                    case 2: EffectSsHitMark_SpawnCustomScale(play, 0, 1, 150, &self->actor.world.pos); break;
                    case 3: EffectSsHitMark_SpawnCustomScale(play, 0, 0, 150, &self->actor.world.pos); break;
                }
                Audio_PlayActorSound2(&self->actor, 0x01000107);
                self->hitFlags |= 1;
            }
        }
    } else {
        Math_Vec3f_Copy(&self->previousPos, &self->actor.world.pos);
        Actor_MoveForward(&self->actor);
        if ((self->touchedPoly = FUN_003aa580(&play->colCtx, &self->actor.prevPos, &self->actor.world.pos,
                            &hitPoint, &self->actor.wallPoly, 1, 1, 1, 1, &bgId))) {
            FUN_00314c68(play, &self->actor, self->actor.wallPoly, bgId, &hitPoint);
            Math_Vec3f_Copy(&posCopy, &self->actor.world.pos);
            Math_Vec3f_Copy(&self->actor.world.pos, &hitPoint);
            DynaPolyActor* dyna = DynaPoly_GetActor(&play->colCtx, bgId);
            if (dyna != NULL && dyna->actor.unk_19B != 0) {
                switch (dyna->actor.unk_19B) {
                    case 1: self->touchedPoly = 3; break;
                    case 2: self->touchedPoly = 2; break;
                    case 3: self->touchedPoly = 1; break;
                    case 4: self->touchedPoly = 0xFF; break;
                }
            } else if (FUN_00314c14(&play->colCtx, self->actor.wallPoly, bgId) == 0xB) {
                self->touchedPoly = 2;
            }
        }
        if (self->actor.params < 9) {
            self->actor.shape.rot.x = Math_Atan2S(self->actor.speedXZ, -self->actor.velocity.y);
        }
    }
    if (self->hitActor != NULL) {
        if (self->hitActor->update != NULL) {
            Vec3f posA;
            Vec3f posB;
            EnArrow_Sum(&self->previousPos, &self->hitOffset, &posA);
            EnArrow_Sum(&self->actor.world.pos, &self->hitOffset, &posB);
            if (BgCheck_EntityLineTest1(&play->colCtx, &posA, &posB, &hitPoint, &hitPoly, 1, 1, 1, 1, &bgId)) {
                self->hitActor->world.pos.x = hitPoint.x + ((hitPoint.x - posB.x < 0.0f) ? -1.0f : 1.0f);
                self->hitActor->world.pos.y = hitPoint.y + ((hitPoint.y - posB.y < 0.0f) ? -1.0f : 1.0f);
                self->hitActor->world.pos.z = hitPoint.z + ((hitPoint.z - posB.z < 0.0f) ? -1.0f : 1.0f);
                EnArrow_Diff(&self->hitActor->world.pos, &self->actor.world.pos, &self->hitOffset);
                self->hitActor->flags &= ~0x8000;
                self->hitActor = NULL;
            } else {
                EnArrow_Sum(&self->actor.world.pos, &self->hitOffset, &self->hitActor->world.pos);
            }
            if (self->touchedPoly && self->hitActor != NULL) {
                self->hitActor->flags &= ~0x8000;
                self->hitActor = NULL;
            }
        } else { self->hitActor = NULL; }
    }
}


#endif
GLOBAL_ASM("asm/EnArrow_Fly.s")

GLOBAL_ASM("asm/EnArrow_Shoot.s")
GLOBAL_ASM("asm/FUN_003aa580.s")


}
