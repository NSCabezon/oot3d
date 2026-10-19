extern "C" {
#include "z_en_floormas.hpp"

#define FLAGS 0x00000405

void EnFloormas_Init(Actor* actor, GameState* state);
void EnFloormas_Destroy(Actor* actor, GameState* state);
void EnFloormas_Update(Actor* actor, GameState* state);
void EnFloormas_Draw(Actor* actor, GameState* state);
void EnFloormas_DrawHighlighted(Actor* actor, GameState* state);
void EnFloormas_Stand(EnFloormas* self, GameState* state);
void EnFloormas_SetupStand(EnFloormas* self);
void FUN_00363d50(EnFloormas* self);
void FUN_0036f00c(PlayState* play, Actor* actor, Vec3f* pos, f32 radius, f32 scale, s32 count,
                  s32 life, s32 speed, s32 random);
s16 Actor_WorldYawTowardActor(Actor* actor, Actor* target);
f32 Actor_WorldDistXZToActor(Actor* actor, Actor* target);
void EnFloormas_Turn(EnFloormas* self, GameState* state);
void Animation_MorphToPlayOnce(SkelAnime* skelAnime, s32 idx, f32 morph);
s32 FUN_003731e0(SkelAnime* skelAnime);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_PlayLoopSetSpeed(SkelAnime* skelAnime, s32 idx, f32 speed);
void FUN_0036f4e4(SkelAnime* skelAnime, f32 speed);
s16 Rand_S16Offset_003702c8(s16 base, s16 range);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);
void FUN_00342be0(Actor* actor, s32 a, s32 b, f32 c, f32 d, f32 e, f32 f);
void SkelAnime_DrawOpa(SkelAnime* skelAnime, MTX34* modelMtx, OverrideLimbDrawOpa overrideLimbDraw,
                       PostLimbDrawOpa postLimbDraw, Actor* actor, s32 arg5);
s32 FUN_00378ab8(void* play, s32 limbIndex, MTX34* mtx, void* actor);
void FUN_0032d278(GameState* state, Actor* actor);
void EnFloormas_Run(EnFloormas* self, GameState* state);
void EnFloormas_BigWalk(EnFloormas* self, GameState* state);
void EnFloormas_Slide(EnFloormas* self, GameState* state);
void EnFloormas_SetupLand(EnFloormas* self);
void FUN_00156b70(EnFloormas* self, GameState* state);
void FUN_00178650(EnFloormas* self);
void EnFloormas_Land(EnFloormas* self, GameState* state);
void EnFloormas_Stand(EnFloormas* self, GameState* state);
void EnFloormas_Charge(EnFloormas* self, GameState* state);
void EnFloormas_Hover(EnFloormas* self, GameState* state);
void EnFloormas_SmWait(EnFloormas* self, GameState* state);
void FUN_00124124(EnFloormas* self, GameState* state);
void FUN_00121940(EnFloormas* self, GameState* state);
void FUN_003e1c50(EnFloormas* self, GameState* state);
void Animation_PlayOnce(SkelAnime* skelAnime, s32 idx);
void FUN_001c77b8(EnFloormas* self, GameState* state);
void FUN_00126ad4(EnFloormas* self, GameState* state);
void FUN_003dcee4(EnFloormas* self, GameState* state);
void Actor_Kill(Actor* actor);
void FUN_003641d0(Actor* actor);
void FUN_00364084(EnFloormas* self, GameState* state);
void Actor_SetScale(Actor* actor, f32 scale);
void Item_DropCollectibleRandom(PlayState* play, Actor* actor, Vec3f* pos, s16 params);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, s32 mode, f32 speed, f32 start, f32 end, f32 morph);
s32 Math_StepToF(f32* value, f32 target, f32 step);
s32 Math_ScaledStepToS(s16* value, s16 target, s16 scale);
s32 Math_StepToS(s16* value, s16 target, s16 step);
void FUN_00370084(s16* value, s16 target, s16 scale, s16 step);
s32 FUN_003731e0(SkelAnime* skelAnime);

f32* EnFloormas_PlaySpeedPtr(SkelAnime* skelAnime);
f32* EnFloormas_CurFramePtr(SkelAnime* skelAnime);
f32* EnFloormas_EndFramePtr(SkelAnime* skelAnime);
void FUN_0037043c(SkelAnime* skelAnime, f32 endFrame);
void EnFloormas_Merge(EnFloormas* self, GameState* state);

void FUN_0010776c(EnFloormas* self, GameState* state);

typedef struct EnFloormasInitData {
    CollisionCheckInfoInit colInfo;
    ColliderCylinderInit cylinder;
    DamageTable damage;
    InitChainEntry chain[3];
} EnFloormasInitData;
typedef char EnFloormasInitDataSizeCheck[(sizeof(CollisionCheckInfoInit) == 8 &&
                                       sizeof(ColliderCylinderInit) == 56 &&
                                       sizeof(EnFloormasInitData) == 108 && sizeof(EnFloormas) == 0x83C) ? 1 : -1];
extern EnFloormasInitData EnFloormas_sInitData;
extern InitChainEntry EnFloormas_sInitChain[3];
extern ColliderCylinderInit EnFloormas_sCylinderInit;
extern CollisionCheckInfoInit EnFloormas_sColChkInfo;
u8 EnFloormas_InitialHealth(CollisionCheckInfoInit* info);
void EnFloormas_BigDecideAction(EnFloormas* self, GameState* state);
void FUN_0036f4d8(SkelAnime* skelAnime, f32 startFrame);
void FUN_0036f59c(Player* player, s32 sound);

typedef void (*EnFloormasActionFunc)(EnFloormas*, GameState*);

static inline void EnFloormas_MakeVulnerable(EnFloormas* self) {
    *(u8*)((u8*)self + 0x7F8) = 0;
    *(u8*)((u8*)self + 0x7F5) &= ~4;
    *(s16*)((u8*)self + 0x7DE) = 0;
}

static inline void EnFloormas_MakeInvulnerable(EnFloormas* self) {
    *(u8*)((u8*)self + 0x7F8) = 12;
    *(u8*)((u8*)self + 0x7F5) |= 4;
    *(s16*)((u8*)self + 0x7DE) = 40;
}

GLOBAL_ASM("data/z_En_Floormas.data.s")
GLOBAL_ASM("../src/game/actors/En_Floormas/data_syms.s")

ActorInit En_Floormas_InitVars = {
    ACTOR_EN_FLOORMAS,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_WALLMASTER,
    sizeof(EnFloormas),
    (ActorFunc)EnFloormas_Init,
    (ActorFunc)EnFloormas_Destroy,
    (ActorFunc)EnFloormas_Update,
    (ActorFunc)EnFloormas_Draw,
};

