/* Complete first candidates, unmatched and inactive. No generated statics enabled. */
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
void FUN_00181770(BossDodongo*,PlayState*);
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

void FUN_001e81fc(BossDodongo* self, PlayState* play) {
    Actor* explosive;
    f32 dx,dy,dz;
    self->unk_7B2=1;
    if(self->unk_77C>20) Audio_PlayActorSound2(&self->actor,0x010002BF);
    Math_SmoothStepToF(&self->unk_7D8,0.05f,1.0f,0.005f,0.0f);
    FUN_00370734(&self->skelAnime);
    if(self->unk_7AA==0) {
        self->actor.speedXZ=self->unk_7B4=0.0f;
        FUN_00353020(&self->skelAnime,&BossDodongo_Anim5,1.0f,0.0f,(f32)Animation_GetLastFrame(&self->skelAnime,BossDodongo_AnimTable[5]),2,0.0f);
        self->actionFunc=FUN_003dc268;
        self->unk_7AA=75;
        self->unk_77E=0;
    } else {
        self->unk_77C++;
        if(self->unk_77C>20 && self->unk_77C<82) {
            explosive=play->actorCtx.actorLists[ACTORCAT_EXPLOSIVES].head;
            while(explosive!=0) {
                if(explosive!=&self->actor) {
                    dx=explosive->world.pos.x-self->unk_9EC;
                    dy=explosive->world.pos.y-self->unk_9F0;
                    dz=explosive->world.pos.z-self->unk_9F4;
                    if(fabsf(dx)<40.0f && fabsf(dy)<40.0f && fabsf(dz)<40.0f && (explosive->id!=0x10 || *((u8*)explosive+0x284)==0)) {
                        Actor_Kill(explosive);
                        Audio_PlayActorSound2(&self->actor,0x010002C0);
                        FUN_00353020(&self->skelAnime,&BossDodongo_Anim7,1.0f,0.0f,(f32)Animation_GetLastFrame(&self->skelAnime,BossDodongo_AnimTable[7]),2,-5.0f);
                        self->actionFunc=FUN_0013ff98;
                        self->unk_7CC=50.0f;
                        self->unk_7D0=300.0f;
                        self->unk_780=10;
                        self->unk_790=2;
                        self->unk_7AA=53;
                        return;
                    }
                }
                explosive=explosive->next;
            }
        }
    }
}

void FUN_0013ff98(BossDodongo* self, PlayState* play) {
    Vec3f dustVel,dustAccel,dustPos,focus;
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
            FUN_00365d20(play,&dustPos,&dustVel,&dustAccel,BossDodongo_DustPrim,BossDodongo_DustEnv,500,10,10);
        }
        end=(f32)Animation_GetLastFrame(&self->skelAnime,BossDodongo_AnimTable[11]);
        FUN_00353020(&self->skelAnime,&BossDodongo_Anim11,1.0f,0.0f,end,2,-5.0f);
        self->actionFunc=FUN_0013cb28;
        Actor_SetColorFilter(&self->actor,0,255,0,(s16)(end+100.0f));
        Audio_PlayActorSound2(&self->actor,0x01000100);
        Audio_PlayActorSound2(&self->actor,0x01000276);
        FUN_0036fca8(&self->actor,play,4,10);
        self->unk_764-=2;
        if(self->unk_764<=0) self->unk_764=1;
        focus=self->actor.focus.pos;
        FUN_00365560(play,0,0,&focus,BossDodongo_ExitParams);
    }
}

