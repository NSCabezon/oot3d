extern "C" {
#include "z_en_horse_normal.hpp"
#include "model_manager.hpp"
#include "mtx34_trans.hpp"
#include "regs.hpp"

#define FLAGS 0x00000000

void EnHorseNormal_Init(Actor* actor, GameState* state);
void EnHorseNormal_Destroy(Actor* actor, GameState* state);
void EnHorseNormal_Update(Actor* actor, GameState* state);
void EnHorseNormal_Draw(Actor* actor, PlayState* play);

GLOBAL_ASM("data/z_En_Horse_Normal.data.s")

GLOBAL_ASM("../src/game/actors/En_Horse_Normal/data_syms.s")

extern s32 EnHorseNormal_sAnimations[];
extern s32 EnHorseNormal_sStepFrames[];
extern s32 EnHorseNormal_sAnimClass[];
extern u8 EnHorseNormal_sRandomIdle[];
extern f32 EnHorseNormal_sPlaybackSpeeds[];
typedef void (*EnHorseNormalActionFunc)(EnHorseNormal*,PlayState*);
extern EnHorseNormalActionFunc EnHorseNormal_sActionFuncs[];
f32* EnHorse_CurFramePtr(SkelAnime*);
f32 FUN_00350c68(EnHorseNormal* self);
extern const u8 EnHorseNormal_sModelIndices[];
void* ZAR_GetCMBByIndex(void*,s32);
void FUN_00299ef8(Actor*,Lights*,PlayState*,s32,Color_RGBA8*);
void FUN_00358ea8(void*,PlayState*,SkelAnime*,void*,void*,s32,void*,void*,s32);
s32 Animation_GetLastFrame(SkelAnime*,s32);
void Animation_Change_00375c08(SkelAnime*,s32,f32,f32,f32,s32,f32);
void Animation_PlayOnce(SkelAnime*,s32);
void SkelAnime_SetPlaySpeed(SkelAnime*,f32);
void Collider_DestroyJntSph(PlayState*,void*);
void FUN_003406b8(Actor*,PlayState*,EnHorseNormalSkin*,void*,PostLimbDrawOpa,s32,s32,s32);
void FUN_003409ac(EnHorseNormalSkin*,s32,Vec3f*,Vec3f*);
void ActorShadow_Draw(Actor*,Lights*,PlayState*,s32,Color_RGBA8*);
void MTX34CopyAsm(MTX34*,MTX34*);
void FUN_002894e0(EnHorseNormal*,Lights*,PlayState*,s32,Color_RGBA8*);
void FUN_003a5778(MTX34*,Vec3f*,Vec3s*,Vec3f*);
f32 sinf(f32);
f32 cosf(f32);
static inline void EnHorseNormal_IdleSound(EnHorseNormal* self) {
 f32 frame = *EnHorse_CurFramePtr(&self->skin.skelAnime);
 if (self->animationIdx==0 && (Animation_OnFrameImpl(&self->skin.skelAnime,47.0f,0.6666667f) || Animation_OnFrameImpl(&self->skin.skelAnime,58.0f,0.6666667f)) && !(self->flags&1)) {
  if(!Animation_OnFrameImpl(&self->skin.skelAnime,47.0f,0.6666667f)) self->flags|=1;
  Audio_PlaySoundGeneral(0x01000183,&self->actor.world.pos,4,&FLOAT_0054ac20,&FLOAT_0054ac20,&DAT_0054ac24.unk_0);
 } else if(self->animationIdx==3 && frame>25.0f && !(self->flags&2)) {
  self->flags|=2; Audio_PlaySoundGeneral(0x01000182,&self->actor.world.pos,4,&FLOAT_0054ac20,&FLOAT_0054ac20,&DAT_0054ac24.unk_0);
 }
}
void EnHorseNormal_WaitClone(EnHorseNormal* self,PlayState* play) {
 EnHorseNormal_IdleSound(self);
 if(FUN_003731e0(&self->skin.skelAnime)) {
  f32 rand=Rand_ZeroOne();
  if(rand<0.4f) self->animationIdx=0;
  else if(rand<0.8f) { self->animationIdx=1;self->flags|=0x20;Audio_PlaySoundGeneral(0x0100016D,&self->soundPos,4,&FLOAT_0054ac20,&FLOAT_0054ac20,&DAT_0054ac24.unk_0); }
  else { self->animationIdx=3;self->flags|=0x20;Audio_PlaySoundGeneral(0x0100015C,&self->soundPos,4,&FLOAT_0054ac20,&FLOAT_0054ac20,&DAT_0054ac24.unk_0); }
  f32 last=Animation_GetLastFrame(&self->skin.skelAnime,EnHorseNormal_sAnimations[self->animationIdx]);
  Animation_Change_00375c08(&self->skin.skelAnime,EnHorseNormal_sAnimations[self->animationIdx],FUN_00350c68(self),0.0f,last,2,0.0f);
  self->flags&=~1;self->flags&=~2;self->flags&=~8;self->flags&=~0x10;
 }
}

void EnHorseNormal_CycleAnimations(EnHorseNormal* self,PlayState* play) {
 self->actor.speedXZ=0.0f;
 if(FUN_003731e0(&self->skin.skelAnime)) {
  self->action=0;
  self->animationIdx++;
  if(self->animationIdx>=9) self->animationIdx=0;
  Animation_PlayOnce(&self->skin.skelAnime,EnHorseNormal_sAnimations[self->animationIdx]);
 }
}

void EnHorseNormal_Wait(EnHorseNormal* self,PlayState* play) {
 if(FUN_003731e0(&self->skin.skelAnime)) {
  f32 rand=Rand_ZeroOne();
  if(rand<0.4f) self->animationIdx=0;
  else if(rand<0.8f) {
   self->animationIdx=1;
   Audio_PlaySoundGeneral(0x0100016D,&self->soundPos,4,&FLOAT_0054ac20,&FLOAT_0054ac20,&DAT_0054ac24.unk_0);
  } else {
   self->animationIdx=3;
   Audio_PlaySoundGeneral(0x0100015C,&self->soundPos,4,&FLOAT_0054ac20,&FLOAT_0054ac20,&DAT_0054ac24.unk_0);
  }
  f32 last=Animation_GetLastFrame(&self->skin.skelAnime,EnHorseNormal_sAnimations[self->animationIdx]);
  Animation_Change_00375c08(&self->skin.skelAnime,EnHorseNormal_sAnimations[self->animationIdx],FUN_00350c68(self),0.0f,last,2,0.0f);
 }
}

void EnHorseNormal_Update(Actor* actor,GameState* state) {
 EnHorseNormal* self=(EnHorseNormal*)actor;
 PlayState* play=(PlayState*)state;
 EnHorseNormal_sActionFuncs[self->action](self,play);
 Actor_MoveForward(actor);
 Actor_UpdateBgCheckInfo(play,actor,20.0f,35.0f,100.0f,0x1D);
 if(play->sceneNum==0x63) actor->world.pos.z=(actor->world.pos.z<=-2400.0f)?-2400.0f:actor->world.pos.z;
 actor->focus.pos=actor->world.pos;
 actor->focus.pos.y+=70.0f;
 self->soundPos=actor->unk_EC;
 self->soundPos.y+=120.0f;
 Collider_UpdateCylinder(actor,&self->bodyCollider);
 CollisionCheck_SetOC(play,&play->colChkCtx,&self->bodyCollider.base);
 actor->colChkInfo.mass=(actor->speedXZ==0.0f)?0xFF:0xFE;
}


#if 0 // Sol small pending: best remains 2 words, 124/124 B.
void EnHorseNormal_Destroy(Actor* actor,GameState* state) {
 EnHorseNormal* self=(EnHorseNormal*)actor;
 PlayState* play=(PlayState*)state;
 SkelAnime_Free2(&self->skin.skelAnime);
 if(self->action==3) {
  SkelAnime_Free2(&self->cloneSkin.skelAnime);
  if(self->shadowModel) {
   void (*destroyModel)(SkeletonAnimationModel*)=(void (*)(SkeletonAnimationModel*))((void**)self->shadowModel->vTable)[1];
   destroyModel(self->shadowModel);
  }
 }
 Collider_DestroyCylinder(play,&self->bodyCollider);
 Collider_DestroyCylinder(play,&self->cloneCollider);
 Collider_DestroyJntSph(play,&self->headCollider);
}
#endif
GLOBAL_ASM("asm/EnHorseNormal_Destroy.s")

void FUN_001c3180(Actor* actor,PlayState* play,EnHorseNormalSkin* skin,void* postDraw,PostLimbDrawOpa postLimb,s32 a,s32 b,s32 c) {
 FUN_003406b8(actor,play,skin,postDraw,postLimb,a,b,c);
}

#if 0 // Sol small pending: 2 words, full 200/200 B; copy argument scheduling remains.
static inline void EnHorseNormal_CopyMatrix(MTX34* src,MTX34* dst) { MTX34CopyAsm(dst,src); }
void FUN_00299ef8(Actor* actor,Lights* lights,PlayState* play,s32 arg3,Color_RGBA8* color) {
 EnHorseNormal* self=(EnHorseNormal*)actor;
 MTX34 saved;
 EnHorseNormal_CopyMatrix(&actor->modelMtx,&saved);
 s16 oldYaw=actor->world.rot.y;
 ActorShadow_Draw(actor,lights,play,1,NULL);
 EnHorseNormal_CopyMatrix(&self->cloneMtx,&actor->modelMtx);
 actor->world.pos.x=self->cloneMtx.data[0][3];
 actor->world.pos.y=self->cloneMtx.data[1][3];
 actor->world.pos.z=self->cloneMtx.data[2][3];
 actor->shape.rot.y=self->cloneRotY;
 FUN_002894e0(self,lights,play,1,NULL);
 EnHorseNormal_CopyMatrix(&saved,&actor->modelMtx);
 actor->world.pos.x=saved.data[0][3];
 actor->world.pos.y=saved.data[1][3];
 actor->world.pos.z=saved.data[2][3];
 actor->shape.rot.y=oldYaw;
}
#endif

class EnHorseNormalVecCopy {
 public:
 EnHorseNormalVecCopy(const Vec3f* p):x(p->x),y(p->y),z(p->z) {}
 f32 x,y,z;
};
void FUN_001c307c(Actor* actor,PlayState* play,EnHorseNormalSkin* skin) {
 Vec3f modelPos;
 Vec3f worldPos;
 s32 i;
 EnHorseNormal* self=(EnHorseNormal*)actor;
 for(i=0;i<self->headCollider.count;i++) {
  modelPos=*(Vec3f*)&EnHorseNormalVecCopy(&self->headCollider.elements[i].modelCenter);
  FUN_003409ac(skin,self->headCollider.elements[i].limb,&modelPos,&worldPos);
  self->headCollider.elements[i].worldCenter=worldPos;
  self->headCollider.elements[i].worldRadius=self->headCollider.elements[i].modelRadius*self->headCollider.elements[i].scale;
 }
 CollisionCheck_SetOC(play,&play->colChkCtx,&self->headCollider.base);
}

f32 fabsf(f32);

// Unmatched shadow draft: 135 words, 652/656 B; see STATUS.md.
#if 0
f32 sinf(f32); f32 cosf(f32);
void FUN_003687b4(CollisionPoly*,MTX34*,f32,f32,f32);
void FUN_00330b98(void*,SkeletonAnimationModel*,s32);
static inline void EnHorseNormal_Translate(MTX34* m,s32 mode,f32 x,f32 y,f32 z) {
 Vec3f t;t.x=x;t.y=y;t.z=z;
 if(mode==1) Matrix_UpdatePosition(m,m,&t);else MTX34_MakeTrans(m,&t);
}
static inline void EnHorseNormal_RotateY(MTX34* m,s32 mode,f32 angle) {
 if(mode==1 && angle!=0.0f) {
  f32 s=sinf(angle), c=cosf(angle);
  for(s32 r=0;r<3;r++) { f32 a=m->data[r][0],b=m->data[r][2];m->data[r][0]=a*c-b*s;m->data[r][2]=a*s+b*c; }
 }
}
static inline void EnHorseNormal_Scale(MTX34* m,s32 mode,f32 x,f32 y,f32 z) {
 for(s32 r=0;r<3;r++) m->data[r][0]*=x;
 for(s32 r=0;r<3;r++) m->data[r][1]*=y;
 for(s32 r=0;r<3;r++) m->data[r][2]*=z;
}
void FUN_002894e0(EnHorseNormal* self,Lights* lights,PlayState* play,s32 arg3,Color_RGBA8* color) {
 MTX34 mtx; f32 shadowHeight=0.8f;
 if(self->actor.shape.shadowAlpha!=0 && self->actor.floorPoly!=NULL) {
  f32 dy=self->actor.world.pos.y-self->actor.floorHeight;
  if(dy>=-50.0f && dy<500.0f) {
   if(dy<0.0f) dy=0.0f; else if(dy>150.0f) dy=150.0f;
   f32 factor=1.0f-dy*(1.0f/350.0f);
   FUN_003687b4(self->actor.floorPoly,&mtx,self->actor.world.pos.x,self->actor.floorHeight,self->actor.world.pos.z);
   EnHorseNormal_Translate(&mtx,1,0.0f,shadowHeight,0.0f);
   EnHorseNormal_RotateY(&mtx,1,self->actor.shape.rot.y*9.58738e-5f);
   factor=self->actor.shape.shadowScale*factor;
   EnHorseNormal_Scale(&mtx,1,self->actor.scale.x*factor,1.0f,self->actor.scale.z*factor);
   self->shadowModel->FUN_003721e0(&mtx);
   self->shadowModel->SetUnkAC(1);
   FUN_00330b98(GetModelManager().unk_180,self->shadowModel,0);
  }
 }
}

#endif

ActorInit En_Horse_Normal_InitVars = {
    ACTOR_EN_HORSE_NORMAL,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HORSE_NORMAL,
    sizeof(EnHorseNormal),
    (ActorFunc)EnHorseNormal_Init,
    (ActorFunc)EnHorseNormal_Destroy,
    (ActorFunc)EnHorseNormal_Update,
    (ActorFunc)EnHorseNormal_Draw,
};

// Unmatched Init draft: 32 words, 1468/1468 B.
#if 0
extern InitChainEntry EnHorseNormal_sInitChain[];
extern ColliderCylinderInit EnHorseNormal_sCylinder1;
extern ColliderCylinderInit EnHorseNormal_sCylinder2;
extern char EnHorseNormal_sJntSph[];
extern CollisionCheckInfoInit EnHorseNormal_sColInfo;
void ActorShadow_DrawHorse(Actor*,Lights*,PlayState*);
void Collider_InitJntSph(PlayState*,EnHorseNormalJntSph*);
void Collider_SetJntSph(PlayState*,EnHorseNormalJntSph*,Actor*,void*,void*);
void* ZAR_GetCMBByIndex(void*,s32);
void FUN_00358ea8(void*,PlayState*,SkelAnime*,void*,void*,s32,void*,void*,s32);
void Animation_PlayOnce(SkelAnime*,s32);
void SkelAnime_SetPlaySpeed(SkelAnime*,f32);
void FUN_00385c34(void*,PlayState*,EnHorseNormal*);
static inline void EnHorseNormal_InitSkin(void* zar,PlayState* play,EnHorseNormal* self) {
 FUN_00358ea8(zar,play,&self->skin.skelAnime,ZAR_GetCMBByIndex(zar,0),self->actor.unk_178,6,self->jointTable,self->morphTable,0x18);
 Animation_PlayOnce(&self->skin.skelAnime,EnHorseNormal_sAnimations[self->animationIdx]);
}
static inline void EnHorseNormal_SetupIdle(EnHorseNormal* self,u8 action) {
 self->action=action;self->animationIdx=0;self->actor.speedXZ=0.0f;self->accel=0.0f;self->yawDelta=0;self->yawVel=0;
 f32 last=Animation_GetLastFrame(&self->skin.skelAnime,EnHorseNormal_sAnimations[self->animationIdx]);
 Animation_Change_00375c08(&self->skin.skelAnime,EnHorseNormal_sAnimations[self->animationIdx],FUN_00350c68(self),0.0f,last,2,0.0f);
}
void EnHorseNormal_Init(Actor* actor,GameState* state) {
 EnHorseNormal* self=(EnHorseNormal*)actor;PlayState* play=(PlayState*)state;
 Actor_ProcessInitChain(actor,EnHorseNormal_sInitChain);
 Actor_SetScale(actor,0.01f);actor->gravity=-3.5f;
 ActorShape_Init(&actor->shape,0.0f,ActorShadow_DrawHorse,20.0f);
 self->action=0;actor->speedXZ=0.0f;actor->focus.pos=actor->world.pos;actor->focus.pos.y+=70.0f;self->animationIdx=0;
 Collider_InitCylinder(play,&self->bodyCollider);Collider_SetCylinder(play,&self->bodyCollider,actor,&EnHorseNormal_sCylinder1);
 Collider_InitJntSph(play,&self->headCollider);Collider_SetJntSph(play,&self->headCollider,actor,EnHorseNormal_sJntSph,self->headElements);
 Collider_InitCylinder(play,&self->cloneCollider);Collider_SetCylinder(play,&self->cloneCollider,actor,&EnHorseNormal_sCylinder2);
 CollisionCheck_SetInfo(&actor->colChkInfo,NULL,&EnHorseNormal_sColInfo);
 void* zar=&play->GetObjectStatus((u8)actor->objBankIndex)->zarInfo;
 if(play->sceneNum==0x63) {
  if(actor->world.rot.z==0 || gSaveContext.nightFlag!=0) {Actor_Kill(actor);return;}
  if(gSaveContext.linkAge!=0) {
   if(Flags_GetEventChkInf(0x14)) {if(actor->world.rot.z!=3){Actor_Kill(actor);return;}}
   else if(actor->world.rot.z!=1){Actor_Kill(actor);return;}
  } else if(Flags_GetEventChkInf(0x18) || DREG(1)) {if(actor->world.rot.z!=7){Actor_Kill(actor);return;}}
  else if(actor->world.rot.z!=5){Actor_Kill(actor);return;}
  actor->home.rot.z=actor->world.rot.z=actor->shape.rot.z=0;
  EnHorseNormal_InitSkin(zar,play,self);
  if((actor->world.pos.x==-730.0f && actor->world.pos.z==-1100.0f)||(actor->world.pos.x==880.0f && actor->world.pos.z==-1170.0f)){FUN_00385c34(zar,play,self);return;}
 } else if(play->sceneNum==0x36) {
  if(gSaveContext.nightFlag==0){Actor_Kill(actor);return;}
  EnHorseNormal_InitSkin(zar,play,self);FUN_00385c34(zar,play,self);return;
 } else if(play->sceneNum==0x5D && actor->world.pos.x==3707.0f && actor->world.pos.y==1413.0f && actor->world.pos.z==-665.0f) {
  EnHorseNormal_InitSkin(zar,play,self);EnHorseNormal_SetupIdle(self,2);return;
 } else EnHorseNormal_InitSkin(zar,play,self);
 if((actor->params&0xF0)==0x10 && (actor->params&0xF)!=0xF) {
  actor->flags|=0x10;self->action=4;self->animationIdx=6;self->waypoint=0;actor->speedXZ=7.0f;
  f32 last=Animation_GetLastFrame(&self->skin.skelAnime,EnHorseNormal_sAnimations[self->animationIdx]);
  Animation_Change_00375c08(&self->skin.skelAnime,EnHorseNormal_sAnimations[self->animationIdx],FUN_00350c68(self),0.0f,last,2,0.0f);
 } else EnHorseNormal_SetupIdle(self,1);
}


#endif
GLOBAL_ASM("asm/EnHorseNormal_Init.s")



// Unmatched Draw draft: 27 words, 1132/1132 B; flagged sounds use cloneSoundPos.
#if 0
typedef void (*EnHorseNormalPostDraw)(Actor*,PlayState*,EnHorseNormalSkin*);
void FUN_001c307c(Actor*,PlayState*,EnHorseNormalSkin*);
void FUN_003583d4(Actor*,PlayState*,EnHorseNormalSkin*,EnHorseNormalPostDraw,s32);
void FUN_001c3180(Actor*,PlayState*,EnHorseNormalSkin*,EnHorseNormalPostDraw,PostLimbDrawOpa,s32,s32,s32);
void SkelAnime_CopyFrameTable(SkelAnime*,void*,void*);
void FUN_00358188(MTX34*,s16,s16,s16,f32,f32,f32,f32,f32,f32);
f32 BgCheck_EntityRaycastFloor3(CollisionContext*,CollisionPoly**,s32*,Vec3f*);

static inline void EnHorseNormal_CloneSound(PlayState* play,EnHorseNormal* self,Vec3f* clonePos) {
 f32 frame=*EnHorse_CurFramePtr(&self->skin.skelAnime);
 self->clonePos=*clonePos;
 Vec3f* sound=&self->cloneSoundPos;
 *sound=self->clonePos;self->cloneSoundPos.y+=120.0f;
 if(self->animationIdx==0 && frame>28.0f && !(self->flags&8)) {self->flags|=8;Audio_PlaySoundGeneral(0x01000183,clonePos,4,&FLOAT_0054ac20,&FLOAT_0054ac20,&DAT_0054ac24.unk_0);}
 else if(self->animationIdx==3 && frame>25.0f && !(self->flags&0x10)) {self->flags|=0x10;Audio_PlaySoundGeneral(0x01000182,&self->clonePos,4,&FLOAT_0054ac20,&FLOAT_0054ac20,&DAT_0054ac24.unk_0);}
 else if(self->animationIdx==3 && (self->flags&0x20)) {self->flags&=~0x20;Audio_PlaySoundGeneral(0x0100015C,sound,4,&FLOAT_0054ac20,&FLOAT_0054ac20,&DAT_0054ac24.unk_0);}
 else if(self->animationIdx==1 && (self->flags&0x20)) {self->flags&=~0x20;Audio_PlaySoundGeneral(0x0100016D,sound,4,&FLOAT_0054ac20,&FLOAT_0054ac20,&DAT_0054ac24.unk_0);}
}

void EnHorseNormal_Draw(Actor* actor,PlayState* play) {
 EnHorseNormal* self=(EnHorseNormal*)actor;
 if(play->sceneNum!=0x63 || play->sceneNum!=0x36) {
 CollisionPoly* poly; Vec3f checkPos;s32 bgId;
 checkPos.x=Math_SinS(actor->shape.rot.y)*30.0f+actor->world.pos.x;
 checkPos.y=actor->world.pos.y+60.0f;
 checkPos.z=Math_CosS(actor->shape.rot.y)*30.0f+actor->world.pos.z;
 self->ground=BgCheck_EntityRaycastFloor3(&play->colCtx,&poly,&bgId,&checkPos);
 actor->shape.rot.x=(s32)(Math_FAtan2F(actor->world.pos.y-self->ground,30.0f)*10430.378f);
 }
 FUN_003583d4(actor,play,&self->skin,FUN_001c307c,1);
 if(self->action==3) {
  s16 rotY=0;Vector3f clonePos(0.0f,0.0f,0.0f);MTX34 saved;
  f32 posY=actor->world.pos.y;
  if(play->sceneNum==0x36) {
   if(actor->world.pos.x==355.0f && posY==0.0f && actor->world.pos.z==-245.0f) {clonePos.x=235.0f;clonePos.z=100.0f;rotY=0x7FFF;}
   else if(actor->world.pos.x==238.0f && posY==0.0f && actor->world.pos.z==-245.0f) {clonePos.x=478.0f;clonePos.z=100.0f;rotY=0x7FFF;}
  } else if(play->sceneNum==0x63) {
   if(actor->world.pos.x==-730.0f && actor->world.pos.z==-1100.0f) {clonePos.x=780.0f;clonePos.z=-80.0f;}
   else if(actor->world.pos.x==880.0f && actor->world.pos.z==-1170.0f) {clonePos.x=-1000.0f;clonePos.z=-70.0f;}
  }
  EnHorseNormal_CloneSound(play,self,(Vec3f*)&clonePos);
  SkelAnime_CopyFrameTable(&self->cloneSkin.skelAnime,self->cloneJointTable,self->jointTable);
  self->cloneRotY=rotY;
  FUN_00358188(&self->cloneMtx,actor->shape.rot.x,rotY,actor->shape.rot.z,actor->scale.x,actor->scale.y,actor->scale.z,clonePos.x,clonePos.y+actor->shape.yOffset*actor->scale.y,clonePos.z);
  MTX34CopyAsm(&saved,&actor->modelMtx);MTX34CopyAsm(&actor->modelMtx,&self->cloneMtx);
  FUN_001c3180(actor,play,&self->cloneSkin,NULL,NULL,1,0,3);
  MTX34CopyAsm(&actor->modelMtx,&saved);
  self->cloneCollider.dim.pos=*(Vec3f*)&clonePos;
  CollisionCheck_SetOC(play,&play->colChkCtx,&self->cloneCollider.base);
 }
}


#endif
GLOBAL_ASM("asm/EnHorseNormal_Draw.s")
void FUN_00385c34(void* zar,PlayState* play,EnHorseNormal* self) {
 self->action=3; self->animationIdx=0;
 self->actor.speedXZ=0.0f; self->accel=0.0f;
 self->yawDelta=0; self->yawVel=0; self->actor.flags|=0x30;
 FUN_00358ea8(zar,play,&self->cloneSkin.skelAnime,ZAR_GetCMBByIndex(zar,0),self->actor.unk_178,6,self->cloneJointTable,NULL,0x18);
 f32 last=Animation_GetLastFrame(&self->skin.skelAnime,EnHorseNormal_sAnimations[self->animationIdx]);
 Animation_Change_00375c08(&self->skin.skelAnime,EnHorseNormal_sAnimations[self->animationIdx],FUN_00350c68(self),0.0f,last,2,0.0f);
 ZARInfo* shadowZar=&play->GetObjectStatus(0)->zarInfo;
 ModelAllocator* allocator=GetModelManager().allocator;
 self->shadowModel=allocator->Create(ZAR_GetCMBByIndex(shadowZar,EnHorseNormal_sModelIndices[1]),0);
 ActorShape_Init(&self->actor.shape,0.0f,(ActorShadowFunc)FUN_00299ef8,20.0f);
}

void EnHorseNormal_FollowPath(EnHorseNormal* self,PlayState* play) {
 Path* path=&play->setupPathList[self->actor.params&0xF];
 Vec3s* pointPos=path->points+self->waypoint;
 f32 dx=pointPos->x-self->actor.world.pos.x;
 f32 dz=pointPos->z-self->actor.world.pos.z;
 f32 distSq=dx*dx+dz*dz;
 Math_SmoothStepToS(&self->actor.world.rot.y,(s16)(Math_FAtan2F(dx,dz)*10430.378f),0xA,0x7D0,1);
 self->actor.shape.rot.y=self->actor.world.rot.y;
 if(distSq<600.0f) {
  self->waypoint++;
  if(self->waypoint>=path->count) self->waypoint=0;
 }
 f32 playSpeed=FUN_00350c68(self);
 SkelAnime_SetPlaySpeed(&self->skin.skelAnime,playSpeed);
 if(FUN_003731e0(&self->skin.skelAnime)) {
  f32 last=Animation_GetLastFrame(&self->skin.skelAnime,EnHorseNormal_sAnimations[self->animationIdx]);
  Animation_Change_00375c08(&self->skin.skelAnime,EnHorseNormal_sAnimations[self->animationIdx],FUN_00350c68(self),0.0f,last,2,0.0f);
  if(self->animationIdx==5) Audio_PlaySoundGeneral(0x0100015B,&self->actor.world.pos,4,&FLOAT_0054ac20,&FLOAT_0054ac20,&DAT_0054ac24.unk_0);
  else if(self->animationIdx==6) Audio_PlaySoundGeneral(0x0100015B,&self->actor.world.pos,4,&FLOAT_0054ac20,&FLOAT_0054ac20,&DAT_0054ac24.unk_0);
 }
}



static inline void EnHorseNormal_StepSound(EnHorseNormal* self) {
 f32 frame=*EnHorse_CurFramePtr(&self->skin.skelAnime);
 if(frame>EnHorseNormal_sStepFrames[self->stepIndex] && !(self->stepIndex==0 && frame>EnHorseNormal_sStepFrames[1])) {
  Audio_PlaySoundGeneral(0x0100015A,&self->actor.world.pos,4,&FLOAT_0054ac20,&FLOAT_0054ac20,&DAT_0054ac24.unk_0);self->stepIndex++;if(self->stepIndex>=2) self->stepIndex=0;
 }
}
static inline void EnHorseNormal_RunSound(EnHorseNormal* self) {
 if(self->animationIdx==5 || self->animationIdx==6) Audio_PlaySoundGeneral(0x0100015B,&self->actor.world.pos,4,&FLOAT_0054ac20,&FLOAT_0054ac20,&DAT_0054ac24.unk_0);
}
void EnHorseNormal_Wander(EnHorseNormal* self,PlayState* play) {
 u8 next=self->animationIdx;
 switch(EnHorseNormal_sAnimClass[self->animationIdx]) {
 case 0: EnHorseNormal_IdleSound(self);self->actor.speedXZ=0.0f;self->accel=0.0f;break;
 case 1:
  if(Rand_ZeroOne()<0.1f) self->accel=2.0f*Rand_ZeroOne()-1.0f;
  self->actor.speedXZ+=self->accel;
  if(self->actor.speedXZ<=0.0f) {self->actor.speedXZ=0.0f;self->accel=0.0f;next=0;}
  else if(self->actor.speedXZ<3.0f) {EnHorseNormal_StepSound(self);next=4;}
  else if(self->actor.speedXZ<6.0f) next=5;
  else if(self->actor.speedXZ<8.0f) next=6;
  else {self->actor.speedXZ=8.0f;next=6;}
  if(Rand_ZeroOne()<0.1f || (self->yawVel==0 && ((self->actor.bgCheckFlags&8)||(self->bodyCollider.base.ocFlags1&2)||(self->headCollider.base.ocFlags1&2)))) {
   self->yawVel+=Rand_ZeroOne()*30.0f-15.0f;
   if(self->yawVel>50)self->yawVel=50;else if(self->yawVel< -50)self->yawVel=-50;
  }
  self->yawDelta+=self->yawVel;
  if(self->yawDelta< -300)self->yawDelta=-300;else if(self->yawDelta>300)self->yawDelta=300;
  else if(Rand_ZeroOne()<0.25f && fabsf((f32)self->yawDelta)<100.0f) {self->yawDelta=0;self->yawVel=0;}
  self->actor.world.rot.y+=self->yawDelta;self->actor.shape.rot.y=self->actor.world.rot.y;
  break;
 case 2:case 3:break;
 }
 if(next!=self->animationIdx || FUN_003731e0(&self->skin.skelAnime)) {
  if(next!=self->animationIdx) {
   self->animationIdx=next;self->flags&=~1;self->flags&=~2;
   if(next==1) Audio_PlaySoundGeneral(0x0100016D,&self->soundPos,4,&FLOAT_0054ac20,&FLOAT_0054ac20,&DAT_0054ac24.unk_0);
   else if(next==3) Audio_PlaySoundGeneral(0x0100015C,&self->soundPos,4,&FLOAT_0054ac20,&FLOAT_0054ac20,&DAT_0054ac24.unk_0);
   else EnHorseNormal_RunSound(self);
   {f32 last=Animation_GetLastFrame(&self->skin.skelAnime,EnHorseNormal_sAnimations[self->animationIdx]); Animation_Change_00375c08(&self->skin.skelAnime,EnHorseNormal_sAnimations[self->animationIdx],FUN_00350c68(self),0.0f,last,2,-3.0f);}
  } else {
   switch(EnHorseNormal_sAnimClass[self->animationIdx]) {
   case 0:
    if(Rand_ZeroOne()<0.25f) {next=4;self->accel=1.0f;}
    else {next=EnHorseNormal_sRandomIdle[(s32)(Rand_ZeroOne()*2.0f)];self->actor.speedXZ=0.0f;self->accel=0.0f;}
    break;
   case 1:case 2:case 3:break;
   }
   self->flags&=~1;self->flags&=~2;
   if(next==1) Audio_PlaySoundGeneral(0x0100016D,&self->soundPos,4,&FLOAT_0054ac20,&FLOAT_0054ac20,&DAT_0054ac24.unk_0);
   else if(next==3) Audio_PlaySoundGeneral(0x0100015C,&self->soundPos,4,&FLOAT_0054ac20,&FLOAT_0054ac20,&DAT_0054ac24.unk_0);
   else EnHorseNormal_RunSound(self);
   if(next!=self->animationIdx) {self->animationIdx=next;{f32 last=Animation_GetLastFrame(&self->skin.skelAnime,EnHorseNormal_sAnimations[self->animationIdx]); Animation_Change_00375c08(&self->skin.skelAnime,EnHorseNormal_sAnimations[self->animationIdx],FUN_00350c68(self),0.0f,last,2,-3.0f);}}
   else {f32 last=Animation_GetLastFrame(&self->skin.skelAnime,EnHorseNormal_sAnimations[self->animationIdx]); Animation_Change_00375c08(&self->skin.skelAnime,EnHorseNormal_sAnimations[self->animationIdx],FUN_00350c68(self),0.0f,last,2,0.0f);}
  }
 }
}



#if 0 // Sol small pending: semantically corrected yaw and angle guards, 360/364 B.
void FUN_003406b8(Actor* actor,PlayState* play,EnHorseNormalSkin* skin,void* postDraw,PostLimbDrawOpa postLimb,s32 arg5,s32 arg6,s32 flags) {
 if(!(flags&1)) {
  Vec3f pos;
  MTX34 modelMtx;
  pos.x=actor->world.pos.x;
  pos.y=actor->world.pos.y+actor->shape.yOffset*actor->scale.y;
  pos.z=actor->world.pos.z;
  FUN_003a5778(&modelMtx,&actor->scale,&actor->shape.rot,&pos);
  f32 yaw=skin->skinYaw;
  if(yaw!=0.0f) {
   f32 angle=yaw*9.58738e-5f;
   if(angle!=0.0f) {
   f32 s=sinf(angle),c=cosf(angle);
   for(s32 i=0;i<3;i++) {
    f32 x=modelMtx.data[i][0],z=modelMtx.data[i][2];
    modelMtx.data[i][0]=x*c-z*s;
    modelMtx.data[i][2]=x*s+z*c;
   }
   }
  }
  MTX34CopyAsm(&actor->modelMtx,&modelMtx);
 }
 SkelAnime_DrawOpa(&skin->skelAnime,&actor->modelMtx,NULL,postLimb,actor,0);
 if(postDraw!=NULL) ((void (*)(Actor*,PlayState*,EnHorseNormalSkin*))postDraw)(actor,play,skin);
}
#endif
GLOBAL_ASM("asm/FUN_003406b8.s")

// Correct matrix order and accepted SDK helpers; remaining compiler-lifetime differences documented in STATUS.
#if 0 // Sol small pending: SDK MakeTrans/MakeScale + axis setters, 396/404 B.
void FUN_003625f8(MTX34* dst, Vec3f* axis, f32 angle);
void FUN_0036c174(MTX34* dst, MTX34* a, MTX34* b);

static inline void EnHorseNormal_SetAxis(Vec3f* p,f32 x,f32 y,f32 z) {p->x=x;p->y=y;p->z=z;}

void FUN_003a5778(MTX34* m, Vec3f* scale, Vec3s* rot, Vec3f* trans) {
    Vec3f axis;
    MTX34 rx;
    MTX34 ry;
    MTX34 rz;
    MTX34 scaleMtx;
    MTX34 tmp;

    MTX34_MakeTrans(m, trans);
    EnHorseNormal_SetAxis(&axis,1.0f,0.0f,0.0f);
    FUN_003625f8(&rx, &axis, rot->x * 9.58738E-5f);
    EnHorseNormal_SetAxis(&axis,0.0f,1.0f,0.0f);
    FUN_003625f8(&ry, &axis, rot->y * 9.58738E-5f);
    EnHorseNormal_SetAxis(&axis,0.0f,0.0f,1.0f);
    FUN_003625f8(&rz, &axis, rot->z * 9.58738E-5f);
    FUN_0036c174(&tmp, &ry, &rx);
    FUN_0036c174(&tmp, &tmp, &rz);
    FUN_0036c174(&tmp, m, &tmp);
    MTX34_MakeScale(&scaleMtx, scale);
    FUN_0036c174(m, &tmp, &scaleMtx);
}
#endif
GLOBAL_ASM("asm/FUN_003a5778.s")




}
