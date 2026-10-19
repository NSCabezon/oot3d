/* Matching 928-byte native 3DS corner movement. */
/* Native 3DS corner movement; no generated statics enabled. */
extern "C" {
#include "z_boss_dodongo.hpp"

#include "model_manager.hpp"
#include "graphics/skeleton_animation_model.hpp"
#include "mtx34_trans.hpp"
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
f32 fabsf(f32); f32 sinf(f32); f32 cosf(f32);

static inline f32 BossDodongo_RollTargetSpeed() {
    return 15.0f;
}

void FUN_001ccc78(BossDodongo* self, PlayState* play) {
    Vec3f* corner;
    Vec3f dust;
    f32 dx,dz;
    self->actor.flags|=0x1000000;
    FUN_00370734(&self->skelAnime);
    if(self->unk_7AA==15) { self->actor.velocity.y=15.0f; Audio_PlayActorSound2(&self->actor,0x01000275); }
    if(self->unk_7AA==1) Audio_PlayActorSound2(&self->actor,0x010002BD);
    corner=&BossDodongo_Corners[self->unk_770];
    self->unk_7BC=3.0f;
    f32 zero;
    f32 one;
    one=1.0f;
    zero=0.0f;
    if(self->unk_7AA==0) {
        Math_SmoothStepToF(&self->unk_7B4,BossDodongo_RollTargetSpeed(),one,0.75f,zero);
        Math_SmoothStepToF(&self->actor.world.pos.x,corner->x,one,self->unk_7B4,zero);
        Math_SmoothStepToF(&self->actor.world.pos.z,corner->z,one,self->unk_7B4,zero);
        self->unk_794+=2000;
        if(self->actor.bgCheckFlags&1) {
            self->unk_7F8=7700.0f;
            Audio_PlayActorSound2(&self->actor,0x010002BE);
            if(!(self->unk_76E&7)) FUN_0036627c((u8*)play+0x364,2,1,8);
            if(!(self->unk_76E&1)) FUN_0036f00c(play,&self->actor,&self->actor.world.pos,40.0f,3,8.0f,500,10,0);
        }
    }
    dx=corner->x-self->actor.world.pos.x; dz=corner->z-self->actor.world.pos.z;
    Math_SmoothStepToF(&self->unk_7B8,1333.3333740234375f,one,self->unk_7BC*100.0f,zero);
    Math_SmoothStepToS(&self->actor.world.rot.y,(s16)(Math_FAtan2F(dx,dz)*10430.3779296875f),5,(s16)(self->unk_7B8*self->unk_7BC),0);
    if(fabsf(dx)<=15.0f && fabsf(dz)<=15.0f) {
        self->unk_778++;
        if(self->unk_778>=2) {
            if(self->unk_776) self->unk_772=1-self->unk_772;
            self->unk_7B4=self->unk_7B8=zero;
            self->unk_794=-21845;
            FUN_00181770(self);
            self->unk_7F8=9200.0f;
            self->actor.velocity.y=20.0f;
            Audio_PlayActorSound2(&self->actor,0x010002BC);
            FUN_0036627c((u8*)play+0x364,2,6,8);
            dust.x=self->actor.world.pos.x; dust.y=self->actor.world.pos.y+60.0f; dust.z=self->actor.world.pos.z;
            FUN_0037378c(play,&dust,250.0f,40,800,10,0);
            FUN_0036fca8(&self->actor,play,6,15);
        } else { self->actor.velocity.y=15.0f; Audio_PlayActorSound2(&self->actor,0x010002BD); }
        if(self->unk_772==0) { self->unk_770++; if(self->unk_770>3) self->unk_770=0; }
        else { self->unk_770--; if(self->unk_770<0) self->unk_770=3; }
    }
}
}