void FUN_00345590(BossDodongo* self, PlayState* play) {
    Vec3f* corner;
    f32 dx,dz;
    if(self->unk_77A==0) {
        if(FUN_0036e5e0(&self->skelAnime,14.0f,1.0f)) {
            FUN_003490e0(&self->skelAnime,&BossDodongo_Anim3);
            self->unk_77A=1;
        }
    } else if(self->unk_78C!=2) {
        if((s32)self->skelAnime.curFrame==1 || (s32)self->skelAnime.curFrame==31) {
            if((s32)self->skelAnime.curFrame==1) FUN_0036f00c(play,&self->actor,(Vec3f*)&self->unk_9E0,25.0f,10,8.0f,500,10,0);
            else FUN_0036f00c(play,&self->actor,(Vec3f*)&self->unk_9D4,25.0f,10,8.0f,500,10,0);
            if(self->unk_78C!=0) Audio_PlaySoundGeneral(0x01000278,0,4,&FLOAT_0054ac20,&FLOAT_0054ac20,&DAT_0054ac24.unk_0);
            else Audio_PlayActorSound2(&self->actor,0x01000278);
            if(self->unk_784==0) FUN_0036fca8(&self->actor,play,4,10);
            else self->unk_786=10;
        }
    }
    FUN_00370734(&self->skelAnime);
    corner=&BossDodongo_Corners[self->unk_770];
    self->unk_7BC=0.7f;
    Math_SmoothStepToF(&self->unk_7B4,2.8f,1.0f,0.175f,0.0f);
    Math_SmoothStepToF(&self->actor.world.pos.x,corner->x,0.3f,self->unk_7B4,0.0f);
    Math_SmoothStepToF(&self->actor.world.pos.z,corner->z,0.3f,self->unk_7B4,0.0f);
    dx=corner->x-self->actor.world.pos.x; dz=corner->z-self->actor.world.pos.z;
    Math_SmoothStepToF(&self->unk_7B8,1333.3333740234375f,1.0f,self->unk_7BC*80.0f,0.0f);
    Math_SmoothStepToS(&self->actor.world.rot.y,(s16)(Math_FAtan2F(dx,dz)*10430.3779296875f),5,(s16)(self->unk_7BC*self->unk_7B8),5);
    Math_SmoothStepToS(&self->unk_794,0,2,2000,0);
    if(fabsf(dx)<=5.0f && fabsf(dz)<=5.0f) {
        self->unk_7B8=self->unk_7B4=0.0f;
        if(self->unk_772==0) { self->unk_770++; if(self->unk_770>=4) self->unk_770=0; }
        else { self->unk_770--; if(self->unk_770<0) self->unk_770=3; }
    }
    if(self->unk_7AA==0 && self->unk_78C==0) {
        if(self->actor.xzDistToPlayer<500.0f && self->unk_774!=0 && self->unk_78A==0) {
            self->actor.speedXZ=0.0f;
            FUN_00353020(&self->skelAnime,&BossDodongo_Anim4,1.0f,0.0f,(f32)Animation_GetLastFrame(&self->skelAnime,BossDodongo_AnimTable[4]),2,-5.0f);
            self->actionFunc=FUN_001e81fc; self->unk_7AA=150; self->unk_77C=0; self->unk_7B2=1;
            Actor_SpawnAsChild(&play->actorCtx,&self->actor,play,0x30,self->unk_9BC,self->unk_9C0-20.0f,self->unk_9C4,0,self->actor.shape.rot.y,0,-1);
        }
        if(self->unk_78A==0 && self->unk_788==0) {
            FUN_00353020(&self->skelAnime,&BossDodongo_Anim6,1.0f,0.0f,59.0f,2,-5.0f);
            self->actionFunc=FUN_001ccc78; self->unk_778=0; self->unk_7AA=41;
        }
    }
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
    if(self->unk_7AA==0) {
        Math_SmoothStepToF(&self->unk_7B4,15.0f,1.0f,0.75f,0.0f);
        Math_SmoothStepToF(&self->actor.world.pos.x,corner->x,1.0f,self->unk_7B4,0.0f);
        Math_SmoothStepToF(&self->actor.world.pos.z,corner->z,1.0f,self->unk_7B4,0.0f);
        self->unk_794+=2000;
        if(self->actor.bgCheckFlags&1) {
            self->unk_7F8=7700.0f;
            Audio_PlayActorSound2(&self->actor,0x010002BE);
            if(!(self->unk_76E&7)) FUN_0036627c((u8*)play+0x364,2,1,8);
            if(!(self->unk_76E&1)) FUN_0036f00c(play,&self->actor,&self->actor.world.pos,100.0f,3,8.0f,500,10,0);
        }
    }
    dx=corner->x-self->actor.world.pos.x; dz=corner->z-self->actor.world.pos.z;
    Math_SmoothStepToF(&self->unk_7B8,1333.3333740234375f,1.0f,self->unk_7BC*40.0f,0.0f);
    Math_SmoothStepToS(&self->actor.world.rot.y,(s16)(Math_FAtan2F(dx,dz)*10430.3779296875f),5,(s16)(self->unk_7B8*self->unk_7BC),0);
    if(fabsf(dx)<=15.0f && fabsf(dz)<=15.0f) {
        self->unk_778++;
        if(self->unk_778>=2) {
            if(self->unk_776) self->unk_772=1-self->unk_772;
            self->unk_7B8=self->unk_7B4=0.0f;
            self->unk_794=-21845;
            FUN_00181770(self,play);
            self->unk_7F8=9200.0f;
            self->actor.velocity.y=20.0f;
            Audio_PlayActorSound2(&self->actor,0x010002BC);
            FUN_0036627c((u8*)play+0x364,2,6,8);
            dust=self->actor.world.pos; dust.y+=60.0f;
            FUN_0037378c(play,&dust,250.0f,40,800,10,0);
            FUN_0036fca8(&self->actor,play,6,15);
        } else { self->actor.velocity.y=15.0f; Audio_PlayActorSound2(&self->actor,0x010002BD); }
        if(self->unk_772==0) { self->unk_770++; if(self->unk_770>=4) self->unk_770=0; }
        else { self->unk_770--; if(self->unk_770<0) self->unk_770=3; }
    }
}