#if 0 // Sol group4 best: 11/139, 556/556B, 15 forms; all remaining differences are GPR choices.
void EnFloormas_Init(Actor* actor, GameState* state) {
    PlayState* play2 = (PlayState*)state;
    EnFloormas* self = (EnFloormas*)actor;
    PlayState* play = (PlayState*)state;
    EnFloormasActionFunc action;
    s32 params;
    s32 invisible;
    Actor_ProcessInitChain((Actor*)self, EnFloormas_sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 50.0f);
    FUN_00372f38(actor, (PlayState*)state, 0);
    FUN_00353c9c(actor, play2, (SkelAnime*)((u8*)self + 0x1A4), 0, 10,
                  (void*)((u8*)self + 0x228), (void*)((u8*)self + 0x500), 14);
    Collider_InitCylinder(play, (ColliderCylinder*)((u8*)self + 0x7E4));
    Collider_SetCylinder(play, (ColliderCylinder*)((u8*)self + 0x7E4), (Actor*)self, &EnFloormas_sCylinderInit);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, &((EnFloormasInitData*)&EnFloormas_sColChkInfo)->damage, &EnFloormas_sColChkInfo);
    *(s16*)((u8*)self + 0x7E0) = -1600;
    params = self->actor.params;
    invisible = params & 0x8000;
    params &= 0x7FFF;
    self->actor.params = params;
    if (invisible) {
        self->actor.flags |= 0x80;
        self->actor.draw = EnFloormas_DrawHighlighted;
    }
    if (params == 0x10) {
        self->actor.draw = NULL;
        self->actor.flags &= ~1;
        action = EnFloormas_SmWait;
    } else {
        self->actor.parent = Actor_Spawn(&play->actorCtx, play, 0x8E, self->actor.world.pos.x, self->actor.world.pos.y,
                                   self->actor.world.pos.z, 0, 0, 0, invisible + 0x10, 1);
        if (self->actor.parent == NULL) { Actor_Kill((Actor*)self); return; }
        self->actor.child = Actor_Spawn(&play->actorCtx, play, 0x8E, self->actor.world.pos.x, self->actor.world.pos.y,
                                  self->actor.world.pos.z, 0, 0, 0, invisible + 0x10, 1);
        if (self->actor.child == NULL) { Actor_Kill(self->actor.parent); Actor_Kill((Actor*)self); return; }
        self->actor.parent->child = (Actor*)self;
        self->actor.parent->parent = self->actor.child;
        self->actor.child->parent = (Actor*)self;
        self->actor.child->child = self->actor.parent;
        Animation_PlayOnce((SkelAnime*)((u8*)self + 0x1A4), 10);
        self->actor.speedXZ = 0.0f;
        action = EnFloormas_BigDecideAction;
    }
    *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = action;
}
#endif
GLOBAL_ASM("asm/EnFloormas_Init.s")

#if 0 // Sol group4 best: 35/300, 1200/1200B, 11 forms; GPR allocation remains.
void FUN_00375fd0(Actor*, ColliderInfo*, s32);
s32 Actor_ApplyDamage(Actor*);
void FUN_00375b70(PlayState*, Actor*);
void Actor_SetColorFilter(Actor*, u32, s32, s32, s32);
void FUN_00330254(PlayState*, Actor*, Vec3f*, s16, s16);
void FUN_00106fb8(EnFloormas*,GameState*);


static inline void EnFloormas_SetupFreeze(EnFloormas* self) {
    Animation_Change_00375c08((SkelAnime*)((u8*)self+0x1A4),2,2,1.5f,0.0f,20.0f,-3.0f);
    self->actor.speedXZ=0.0f;
    if(self->actor.colChkInfo.damageEffect==4) {
        Actor_SetColorFilter((Actor*)self,0x800000,255,0,80);
    } else {
        Actor_SetColorFilter((Actor*)self,0,255,0,80);
        if(self->actor.scale.x>0.004f) Audio_PlayActorSound2((Actor*)self,0x0100030E);
        else Audio_PlayActorSound2((Actor*)self,0x0100030E);
    }
    *(s16*)((u8*)self+0x7DC)=120;
    *(EnFloormasActionFunc*)((u8*)self+0x7D8)=FUN_003e1c50;
}
static inline void EnFloormas_SetupTakeDamage(EnFloormas* self) {
    Animation_MorphToPlayOnce((SkelAnime*)((u8*)self+0x1A4),0,-3.0f);
    if(**(u32**)((u8*)self+0x820)&0x1F824) self->actor.world.rot.y=(*(Actor**)((u8*)self+0x7EC))->world.rot.y;
    else self->actor.world.rot.y=Actor_WorldYawTowardActor((Actor*)self,*(Actor**)((u8*)self+0x7EC))+0x8000;
    Actor_SetColorFilter((Actor*)self,0x400000,255,0,20);
    self->actor.speedXZ=5.0f;
    self->actor.velocity.y=5.5f;
    *(EnFloormasActionFunc*)((u8*)self+0x7D8)=FUN_00121940;
}
static void EnFloormas_ColliderCheck(EnFloormas* self, GameState* state) {
    PlayState* play=(PlayState*)state;
    s32 isSmall;
    if(*(u8*)((u8*)self+0x7F5)&2) {
        *(u8*)((u8*)self+0x7F5)&=~2;
        FUN_00375fd0((Actor*)self,(ColliderInfo*)((u8*)self+0x7FC),1);
        if(self->actor.colChkInfo.damageEffect!=0 || self->actor.colChkInfo.damage!=0) {
            if(*(u8*)((u8*)self+0x7F8)!=12) {
                isSmall=(self->actor.scale.x<0.01f)?1:0;
                if(isSmall && (**(u32**)((u8*)self+0x820)&0x80)) {
                    self->actor.colChkInfo.damage=2;
                    self->actor.colChkInfo.damageEffect=0;
                }
                if(Actor_ApplyDamage((Actor*)self)==0) {
                    if(isSmall) Audio_PlayActorSound2((Actor*)self,0x010003A2);
                    else Audio_PlayActorSound2((Actor*)self,0x01000306);
                    FUN_00375b70(play,(Actor*)self);
                    self->actor.flags&=~1;
                } else if(self->actor.colChkInfo.damage!=0) Audio_PlayActorSound2((Actor*)self,0x01000305);
                if(self->actor.colChkInfo.damageEffect==4 || self->actor.colChkInfo.damageEffect==1) {
                    if(*(EnFloormasActionFunc*)((u8*)self+0x7D8)!=FUN_003e1c50) EnFloormas_SetupFreeze(self);
                } else {
                    if(self->actor.colChkInfo.damageEffect==2) {
                        s16 scale=(s16)(self->actor.scale.x*4000.0f);
                        FUN_00330254(play,(Actor*)self,&self->actor.world.pos,scale,scale);
                    }
                    EnFloormas_SetupTakeDamage(self);
                }
            }
        }
    }
}
void EnFloormas_Update(Actor* actor, GameState* state) {
    EnFloormas* self=(EnFloormas*)actor;
    PlayState* play=(PlayState*)state;
    if(*(EnFloormasActionFunc*)((u8*)self+0x7D8)!=EnFloormas_SmWait) {
        if(*(u8*)((u8*)self+0x7F4)&2) {
            *(u8*)((u8*)self+0x7F4)&=~2;
            actor->speedXZ*=-0.5f;
            actor->speedXZ=(actor->speedXZ>-5.0f)?-5.0f:actor->speedXZ;
            actor->velocity.y=5.0f;
            EnFloormas_SetupLand(self);
        }
        EnFloormas_ColliderCheck(self,state);
        (*(EnFloormasActionFunc*)((u8*)self+0x7D8))(self,state);
        if(*(EnFloormasActionFunc*)((u8*)self+0x7D8)!=FUN_00121940) actor->world.rot.y=actor->shape.rot.y;
        if(*(EnFloormasActionFunc*)((u8*)self+0x7D8)!=FUN_0010776c) Actor_MoveForward(actor);
        Actor_UpdateBgCheckInfo(play,actor,20.0f,actor->scale.x*3000.0f,0.0f,0x1D);
        Collider_UpdateCylinder(actor,(ColliderCylinder*)((u8*)self+0x7E4));
        if(*(EnFloormasActionFunc*)((u8*)self+0x7D8)==EnFloormas_Charge) {
            actor->flags|=0x1000000;
            CollisionCheck_SetAT(play,(CollisionCheckContext*)((u8*)play+0x5C78),(Collider*)((u8*)self+0x7E4));
        }
        if(*(EnFloormasActionFunc*)((u8*)self+0x7D8)!=FUN_0010776c) {
            if(*(EnFloormasActionFunc*)((u8*)self+0x7D8)!=FUN_00106fb8 &&
               *(EnFloormasActionFunc*)((u8*)self+0x7D8)!=FUN_00121940 && actor->freezeTimer==0)
                CollisionCheck_SetAC(play,(CollisionCheckContext*)((u8*)play+0x5C78),(Collider*)((u8*)self+0x7E4));
            if(*(EnFloormasActionFunc*)((u8*)self+0x7D8)!=FUN_00126ad4 || *EnFloormas_CurFramePtr((SkelAnime*)((u8*)self+0x1A4))<20.0f)
                CollisionCheck_SetOC(play,(CollisionCheckContext*)((u8*)play+0x5C78),(Collider*)((u8*)self+0x7E4));
        }
        Actor_SetFocus(actor,actor->scale.x*2500.0f);
        if(*(u8*)((u8*)self+0x7F8)==12) {
            if(*(s16*)((u8*)self+0x7DE)!=0) --*(s16*)((u8*)self+0x7DE);
            if(*(s16*)((u8*)self+0x7DE)==0) *(s16*)((u8*)self+0x7DE)=40;
        }
    }
}
#endif
GLOBAL_ASM("asm/EnFloormas_Update.s")

