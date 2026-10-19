/* Best native-sized candidate: 23 differing words. ASM remains active. */
#if 0
/* Walking-action reconstruction; reference FUN_00345590, 1144 bytes. */
#include "z_boss_dodongo.hpp"
extern "C" {
extern s32 BossDodongo_AnimTable[];
extern s32 BossDodongo_Anim3, BossDodongo_Anim4, BossDodongo_Anim6;
extern Vec3f BossDodongo_Corners[4];
s32 Animation_GetLastFrame(SkelAnime*,s32);
void FUN_00353020(SkelAnime*,s32*,f32,f32,f32,s32,f32);
void FUN_003490e0(SkelAnime*,s32*);
void FUN_0036f00c(PlayState*,Actor*,Vec3f*,f32,s16,f32,s16,s16,u8);
void FUN_0036fca8(Actor*,PlayState*,s16,s16);
void FUN_001e81fc(BossDodongo*,PlayState*);
void FUN_001ccc78(BossDodongo*,PlayState*);
f32 fabsf(f32);
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
    Math_SmoothStepToS(&self->actor.world.rot.y,(s16)(Math_FAtan2F(dx,dz)*10430.3779296875f),5,(s16)(self->unk_7B8*self->unk_7BC),5);
    Math_SmoothStepToS(&self->unk_794,0,2,2000,0);
    if(fabsf(dx)<=5.0f && fabsf(dz)<=5.0f) {
        self->unk_7B4=self->unk_7B8=0.0f;
        if(self->unk_772==0) { self->unk_770++; if(self->unk_770>3) self->unk_770=0; }
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

}

#endif