void BossDodongo_Init(Actor* actor, GameState* state) {
    BossDodongo* self=(BossDodongo*)actor;
    PlayState* play=(PlayState*)state;
    ZARInfo* zar=FUN_00372f38(actor,play,0);
    s32 indices[80];
    s32 i;
    void* cmab;
    ModelManager& manager=GetModelManager();
    self->texture=ZAR_GetCTXBByIndex(zar,*(s32*)((u8*)&manager+0x41FC));
    *(BossDodongoEffect**)((u8*)play+0x5C28)=self->effects;
    Actor_ProcessInitChain(actor,BossDodongo_InitChain);
    ActorShape_Init(&actor->shape,250.0f,ActorShadow_DrawCircle,9200.0f);
    Actor_SetScale(actor,0.01f);
    FUN_00353c9c(actor,play,&self->skelAnime,2,BossDodongo_AnimTable[1],0,0,0);
    self->skelAnime.unk_75[0]=0;
    for(i=0;i<80;i++) indices[i]=3;
    FUN_00352ee0(actor,play,80,(SkeletonAnimationModel**)self->modelPointers,indices);
    cmab=zar->GetCMABByIndex(1);
    for(i=0;i<80;i++) {
        self->effects[i].model=(SkeletonAnimationModel*)self->modelPointers[i];
        ((SkeletonAnimationModel*)self->modelPointers[i])->unk_0C->FUN_00372d94(cmab);
        ((SkeletonAnimationModel*)self->modelPointers[i])->unk_0C->SetAnimType(1);
        ((SkeletonAnimationModel*)self->modelPointers[i])->unk_0C->SetAnimSpeed(0.0f);
    }
    FUN_003490e0(&self->skelAnime,&BossDodongo_Anim1);
    self->unk_7C8=1.0f;
    FUN_00353020(&self->skelAnime,&BossDodongo_Anim1,1.0f,0.0f,(f32)Animation_GetLastFrame(&self->skelAnime,BossDodongo_AnimTable[1]),0,-10.0f);
    self->actionFunc=FUN_0019e118;
    self->unk_76C=0; self->unk_78C=1; self->unk_764=12;
    self->unk_7EC=995.0f; self->unk_7F0=1000.0f; self->unk_7F4=2.0f;
    actor->colChkInfo.mass=255;
    self->unk_7F8=250.0f;
    Collider_InitJntSph(play,self->collider);
    Collider_SetJntSph(play,self->collider,actor,BossDodongo_ColliderInit,self->colliderElements);
    if(FUN_0035b164()!=1 && Flags_GetClear(play,*((s8*)play+0x4C30))) {
        FUN_003309e0(actor,play,1);
        Actor_Kill(actor);
        Actor_SpawnAsChild(&play->actorCtx,actor,play,0x5D,-890.0f,-1520.0f,-3304.0f,0,0,0,0);
        Actor_Spawn(&play->actorCtx,play,0x59,-890.0f,-1520.0f,-3304.0f,0,0,0x6000,1,0);
        Actor_Spawn(&play->actorCtx,play,0x5F,-690.0f,-1520.0f,-3304.0f,0,0,0,0,1);
    }
    actor->unk_19B=4;
    *((u8*)self+0x2068)=0;
    actor->flags&=~1;
}