void EnFloormas_Draw(Actor* actor, GameState* state) {
    EnFloormas* self = (EnFloormas*)actor;
    FUN_00357fd0(GET_PLAYER((PlayState*)state), actor->unk_178, &actor->world.pos);
    if (*(u8*)((u8*)self + 0x7F8) == 12) {
        s32 type = 0;
        s32 mode = 5;
        f32 value = (f32)*(s16*)((u8*)self + 0x7DE) * 0.025f;
        FUN_00342be0(actor, mode, type, 0.0f, value * 0.6f, 0.0f, 1.0f);
    }
    SkelAnime_DrawOpa((SkelAnime*)((u8*)self + 0x1A4), (MTX34*)((u8*)self + 0x148),
                      (OverrideLimbDrawOpa)FUN_00378ab8, NULL, actor, 0);
    if (*(u8*)((u8*)self + 0x7F8) == 12) {
        FUN_0032d278((GameState*)state, actor);
    }
}


void EnFloormas_BigDecideAction(EnFloormas* self, GameState* state) {
    if (FUN_003731e0((SkelAnime*)((u8*)self + 0x1A4))) {
        if (*(f32*)((u8*)self + 0x98) < 400.0f && !Actor_IsFacingPlayer((Actor*)self, 0x4000)) {
            *(s16*)((u8*)self + 0x7DE) = *(s16*)((u8*)self + 0x92);
            FUN_00363d50(self);
        } else if (*(f32*)((u8*)self + 0x98) < 280.0f && Actor_IsFacingPlayer((Actor*)self, 0x2000)) {
            Animation_Change_00375c08((SkelAnime*)((u8*)self + 0x1A4), 9, 2, 3.0f, 0.0f,
                                      (f32)Animation_GetLastFrame((SkelAnime*)((u8*)self + 0x1A4), 9), -3.0f);
            *(f32*)((u8*)self + 0x6C) = 0.0f;
            *(f32*)((u8*)self + 0x70) = 0.0f;
            *(u8*)((u8*)self + 0x7F8) = 12;
            *(u8*)((u8*)self + 0x7F5) |= 4;
            *(s16*)((u8*)self + 0x7DE) = 40;
            FUN_0036f00c((PlayState*)state, (Actor*)self, (Vec3f*)((u8*)self + 0x28), 15.0f, 20.0f,
                         6, 300, 100, 1);
            Audio_PlayActorSound2((Actor*)self, 0x010003A0);
            *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = EnFloormas_Hover;
        } else {
            Animation_MorphToPlayOnce((SkelAnime*)((u8*)self + 0x1A4), 11, -3.0f);
            *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = EnFloormas_Stand;
        }
    }
}

void EnFloormas_BigWalk(EnFloormas* self, GameState* state) {
    FUN_003731e0((SkelAnime*)((u8*)self + 0x1A4));
    s32 animPastFrame = Animation_OnFrameImpl((SkelAnime*)((u8*)self + 0x1A4), 0.0f, 1.0f);
    if (animPastFrame) {
        if (*(s16*)((u8*)self + 0x7DC) != 0) {
            --*(s16*)((u8*)self + 0x7DC);
        }
    }
    if (animPastFrame || Animation_OnFrameImpl((SkelAnime*)((u8*)self + 0x1A4), 12.0f, 1.0f) ||
               Animation_OnFrameImpl((SkelAnime*)((u8*)self + 0x1A4), 24.0f, 1.0f) ||
               Animation_OnFrameImpl((SkelAnime*)((u8*)self + 0x1A4), 36.0f, 1.0f)) {
        Audio_PlayActorSound2((Actor*)self, 0x01000304);
    }
    if (*(f32*)((u8*)self + 0x98) < 320.0f && Actor_IsFacingPlayer((Actor*)self, 0x4000)) {
        *(s16*)((u8*)self + 0x7DC) = 0;
        *(f32*)((u8*)self + 0x6C) = 5.0f;
        FUN_0036f4e4((SkelAnime*)((u8*)self + 0x1A4), 3.0f);
        *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = EnFloormas_Run;
    } else if (*(u16*)((u8*)self + 0x90) & 8) {
        *(s16*)((u8*)self + 0x7DE) = *(s16*)((u8*)self + 0x82);
        FUN_00363d50(self);
    } else if (*(f32*)((u8*)self + 0x98) < 400.0f &&
               !Actor_IsFacingPlayer((Actor*)self, 0x4000)) {
        *(s16*)((u8*)self + 0x7DE) = *(s16*)((u8*)self + 0x92);
        FUN_00363d50(self);
    } else if (*(s16*)((u8*)self + 0x7DC) == 0) {
        Animation_PlayOnce((SkelAnime*)((u8*)self + 0x1A4), 8);
        *(f32*)((u8*)self + 0x6C) = 0.0f;
        *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = FUN_00156b70;
    }
}


void EnFloormas_Charge(EnFloormas* self, GameState* state) {
    if (*(s16*)((u8*)self + 0x7DC) != 0) {
        --*(s16*)((u8*)self + 0x7DC);
    }
    Math_StepToF((f32*)((u8*)self + 0x6C), 15.0f,
                *(f32*)((u8*)self + 0x6C) * *(f32*)((u8*)self + 0x6C) * (1.0f / 3.0f));
    Math_ScaledStepToS((s16*)((u8*)self + 0xBC), -0x1680, 0x140);
    f32 floorHeight;
    f32 distFromGround;
    distFromGround = *(f32*)((u8*)self + 0x2C);
    floorHeight = *(f32*)((u8*)self + 0x84);
    distFromGround -= floorHeight;
    if (distFromGround < 10.0f) {
        *(f32*)((u8*)self + 0x70) = 0.0f;
        *(f32*)((u8*)self + 0x64) = 0.0f;
        *(f32*)((u8*)self + 0x2C) = floorHeight + 10.0f;
    }
    if (distFromGround < 12.0f) {
        EnFloormas_Slide(self, state);
    }
    if ((*(u16*)((u8*)self + 0x90) & 8) || *(s16*)((u8*)self + 0x7DC) == 0) {
        EnFloormas_SetupLand(self);
    }
}


