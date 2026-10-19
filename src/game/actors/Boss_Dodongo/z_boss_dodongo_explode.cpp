/* Matching 3DS explosion action; owns the two original dust colours (8 bytes). */
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

/* These two original dust colours occupy 5144FC..514503.
 * Root placement is required before final link; no ASM data was edited here. */
static Color_RGBA8 sDodongoDustPrim={255,255,0,255};
static Color_RGBA8 sDodongoDustEnv={255,10,0,255};

void FUN_0013ff98(BossDodongo* self, PlayState* play) {
    Vec3f dustVel,dustAccel,dustPos;
    s16 i;
    f32 end;
    Math_SmoothStepToF(&self->unk_7D8,0.05f,1.0f,0.005f,0.0f);
    FUN_00370734(&self->skelAnime);
    if(self->unk_7AA==0) {
        for(i=0;i<30;i++) {
            dustVel.x=Rand_CenteredFloat(20.0f);
            dustVel.y=Rand_CenteredFloat(20.0f);
            dustVel.z=Rand_CenteredFloat(20.0f);
            dustAccel.x=dustVel.x*-0.1f; dustAccel.y=dustVel.y*-0.1f; dustAccel.z=dustVel.z*-0.1f;
            dustPos.x=self->actor.world.pos.x+dustVel.x*3.0f;
            dustPos.y=self->actor.world.pos.y+(90.0f+dustVel.y*3.0f);
            dustPos.z=self->actor.world.pos.z+dustVel.z*3.0f;
            FUN_00365d20(play,&dustPos,&dustVel,&dustAccel,&sDodongoDustPrim,&sDodongoDustEnv,500,10,10);
        }
        end=(f32)Animation_GetLastFrame(&self->skelAnime,BossDodongo_AnimTable[11]);
        FUN_00353020(&self->skelAnime,&BossDodongo_AnimTable[11],1.0f,0.0f,end,2,-5.0f);
        self->actionFunc=FUN_0013cb28;
        Actor_SetColorFilter(&self->actor,0,255,0,(s16)(end+100.0f));
        Audio_PlayActorSound2(&self->actor,0x01000100);
        Audio_PlayActorSound2(&self->actor,0x01000276);
        FUN_0036fca8(&self->actor,play,4,10);
        self->unk_764-=2;
        if(self->unk_764<1) self->unk_764=1;
        { Vec3f focus={self->actor.focus.pos.x,self->actor.focus.pos.y,self->actor.focus.pos.z};
          FUN_00365560(play,0,0,&focus,*(s16*)BossDodongo_AnimTable);
        }
    }
}
}