void FUN_00389970(SkelAnime* skel, s32 limbIndex, MTX34* mtx, Actor* actor) {
    BossDodongo* self=(BossDodongo*)actor;
    static Vector3f focusOffset(5000.0f,-2500.0f,0.0f);
    static Vector3f vecOffset(0.0f,0.0f,0.0f);
    static Vector3f fireOffset(6000.0f,0.0f,0.0f);
    static Vector3f mouthOffset(5000.0f,-2000.0f,0.0f);
    static Vector3f rightFootOffset(8000.0f,0.0f,0.0f);
    static Vector3f leftFootOffset(8000.0f,0.0f,0.0f);
    if(limbIndex==20) {
        VEC3TransformAsm((Vector3f*)&self->unk_9BC,mtx,&vecOffset);
        VEC3TransformAsm((Vector3f*)&actor->focus.pos,mtx,&focusOffset);
        VEC3TransformAsm((Vector3f*)&self->unk_9C8,mtx,&fireOffset);
        VEC3TransformAsm((Vector3f*)&self->unk_9EC,mtx,&mouthOffset);
    } else if(limbIndex==6) VEC3TransformAsm((Vector3f*)&self->unk_9E0,mtx,&rightFootOffset);
    else if(limbIndex==3) VEC3TransformAsm((Vector3f*)&self->unk_9D4,mtx,&leftFootOffset);
    Collider_UpdateSpheres(limbIndex,self->collider,mtx);
}