void EnFloormas_DrawHighlighted(Actor* actor, GameState* state) {
    EnFloormas* self = (EnFloormas*)actor;
    FUN_00357fd0(GET_PLAYER((PlayState*)state), actor->unk_178, &actor->world.pos);
    if (*(u8*)((u8*)self + 0x7F8) == 12) {
        s32 type = 0;
        s32 mode = 5;
        f32 value = (f32)*(s16*)((u8*)self + 0x7DE) * 0.025f;
        FUN_00342be0(actor, mode, type, 0.0f, value * 0.6f, 0.0f, 1.0f);
    }
    SkelAnime_DrawOpa((SkelAnime*)((u8*)self + 0x1A4), (MTX34*)((u8*)self + 0x148),
                      (OverrideLimbDrawOpa)FUN_00378ab8, NULL, actor, 0);
    if (*(u8*)((u8*)self + 0x7F8) == 12) {
        FUN_0032d278((GameState*)state, actor);
    }
}


void EnFloormas_Hover(EnFloormas* self, GameState* state) {
    if (FUN_003731e0((SkelAnime*)((u8*)self + 0x1A4))) {
        *(s16*)((u8*)self + 0x7DC) = 0x26;
        *(f32*)((u8*)self + 0x70) = -0.15f;
        *(f32*)((u8*)self + 0x6C) = 0.5f;
        *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = EnFloormas_Charge;
    }
    *(s16*)((u8*)self + 0xBC) += 0x140;
    *(f32*)((u8*)self + 0x2C) += 10.0f;
    FUN_00370084((s16*)((u8*)self + 0xBE), *(s16*)((u8*)self + 0x92), 3, 0xAAA);
    Math_StepToS((s16*)((u8*)self + 0x7E0), 1200, 100);
}

void EnFloormas_Land(EnFloormas* self, GameState* state) {
    s32 isOnGround = *(u16*)((u8*)self + 0x90) & 1;
    if (*(u16*)((u8*)self + 0x90) & 2) {
        if (*(s16*)((u8*)self + 0x1C) != 0x40) {
            EnFloormas_MakeVulnerable(self);
        }
        if (*(f32*)((u8*)self + 0x64) < -4.0f) {
            if (*(f32*)((u8*)self + 0x54) > 0.004f) {
                Audio_PlayActorSound2((Actor*)self, 0x01000303);
            } else {
                Audio_PlayActorSound2((Actor*)self, 0x010003A7);
            }
        }
    }
    if (*(u16*)((u8*)self + 0x90) & 8) {
        *(f32*)((u8*)self + 0x6C) = 0.0f;
    }
    if (isOnGround) {
        Math_StepToF((f32*)((u8*)self + 0x6C), 0.0f, 2.0f);
    }
    if (*(f32*)((u8*)self + 0x6C) > 0.0f &&
        *(f32*)((u8*)self + 0x2C) - *(f32*)((u8*)self + 0x84) < 12.0f) {
        EnFloormas_Slide(self, state);
    }
    if (FUN_003731e0((SkelAnime*)((u8*)self + 0x1A4))) {
        if (*(s16*)((u8*)self + 0x7DC) != 0) {
            --*(s16*)((u8*)self + 0x7DC);
        }
        if (*(s16*)((u8*)self + 0x7DC) == 0 && isOnGround) {
            if (*EnFloormas_EndFramePtr((SkelAnime*)((u8*)self + 0x1A4)) < 45.0f) {
                FUN_0037043c((SkelAnime*)((u8*)self + 0x1A4),
                            (f32)Animation_GetLastFrame((SkelAnime*)((u8*)self + 0x1A4), 2));
            } else if (*(s16*)((u8*)self + 0x1C) == 0x40) {
                Animation_PlayOnce((SkelAnime*)((u8*)self + 0x1A4), 10);
                *(s16*)((u8*)self + 0x7DC) = 0;
                *(s16*)((u8*)self + 0x7E2) += 1500;
                EnFloormas_MakeInvulnerable(self);
                *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = EnFloormas_Merge;
            } else {
                EnFloormas_SetupStand(self);
                *(s16*)((u8*)self + 0x7E2) = 75;
            }
        }
    }
    Math_ScaledStepToS((s16*)((u8*)self + 0xBC), 0, 0x140);
    Math_StepToS((s16*)((u8*)self + 0x7E0), -1600, 100);
}

#if 0 // group3 best: 6/140 words; 560B matches ROM size; required real data aliases recorded in STATUS
extern const ColliderCylinderInit EnFloormas_sCylinderInit;
extern const CollisionCheckInfoInit EnFloormas_sColChkInfo;
void FUN_003d8b54(EnFloormas* self, GameState* state);
void FUN_00373264(Actor* actor, s32 sound);

void EnFloormas_Merge(EnFloormas* self, GameState* state) {
    EnFloormas* parent;
    EnFloormas* child;
    s32 mergeCount = 0;
    f32 prevScale;
    f32 curScale;
    if (*(s16*)((u8*)self + 0x7E2) != 0) {
        --*(s16*)((u8*)self + 0x7E2);
    }
    parent = *(EnFloormas**)((u8*)self + 0x124);
    child = *(EnFloormas**)((u8*)self + 0x128);
    if (*(s16*)((u8*)self + 0x7E2) == 0) {
        if (*(EnFloormasActionFunc*)((u8*)parent + 0x7D8) != EnFloormas_SmWait) {
            FUN_00364084(parent, state);
        }
        if (*(EnFloormasActionFunc*)((u8*)child + 0x7D8) != EnFloormas_SmWait) {
            FUN_00364084(child, state);
        }
    } else {
        if (*(EnFloormasActionFunc*)((u8*)parent + 0x7D8) != EnFloormas_SmWait &&
            *(EnFloormasActionFunc*)((u8*)parent + 0x7D8) != FUN_003d8b54) {
            ++mergeCount;
        }
        if (*(EnFloormasActionFunc*)((u8*)child + 0x7D8) != EnFloormas_SmWait &&
            *(EnFloormasActionFunc*)((u8*)child + 0x7D8) != FUN_003d8b54) {
            ++mergeCount;
        }
    }
    prevScale = ((Actor*)self)->scale.x;
    if (mergeCount == 1) {
        Math_StepToF(&((Actor*)self)->scale.x, 0.007f, 0.0005f);
    } else if (mergeCount == 0) {
        Math_StepToF(&((Actor*)self)->scale.x, 0.01f, 0.0005f);
    }
    curScale = ((Actor*)self)->scale.x;
    ((Actor*)self)->scale.y = ((Actor*)self)->scale.z = curScale;
    if ((prevScale == 0.007f || prevScale == 0.004f) && prevScale != ((Actor*)self)->scale.x) {
        Audio_PlayActorSound2((Actor*)self, 0x010003A4);
    }
    ((ColliderCylinder*)((u8*)self + 0x7E4))->dim.radius =
        (EnFloormas_sCylinderInit.dim.radius * ((Actor*)self)->scale.x) * 100.0f;
    ((ColliderCylinder*)((u8*)self + 0x7E4))->dim.height =
        (EnFloormas_sCylinderInit.dim.height * ((Actor*)self)->scale.x) * 100.0f;
    if (FUN_003731e0((SkelAnime*)((u8*)self + 0x1A4))) {
        if (((Actor*)self)->scale.x >= 0.01f) {
            ((Actor*)self)->flags &= ~0x10;
            EnFloormas_MakeVulnerable(self);
            ((ColliderCylinder*)((u8*)self + 0x7E4))->info.bumperFlags |= 4;
            *(s16*)((u8*)self + 0x1C) = 0;
            *(u8*)((u8*)self + 0xB7) = EnFloormas_sColChkInfo.health;
            EnFloormas_SetupStand(self);
        } else {
            if (*(s16*)((u8*)self + 0x7DC) == 0) {
                Animation_PlayOnce((SkelAnime*)((u8*)self + 0x1A4), 6);
                *(s16*)((u8*)self + 0x7DC) = 1;
            } else {
                Animation_PlayOnce((SkelAnime*)((u8*)self + 0x1A4), 10);
                *(s16*)((u8*)self + 0x7DC) = 0;
            }
        }
    }
    FUN_00373264((Actor*)self, 0x010003A3);
}
#endif
GLOBAL_ASM("asm/EnFloormas_Merge.s")

void EnFloormas_Run(EnFloormas* self, GameState* state) {
    FUN_003731e0((SkelAnime*)((u8*)self + 0x1A4));
    if (Animation_OnFrameImpl((SkelAnime*)((u8*)self + 0x1A4), 0.0f, 1.0f) ||
        Animation_OnFrameImpl((SkelAnime*)((u8*)self + 0x1A4), 12.0f, 1.0f) ||
        Animation_OnFrameImpl((SkelAnime*)((u8*)self + 0x1A4), 24.0f, 1.0f) ||
        Animation_OnFrameImpl((SkelAnime*)((u8*)self + 0x1A4), 36.0f, 1.0f)) {
        Audio_PlayActorSound2((Actor*)self, 0x01000304);
    }
    FUN_00370084((s16*)((u8*)self + 0xBE), *(s16*)((u8*)self + 0x92), 3, 0x71C);
    if (*(f32*)((u8*)self + 0x98) < 280.0f && Actor_IsFacingPlayer((Actor*)self, 0x2000) &&
        !(*(u16*)((u8*)self + 0x90) & 8)) {
        Animation_Change_00375c08((SkelAnime*)((u8*)self + 0x1A4), 9, 2, 3.0f, 0.0f,
                                  (f32)Animation_GetLastFrame((SkelAnime*)((u8*)self + 0x1A4), 9), -3.0f);
        *(f32*)((u8*)self + 0x6C) = 0.0f;
        *(f32*)((u8*)self + 0x70) = 0.0f;
        *(u8*)((u8*)self + 0x7F8) = 12;
        *(u8*)((u8*)self + 0x7F5) |= 4;
        *(s16*)((u8*)self + 0x7DE) = 40;
        FUN_0036f00c((PlayState*)state, (Actor*)self, (Vec3f*)((u8*)self + 0x28), 15.0f, 20.0f,
                     6, 300, 100, 1);
        Audio_PlayActorSound2((Actor*)self, 0x010003A0);
        *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = EnFloormas_Hover;
    } else if (*(f32*)((u8*)self + 0x98) > 400.0f) {
        FUN_00178650(self);
    }
}

#if 0
void FUN_003e1c50(EnFloormas* self, GameState* state) {
    FUN_003731e0((SkelAnime*)((u8*)self + 0x1A4));
    u8* action = (u8*)self + 0x700;
    s16* timer = (s16*)(action + 0xDC);
    if (*timer != 0) {
        *timer = (s16)(*timer - 1);
        if (*timer != 0) {
            return;
        }
    }
    if (*(u8*)((u8*)self + 0xB7) == 0) {
        if (*(f32*)((u8*)self + 0x54) > 0.004f) {
            *(s16*)((u8*)self + 0xBE) = *(s16*)((u8*)self + 0x92) + 0x8000;
            FUN_003641d0((Actor*)*(EnFloormas**)((u8*)self + 0x128));
            FUN_003641d0((Actor*)*(EnFloormas**)((u8*)self + 0x124));
            FUN_003641d0((Actor*)self);
            Audio_PlayActorSound2((Actor*)self, 0x010003A5);
        } else {
            Item_DropCollectibleRandom((PlayState*)state, (Actor*)self, (Vec3f*)((u8*)self + 0x28), 0x90);
            FUN_00364084(self, state);
        }
    } else {
        Animation_MorphToPlayOnce((SkelAnime*)((u8*)self + 0x1A4), 11, -3.0f);
        *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = EnFloormas_Stand;
    }
}
#endif

void FUN_003d8b54(EnFloormas* self, GameState* state) {
    if (Math_StepToF((f32*)((u8*)self + 0x54), 0.0f, 0.0015f)) {
        EnFloormas* parent = *(EnFloormas**)((u8*)self + 0x124);
        EnFloormas* child = *(EnFloormas**)((u8*)self + 0x128);
        if (*(EnFloormasActionFunc*)((u8*)parent + 0x7D8) == EnFloormas_SmWait &&
            *(EnFloormasActionFunc*)((u8*)child + 0x7D8) == EnFloormas_SmWait) {
            Actor_Kill((Actor*)parent);
            Actor_Kill((Actor*)child);
            Actor_Kill((Actor*)self);
        } else {
            *(u32*)((u8*)self + 0x140) = 0;
            *(u32*)((u8*)self + 4) &= ~0x11;
            *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = EnFloormas_SmWait;
        }
    }
    *(f32*)((u8*)self + 0x5C) = *(f32*)((u8*)self + 0x54);
    *(f32*)((u8*)self + 0x58) = *(f32*)((u8*)self + 0x54);
}

void FUN_00106fb8(EnFloormas* self, GameState* state) {
    if (*(u16*)((u8*)self + 0x90) & 1) {
        if (FUN_003731e0((SkelAnime*)((u8*)self + 0x1A4))) {
            *(u32*)((u8*)self + 4) |= 1;
            *(s16*)((u8*)self + 0x7E2) = 75;
            Animation_MorphToPlayOnce((SkelAnime*)((u8*)self + 0x1A4), 11, -3.0f);
            *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = EnFloormas_Stand;
        }
        Math_StepToF((f32*)((u8*)self + 0x6C), 0.0f, 1.0f);
    }
    if (*(u16*)((u8*)self + 0x90) & 2) {
        Audio_PlayActorSound2((Actor*)self, 0x010003A7);
    }
}

void FUN_00363d50(EnFloormas* self) {
    *(f32*)((u8*)self + 0x6C) = 0.0f;
    s16 rotDelta = *(s16*)((u8*)self + 0x7DE) - *(s16*)((u8*)self + 0xBE);
    if (rotDelta > 0) {
        Animation_MorphToPlayOnce((SkelAnime*)((u8*)self + 0x1A4), 3, -3.0f);
    } else {
        Animation_Change_00375c08((SkelAnime*)((u8*)self + 0x1A4), 3, 2, -1.0f,
                                         (f32)Animation_GetLastFrame((SkelAnime*)((u8*)self + 0x1A4), 3), 0.0f,
                                         -3.0f);
    }
    if (*(f32*)((u8*)self + 0x54) > 0.004f) {
        *(s16*)((u8*)self + 0x7DE) = (s16)(rotDelta * (2.0f / 30.0f));
    } else {
        f32 playSpeed = *EnFloormas_PlaySpeedPtr((SkelAnime*)((u8*)self + 0x1A4));
        FUN_0036f4e4((SkelAnime*)((u8*)self + 0x1A4), playSpeed * 2.0f);
        *(s16*)((u8*)self + 0x7DE) = (s16)(rotDelta * (2.0f / 15.0f));
    }
    *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = EnFloormas_Turn;
}