void BossDodongo_Draw(Actor* actor, GameState* state) {
    BossDodongo* self=(BossDodongo*)actor;
    PlayState* play=(PlayState*)state;
    f32 angle,sine,cosine,a,b;
    s16 i;
    BossDodongoEffect* eff;
    MTX34 matrix;
    Vec3f pos;
    f32 color[4];
    angle=self->unk_80C;
    if(angle!=0.0f) {
        sine=sinf(angle); cosine=cosf(angle);
        for(i=0;i<3;i++) {
            a=actor->modelMtx.data[i][0]; b=actor->modelMtx.data[i][1];
            actor->modelMtx.data[i][0]=a*cosine+b*sine;
            actor->modelMtx.data[i][1]=b*cosine-a*sine;
        }
    }
    angle=((f32)self->unk_794*(1.0f/32768.0f))*3.141590118408203f;
    if(angle!=0.0f) {
        sine=sinf(angle); cosine=cosf(angle);
        for(i=0;i<3;i++) {
            a=actor->modelMtx.data[i][1]; b=actor->modelMtx.data[i][2];
            actor->modelMtx.data[i][1]=a*cosine+b*sine;
            actor->modelMtx.data[i][2]=b*cosine-a*sine;
        }
    }
    SkelAnime_DrawOpa(&self->skelAnime,&actor->modelMtx,(OverrideLimbDrawOpa)FUN_003815f0,(PostLimbDrawOpa)FUN_00389970,actor,0);
    eff=*(BossDodongoEffect**)((u8*)play+0x5C28);
    for(i=0;i<80;i++,eff++) {
        if(eff->active==1) {
            color[0]=eff->color[0]*(1.0f/255.0f); color[1]=eff->color[1]*(1.0f/255.0f);
            color[2]=eff->color[2]*(1.0f/255.0f); color[3]=eff->alpha*(1.0f/255.0f);
            FUN_00358778(eff->model,0,0,color,0);
            pos=eff->pos; MTX34_MakeTrans(&matrix,&pos);
            FUN_00371fac(&matrix,(MTX34*)((u8*)play+0x2FC));
            matrix.data[0][0]*=eff->scale; matrix.data[1][0]*=eff->scale; matrix.data[2][0]*=eff->scale;
            matrix.data[0][1]*=eff->scale; matrix.data[1][1]*=eff->scale; matrix.data[2][1]*=eff->scale;
            eff->model->SetUnkAC(1);
            eff->model->FUN_003721e0(&matrix);
            eff->model->FUN_00372170(0);
        }
    }
}
/* 3DS hit element extends the N64 record to 0x50 bytes. */
struct BossDodongoHitElement {
    char pad0[0xE]; Vec3s hitPos; char pad14[2]; u8 flags; char pad17[0xD];
    u32* damageInfo; char pad28[0x28];
};
typedef char BossHitSize[(sizeof(BossDodongoHitElement)==0x50)?1:-1];
extern s32 BossDodongo_Anim12, BossDodongo_DebugKill;
void FUN_00121ba0(BossDodongo*,PlayState*);
void FUN_003d944c(BossDodongo*,PlayState*);
void Audio_SetBGM(s32,s32);
void FUN_00375b70(PlayState*,Actor*);
void FUN_003741e4(PlayState*,u32,s32,Vec3f*,s32);
void FUN_003661a8(PlayState*,Vec3f*,s32);
s32 FUN_003656fc(PlayState*,u32);
void FUN_0032d674(PlayState*);
void FUN_0029e828(BossDodongo* self, PlayState* play) {
    BossDodongoHitElement* elements;
    u32* damageInfo;
    Vec3f hitPos;
    s16 i;
    s32 damage;
    if(self->unk_75C) self->unk_75C--;
    if(BossDodongo_DebugKill) { BossDodongo_DebugKill=0; self->unk_764=0; }
    if(self->unk_764<=0) {
        if(self->actionFunc!=FUN_003da7ac) {
            self->actor.speedXZ=self->unk_7B4=0.0f;
            FUN_00353020(&self->skelAnime,&BossDodongo_Anim12,1.0f,0.0f,(f32)Animation_GetLastFrame(&self->skelAnime,BossDodongo_AnimTable[12]),2,-5.0f);
            self->actionFunc=FUN_003da7ac;
            Audio_PlayActorSound2(&self->actor,0x01000277);
            self->unk_7AA=0; self->unk_76C=0;
            self->actor.flags&=~5;
            self->unk_78C=1;
            Audio_SetBGM(0,1);
            FUN_00375b70(play,&self->actor);
        }
        return;
    }
    if(self->unk_790!=0 && self->unk_75C==0) {
        elements=*(BossDodongoHitElement**)((u8*)self+0xA2C);
        for(i=0;i<19;i++) {
            if(elements[i].flags&2) {
                Audio_PlayActorSound2(&self->actor,0x0100010C);
                self->unk_75C=5;
                damageInfo=elements[i].damageInfo;
                Math_Vec3s_ToVec3f(&hitPos,&elements[i].hitPos);
                if(i==0) FUN_003741e4(play,*damageInfo,1,&hitPos,0);
                else {
                    FUN_003741e4(play,*damageInfo,2,&hitPos,0);
                    if(!(*damageInfo&5)) FUN_003661a8(play,&hitPos,4);
                }
                if(FUN_003656fc(play,*damageInfo)) FUN_0032d674(play);
                break;
            }
        }
    }
    if(self->actionFunc==FUN_001e81fc) {
        elements=*(BossDodongoHitElement**)((u8*)self+0xA2C);
        for(i=0;i<19;i++) {
            if((elements[i].flags&2) && ((damageInfo=elements[i].damageInfo),(*damageInfo&0x14))) {
                elements[i].flags&=~2;
                self->unk_790=2;
                Actor_SetColorFilter(&self->actor,0x400000,255,0,12);
                FUN_00353020(&self->skelAnime,&BossDodongo_AnimTable[2],1.0f,0.0f,(f32)Animation_GetLastFrame(&self->skelAnime,BossDodongo_AnimTable[2]),2,-10.0f);
                self->unk_77A=0; self->actionFunc=FUN_00345590; self->unk_7B4=0.0f;
                self->actor.flags|=1; self->unk_7AA=75;
                elements=*(BossDodongoHitElement**)((u8*)self+0xA2C);
                Math_Vec3s_ToVec3f(&hitPos,&elements[i].hitPos);
                FUN_003741e4(play,*damageInfo,0,&hitPos,0);
                Audio_PlayActorSound2(&self->actor,0x01000466);
                self->unk_75C=10;
                return;
            }
        }
    }
    elements=*(BossDodongoHitElement**)((u8*)self+0xA2C);
    if(elements[0].flags&2) {
        elements[0].flags&=~2;
        damageInfo=elements[0].damageInfo;
        damage=FUN_003656fc(play,*damageInfo);
        if((self->actionFunc==FUN_003d944c || self->actionFunc==FUN_0013cb28) && damage!=0) {
            Audio_PlayActorSound2(&self->actor,0x01000276);
            if(self->actionFunc!=FUN_00121ba0) {
                FUN_00353020(&self->skelAnime,&BossDodongo_AnimTable[8],1.0f,0.0f,(f32)Animation_GetLastFrame(&self->skelAnime,BossDodongo_AnimTable[8]),2,-5.0f);
                self->actionFunc=FUN_00121ba0;
            }
            self->unk_7AA=150;
            self->unk_790=5;
            Actor_SetColorFilter(&self->actor,0x400000,255,0,12);
            self->unk_764-=damage;
            elements=*(BossDodongoHitElement**)((u8*)self+0xA2C);
            Math_Vec3s_ToVec3f(&hitPos,&elements[0].hitPos);
            FUN_003741e4(play,*damageInfo,0,&hitPos,0);
            self->unk_75C=5;
            FUN_0032d674(play);
            return;
        }
        if(self->unk_75C==0) {
            elements=*(BossDodongoHitElement**)((u8*)self+0xA2C);
            Math_Vec3s_ToVec3f(&hitPos,&elements[0].hitPos);
            FUN_003741e4(play,*damageInfo,1,&hitPos,0);
            Audio_PlayActorSound2(&self->actor,0x0100010C);
            self->unk_75C=5;
            if(damage) FUN_0032d674(play);
        }
    } else if(self->unk_75C==0) {
        for(i=1;i<19;i++) {
            if(elements[i].flags&2) {
                Audio_PlayActorSound2(&self->actor,0x0100010C);
                self->unk_75C=5;
                elements=*(BossDodongoHitElement**)((u8*)self+0xA2C);
                damageInfo=elements[i].damageInfo;
                Math_Vec3s_ToVec3f(&hitPos,&elements[i].hitPos);
                FUN_003741e4(play,*damageInfo,2,&hitPos,0);
                if(!(*damageInfo&5)) FUN_003661a8(play,&hitPos,4);
                if(FUN_003656fc(play,*damageInfo)) FUN_0032d674(play);
                break;
            }
        }
    }
}

}

#endif