#if 0 // Sol group4 best: 4/73, 292/292B, 15 cumulative forms; no guarded static.
void FUN_003641d0(Actor* actor) {
    EnFloormas* self = (EnFloormas*)actor;
    Actor_SetScale(actor, 0.004f);
    actor->flags |= 0x10;
    actor->targetArrowOffset *= 0.4f;
    actor->draw = (actor->flags & 0x80) ? EnFloormas_DrawHighlighted : EnFloormas_Draw;
    actor->shape.rot.y = actor->parent->shape.rot.y + 0x5555;
    *(Vector3f*)&actor->world.pos = *(Vector3f*)&actor->parent->world.pos;
    actor->params = 0x10;
    f32 endFrame = (f32)Animation_GetLastFrame((SkelAnime*)((u8*)self + 0x1A4), 2);
    Animation_Change_00375c08((SkelAnime*)((u8*)self + 0x1A4), 2, 2, 1.0f, 41.0f, endFrame, 0.0f);
    ((ColliderCylinder*)((u8*)self + 0x7E4))->dim.radius = EnFloormas_sCylinderInit.dim.radius * 0.6f;
    ((ColliderCylinder*)((u8*)self + 0x7E4))->dim.height = EnFloormas_sCylinderInit.dim.height * 0.6f;
    u8 bumperFlags = self->collider.info.bumperFlags;
    bumperFlags &= ~4;
    self->collider.info.bumperFlags = bumperFlags;
    *(f32*)((u8*)self + 0x6C) = 4.0f;
    actor->velocity.y = 7.0f;
    *(u8*)((u8*)self+0xB7) = EnFloormas_InitialHealth(&((EnFloormasInitData*)((u8*)&EnFloormas_sCylinderInit - sizeof(CollisionCheckInfoInit)))->colInfo) >> 1;
    *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = FUN_00106fb8;
}
#endif

void EnFloormas_Turn(EnFloormas* self, GameState* state) {
    f32 sinYaw;
    f32 cosYaw;
    if (FUN_003731e0((SkelAnime*)((u8*)self + 0x1A4))) {
        EnFloormas_SetupStand(self);
    }
    if (((*EnFloormas_PlaySpeedPtr((SkelAnime*)((u8*)self + 0x1A4)) > 0.0f) &&
         Animation_OnFrameImpl((SkelAnime*)((u8*)self + 0x1A4), 21.0f, 1.0f)) ||
        ((*EnFloormas_PlaySpeedPtr((SkelAnime*)((u8*)self + 0x1A4)) < 0.0f) &&
         Animation_OnFrameImpl((SkelAnime*)((u8*)self + 0x1A4), 6.0f, 1.0f))) {
        if (*(f32*)((u8*)self + 0x54) > 0.004f) {
            Audio_PlayActorSound2((Actor*)self, 0x01000304);
        } else {
            Audio_PlayActorSound2((Actor*)self, 0x010003A1);
        }
    }
    f32 curFrame = *EnFloormas_CurFramePtr((SkelAnime*)((u8*)self + 0x1A4));
    if (!curFrame) {
    }
    if (curFrame >= 7.0f && curFrame < 22.0f) {
        sinYaw = Math_SinS(*(s16*)((u8*)self + 0xBE) + 0x4268);
        cosYaw = Math_CosS(*(s16*)((u8*)self + 0xBE) + 0x4268);
        *(s16*)((u8*)self + 0xBE) += *(s16*)((u8*)self + 0x7DE);
        *(f32*)((u8*)self + 0x28) -= (*(f32*)((u8*)self + 0x54) * 2700.0f) *
                                    (Math_SinS(*(s16*)((u8*)self + 0xBE) + 0x4268) - sinYaw);
        *(f32*)((u8*)self + 0x30) -= (*(f32*)((u8*)self + 0x54) * 2700.0f) *
                                    (Math_CosS(*(s16*)((u8*)self + 0xBE) + 0x4268) - cosYaw);
    }
}






GLOBAL_ASM("asm/EnFloormas_Slide.s")

void EnFloormas_SmWait(EnFloormas* self, GameState* state) {}

void FUN_001e7e34(EnFloormas* self, GameState* state) {
    if (FUN_003731e0((SkelAnime*)((u8*)self + 0x1A4))) {
        Animation_MorphToPlayOnce((SkelAnime*)((u8*)self + 0x1A4), 11, -3.0f);
        *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = EnFloormas_Stand;
    }
}



void EnFloormas_Destroy(Actor* actor, GameState* state) {
    Collider_DestroyCylinder((PlayState*)state, (ColliderCylinder*)((u8*)actor + 0x7E4));
    SkelAnime_Free2((SkelAnime*)((u8*)actor + 0x1A4));
    FUN_003508b8(actor, 0);
}

 #if 0
void FUN_0036f4d8(SkelAnime* skelAnime, f32 value) {
    *(f32*)((u8*)skelAnime + 0x44) = value;
}
#endif



static inline void EnFloormas_TranslateLimb(Vector3f* offset, MTX34* mtx) {
    Matrix_UpdatePosition(mtx, mtx, (Vec3f*)offset);
}

s32 FUN_00378ab8(void* unused, s32 skip, MTX34* mtx, void* actor) {
    EnFloormas* self = (EnFloormas*)actor;
    if (skip == 0) {
        Vector3f offset(0.0f, 0.0f, (f32)*(s16*)((u8*)self + 0x7E0));
        EnFloormas_TranslateLimb(&offset, mtx);
    }
    return 0;
}


void EnFloormas_Stand(EnFloormas* self, GameState* state) {
    if (FUN_003731e0((SkelAnime*)((u8*)self + 0x1A4))) {
        if (*(f32*)((u8*)self + 0x54) > 0.004f) {
            if (*(EnFloormasActionFunc*)((u8*)self + 0x7D8) != EnFloormas_Run) {
                Animation_PlayLoopSetSpeed((SkelAnime*)((u8*)self + 0x1A4), 7, 1.5f);
            } else {
                FUN_0036f4e4((SkelAnime*)((u8*)self + 0x1A4), 1.5f);
            }
            *(s16*)((u8*)self + 0x7DC) = Rand_S16Offset_003702c8(2, 4);
            *(f32*)((u8*)self + 0x6C) = 1.5f;
            *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = EnFloormas_BigWalk;
        } else if (*(s16*)((u8*)self + 0x1C) == 0x20) {
            if (*(EnFloormasActionFunc*)((u8*)self + 0x7D8) != FUN_00124124) {
                Animation_PlayLoopSetSpeed((SkelAnime*)((u8*)self + 0x1A4), 7, 4.5f);
            }
            *(f32*)((u8*)self + 0x6C) = 5.0f;
            *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = FUN_001c77b8;
        } else {
            Animation_PlayLoopSetSpeed((SkelAnime*)((u8*)self + 0x1A4), 7, 4.5f);
            *(f32*)((u8*)self + 0x6C) = 5.0f;
            *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = FUN_00124124;
        }
    }
}

void FUN_001c77b8(EnFloormas* self, GameState* state) {
    FUN_003731e0((SkelAnime*)((u8*)self + 0x1A4));
    if (Animation_OnFrameImpl((SkelAnime*)((u8*)self + 0x1A4), 0.0f, 1.0f) ||
        Animation_OnFrameImpl((SkelAnime*)((u8*)self + 0x1A4), 18.0f, 1.0f)) {
        Audio_PlayActorSound2((Actor*)self, 0x010003A1);
    }
    if (*(u16*)((u8*)self + 0x90) & 8) {
        *(s16*)((u8*)self + 0x7DE) = *(s16*)((u8*)self + 0x82);
        FUN_00363d50(self);
        return;
    }
    if (*(s16*)((u8*)self + 0x1C) == 0x20) {
        EnFloormas* primary = *(EnFloormas**)((u8*)self + 0x124);
        if (*(s16*)((u8*)primary + 0x1C) != 0x40) {
            primary = *(EnFloormas**)((u8*)self + 0x128);
            if (*(s16*)((u8*)primary + 0x1C) != 0x40) {
                *(s16*)((u8*)self + 0x1C) = 0x10;
                return;
            }
        }
        Math_ScaledStepToS((s16*)((u8*)self + 0xBE), Actor_WorldYawTowardActor((Actor*)self, (Actor*)primary),
                           0x38E);
        if (Actor_WorldDistXZToActor((Actor*)self, (Actor*)primary) < 80.0f) {
            Animation_Change_00375c08((SkelAnime*)((u8*)self + 0x1A4), 2, 2, 2.0f, 0.0f, 41.0f, 0.0f);
            *(f32*)((u8*)self + 0x6C) = 0.0f;
            *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = (EnFloormasActionFunc)FUN_00126ad4;
        }
    } else {
        FUN_00370084((s16*)((u8*)self + 0xBE), *(s16*)((u8*)self + 0x92), 3, 0x71C);
        if (*(f32*)((u8*)self + 0x98) < 80.0f) {
            Animation_Change_00375c08((SkelAnime*)((u8*)self + 0x1A4), 2, 2, 2.0f, 0.0f, 41.0f, 0.0f);
            *(f32*)((u8*)self + 0x6C) = 0.0f;
            *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = (EnFloormasActionFunc)FUN_003dcee4;
        }
    }
}



#if 0 // batch 3 draft: 69/77, 544B vs 308B
void FUN_00124124(EnFloormas* self, GameState* state) {
    SkelAnime* skel = (SkelAnime*)((u8*)self + 0x1A4);
    s16* timer = (s16*)((u8*)self + 0x7E2);
    FUN_003731e0(skel);
    if (*timer != 0) {
        --*timer;
    }
    if (Animation_OnFrameImpl(skel, 0.0f, 1.0f) || Animation_OnFrameImpl(skel, 18.0f, 1.0f)) {
        Audio_PlayActorSound2((Actor*)self, 0x010003A1);
    }
    if (*timer == 0) {
        if (*(EnFloormasActionFunc*)((u8*)self + 0x7D8) != FUN_00124124) {
            Animation_PlayLoopSetSpeed(skel, 7, 4.5f);
        }
        *(f32*)((u8*)self + 0x6C) = 5.0f;
        *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = FUN_001c77b8;
    } else if (*(u16*)((u8*)self + 0x90) & 8) {
        *(s16*)((u8*)self + 0x7DE) = *(s16*)((u8*)self + 0x82);
        FUN_00363d50(self);
    } else if (*(f32*)((u8*)self + 0x98) < 120.0f) {
        Math_ScaledStepToS((s16*)((u8*)self + 0xBE), *(s16*)((u8*)self + 0x92) + 0x8000, 0x38E);
    }
}
#endif

#if 0 // batch 3 draft: 33/76, 184B vs 304B; death path still incomplete
void FUN_00121940(EnFloormas* self, GameState* state) {
    SkelAnime* skel = (SkelAnime*)((u8*)self + 0x1A4);
    if (FUN_003731e0(skel)) {
        if (*(u8*)((u8*)self + 0xB7) == 0) {
            FUN_003e1c50(self, state);
        } else {
            Animation_PlayOnce(skel, 1);
            *(s16*)((u8*)self + 0x36) = *(s16*)((u8*)self + 0xBE);
            *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = FUN_001e7e34;
            *(f32*)((u8*)self + 0x6C) = 0.0f;
            *(f32*)((u8*)self + 0x64) = 0.0f;
        }
    }
    if (Animation_OnFrameImpl(skel, 13.0f, 1.0f)) {
        Audio_PlayActorSound2((Actor*)self, 0x010002EB);
    }
    Math_StepToF((f32*)((u8*)self + 0x6C), 0.0f, 0.2f);
}
#endif
static inline void EnFloormas_SetupSmallWait(EnFloormas* self) {
    EnFloormas* parent = *(EnFloormas**)((u8*)self + 0x124);
    EnFloormas* child = *(EnFloormas**)((u8*)self + 0x128);
    if (*(EnFloormasActionFunc*)((u8*)parent + 0x7D8) == EnFloormas_SmWait &&
        *(EnFloormasActionFunc*)((u8*)child + 0x7D8) == EnFloormas_SmWait) {
        Actor_Kill((Actor*)parent);
        Actor_Kill((Actor*)child);
        Actor_Kill((Actor*)self);
        return;
    }
    ((Actor*)self)->draw = NULL;
    ((Actor*)self)->flags &= ~0x11;
    *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = EnFloormas_SmWait;
}

void FUN_00126ad4(EnFloormas* self, GameState* state) {
    Actor* primary;
    FUN_003731e0((SkelAnime*)((u8*)self + 0x1A4));
    if (((Actor*)self)->parent->params == 0x40) {
        primary = ((Actor*)self)->parent;
    } else if (((Actor*)self)->child->params == 0x40) {
        primary = ((Actor*)self)->child;
    } else {
        if (((Actor*)self)->bgCheckFlags & 2) {
            ((Actor*)self)->params = 0x10;
            EnFloormas_SetupLand(self);
        }
        return;
    }
    if (Animation_OnFrameImpl((SkelAnime*)((u8*)self + 0x1A4), 20.0f, 1.0f)) {
        *(f32*)((u8*)self + 0x6C) = 5.0f;
        ((Actor*)self)->velocity.y = 7.0f;
    } else if (*EnFloormas_CurFramePtr((SkelAnime*)((u8*)self + 0x1A4)) < 20.0f) {
        FUN_00370084(&((Actor*)self)->shape.rot.y, Actor_WorldYawTowardActor((Actor*)self, primary), 2, 0xE38);
    } else if (primary->world.pos.y - ((Actor*)self)->world.pos.y < -10.0f &&
               __fabsf(((Actor*)self)->world.pos.x - primary->world.pos.x) < 10.0f &&
               __fabsf(((Actor*)self)->world.pos.z - primary->world.pos.z) < 10.0f) {
        EnFloormas_SetupSmallWait(self);
        *(u8*)((u8*)self + 0x7F6) |= 1;
    } else if (((Actor*)self)->bgCheckFlags & 2) {
        *(f32*)((u8*)self + 0x6C) = 0.0f;
        Audio_PlayActorSound2((Actor*)self, 0x010003A7);
        EnFloormas_SetupLand(self);
    }
    if (__fabsf(((Actor*)self)->world.pos.x - primary->world.pos.x) < 5.0f &&
        __fabsf(((Actor*)self)->world.pos.z - primary->world.pos.z) < 5.0f) {
        Math_StepToF((f32*)((u8*)self + 0x6C), 0.0f, 2.0f);
    }
}
static inline void EnFloormas_SetupGrabLink(EnFloormas* self, Player* player) {
    f32 yDelta;
    f32 xzDelta;
    Animation_Change_00375c08((SkelAnime*)((u8*)self + 0x1A4), 2, 2, 1.0f, 36.0f, 45.0f, -3.0f);
    *(f32*)((u8*)self + 0x6C) = 0.0f;
    ((Actor*)self)->velocity.y = 0.0f;
    ((Actor*)self)->flags &= ~1;
    EnFloormas_MakeInvulnerable(self);
    if (gSaveContext.linkAge != 0) {
        yDelta = (-((Actor*)self)->yDistToPlayer < 20.0f) ? 20.0f :
                 ((-((Actor*)self)->yDistToPlayer > 30.0f) ? 30.0f : -((Actor*)self)->yDistToPlayer);
        xzDelta = -10.0f;
    } else {
        yDelta = (-((Actor*)self)->yDistToPlayer < 25.0f) ? 25.0f :
                 ((-((Actor*)self)->yDistToPlayer > 45.0f) ? 45.0f : -((Actor*)self)->yDistToPlayer);
        xzDelta = -70.0f;
    }
    ((Actor*)self)->world.pos.y = player->actor.world.pos.y + yDelta;
    ((Actor*)self)->world.pos.x = (xzDelta * 0.1f) * Math_SinS(((Actor*)self)->shape.rot.y) + player->actor.world.pos.x;
    ((Actor*)self)->world.pos.z = (xzDelta * 0.1f) * Math_CosS(((Actor*)self)->shape.rot.y) + player->actor.world.pos.z;
    ((Actor*)self)->shape.rot.x = -0x4CE0;
    *(EnFloormasActionFunc*)((u8*)self + 0x7D8) = FUN_0010776c;
}

void FUN_003dcee4(EnFloormas* self, GameState* state) {
    Player* player = GET_PLAYER((PlayState*)state);
    FUN_003731e0((SkelAnime*)((u8*)self + 0x1A4));
    if (*EnFloormas_CurFramePtr((SkelAnime*)((u8*)self + 0x1A4)) < 20.0f) {
        FUN_00370084(&((Actor*)self)->shape.rot.y, ((Actor*)self)->yawTowardsPlayer, 2, 0xE38);
    } else if (Animation_OnFrameImpl((SkelAnime*)((u8*)self + 0x1A4), 20.0f, 1.0f)) {
        *(f32*)((u8*)self + 0x6C) = 5.0f;
        ((Actor*)self)->velocity.y = 7.0f;
    } else if (((Actor*)self)->bgCheckFlags & 2) {
        *(f32*)((u8*)self + 0x6C) = 0.0f;
        *(s16*)((u8*)self + 0x7DC) = 75;
        Audio_PlayActorSound2((Actor*)self, 0x010003A7);
        EnFloormas_SetupLand(self);
    } else if (((Actor*)self)->yDistToPlayer < -10.0f &&
               (*(u8*)((u8*)self + 0x7F6) & 2) &&
               *(Actor**)((u8*)self + 0x7F0) == (Actor*)player &&
               !(player->stateFlags2 & 0x10)) {
        typedef s32 (*GrabPlayerFunc)(PlayState*, Player*);
        (*(GrabPlayerFunc*)((u8*)state + 0x5BA0))((PlayState*)state, player);
        EnFloormas_SetupGrabLink(self, player);
    }
}

#if 0 // Sol group4 best: 19/181, 720/724B, 2 forms; self-address ADR vs ROM LDR/pool.
void FUN_0010776c(EnFloormas* self, GameState* state) {
    Player* player = GET_PLAYER((PlayState*)state);
    EnFloormas* parent;
    EnFloormas* child;
    f32 yDelta;
    f32 xzDelta;
    if (FUN_003731e0((SkelAnime*)((u8*)self + 0x1A4))) {
        if (*EnFloormas_PlaySpeedPtr((SkelAnime*)((u8*)self + 0x1A4)) > 0.0f) {
            FUN_0036f4e4((SkelAnime*)((u8*)self + 0x1A4), -1.0f);
            FUN_0037043c((SkelAnime*)((u8*)self + 0x1A4), 36.0f);
            FUN_0036f4d8((SkelAnime*)((u8*)self + 0x1A4), 45.0f);
        } else {
            FUN_0036f4e4((SkelAnime*)((u8*)self + 0x1A4), 1.0f);
            FUN_0037043c((SkelAnime*)((u8*)self + 0x1A4), 45.0f);
            FUN_0036f4d8((SkelAnime*)((u8*)self + 0x1A4), 36.0f);
        }
    }
    if (gSaveContext.linkAge != 0) {
        yDelta = (-self->actor.yDistToPlayer < 20.0f) ? 20.0f :
                 ((-self->actor.yDistToPlayer > 30.0f) ? 30.0f : -self->actor.yDistToPlayer);
        xzDelta = -10.0f;
    } else {
        yDelta = (-self->actor.yDistToPlayer < 25.0f) ? 25.0f :
                 ((-self->actor.yDistToPlayer > 45.0f) ? 45.0f : -self->actor.yDistToPlayer);
        xzDelta = -30.0f;
    }
    self->actor.world.pos.y = player->actor.world.pos.y + yDelta;
    self->actor.world.pos.x = (xzDelta * 0.1f) * Math_SinS(self->actor.shape.rot.y) + player->actor.world.pos.x;
    self->actor.world.pos.z = (xzDelta * 0.1f) * Math_CosS(self->actor.shape.rot.y) + player->actor.world.pos.z;
    if (!(player->stateFlags2 & 0x80) || *(s8*)((u8*)player + 0x2488) < 0) {
        parent = (EnFloormas*)self->actor.parent;
        child = (EnFloormas*)self->actor.child;
        if ((*(EnFloormasActionFunc*)((u8*)parent + 0x7D8) == FUN_0010776c ||
             *(EnFloormasActionFunc*)((u8*)parent + 0x7D8) == EnFloormas_SmWait) &&
            (*(EnFloormasActionFunc*)((u8*)child + 0x7D8) == FUN_0010776c ||
             *(EnFloormasActionFunc*)((u8*)child + 0x7D8) == EnFloormas_SmWait)) {
            parent->actor.params = 0x20;
            child->actor.params = 0x20;
            self->actor.params = 0x40;
        }
        self->actor.shape.rot.x = 0;
        self->actor.velocity.y = 6.0f;
        self->actor.speedXZ = -3.0f;
        self->actor.flags |= 1;
        EnFloormas_SetupLand(self);
    } else if (*(s16*)((u8*)self + 0x7DE) % 20 == 0) {
        if (gSaveContext.linkAge != 0) {
            FUN_0036f59c(player, 0x01000510);
        } else {
            FUN_0036f59c(player, 0x010004F0);
        }
        typedef void (*DamagePlayerFunc)(PlayState*, s32);
        (*(DamagePlayerFunc*)((u8*)state + 0x5BAC))((PlayState*)state, -8);
    }
    Audio_PlayActorSound2((Actor*)self, 0x010003A6);
}
#endif

}
