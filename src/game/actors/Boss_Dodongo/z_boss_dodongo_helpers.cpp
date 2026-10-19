/* Unmatched native 3DS candidate. Original ASM remains active. */
#if 0
#include "z_boss_dodongo.hpp"
extern "C" {
extern s32 BossDodongo_AnimTable[];
extern s32 BossDodongo_ResetDescriptor[4];
void FUN_00353020(SkelAnime*,s32*,f32,f32,f32,s32,f32);
void FUN_001ccc78(BossDodongo*,PlayState*);
void FUN_0036932c(void*,s32);
void FUN_00358778(void*,s32,s32,void*,s32);
struct BossDodongoMaterialState { char pad0[12]; f32 alpha; char pad10[136]; };
struct BossDodongoRoomMaterial { char pad0[4]; void* model; char pad8[4]; u16 count; char padE[2]; BossDodongoMaterialState* states; };
struct BossDodongoMaterialColor { f32 rgba[4]; };
static inline void BossDodongo_SpawnFire(BossDodongo* self, PlayState* play) {
    PlayState* play2=play;
        Actor_SpawnAsChild(&play2->actorCtx, &self->actor, play2, 0x30,
                           self->unk_9BC, self->unk_9C0 - 20.0f, self->unk_9C4,
                           0, self->actor.shape.rot.y, 0, self->unk_77E);
}
void FUN_003dc268(BossDodongo* self, PlayState* play) {
    f32 one;
    f32 zero = 0.0f;
    FUN_00370734(&self->skelAnime);
    one=1.0f;
    if (FUN_0036e5e0(&self->skelAnime, 12.0f, one)) {
        Audio_PlayActorSound2(&self->actor, 0x01000275);
    }
    if (FUN_0036e5e0(&self->skelAnime, 17.0f, one)) {
        self->unk_798 = 0x1C;
    }
    if (self->skelAnime.curFrame > 17.0f) {
      if (self->skelAnime.curFrame < 35.0f) {
        BossDodongo_SpawnFire(self,play);
        self->unk_77E++;
        Math_SmoothStepToF(&self->unk_814, zero, one, 8.0f, zero);
    }
      }
    if (self->unk_7AA == 0) {
        FUN_00353020(&self->skelAnime, &BossDodongo_AnimTable[6], one, zero, 59.0f, 2, -5.0f);
        self->actionFunc = FUN_001ccc78;
        self->unk_778 = 0;
        self->unk_7AA = 0x29;
    }
}

void FUN_003309e0(Actor* actor, PlayState* play, s32 reset) {
    BossDodongoRoomMaterial* room;
    f32 zero;
    f32 current;
    BossDodongoMaterialColor color;
    s32 count;
    s32 i;
    if(*((u8*)play+0x4C36)==0) return;
    room=(BossDodongoRoomMaterial*)((u8*)play+0x4C38);
    zero=0.0f;
    count=room->count;
    for(i=0;i<count;i++) {
        if(reset!=0) room->states[i].alpha=zero;
        else {
            current=room->states[i].alpha;
            Math_SmoothStepToF(&current,zero,0.1f,0.1f,zero);
            room->states[i].alpha=current;
        }
    }
    if(reset!=0) current=zero;
    color=*(BossDodongoMaterialColor*)BossDodongo_ResetDescriptor;
    color.rgba[3]=1.0f-current;
    if(color.rgba[3]<=zero) color.rgba[3]=zero;
    if(color.rgba[3]==1.0f) FUN_0036932c(room->model,3);
    FUN_00358778(room->model,5,0,&color,2);
    FUN_00358778(room->model,1,0,&color,2);
    FUN_00358778(room->model,3,0,&color,2);
    FUN_00358778(room->model,7,0,&color,2);
    FUN_00358778(room->model,8,0,&color,2);
    FUN_00358778(room->model,11,0,&color,2);
}
}

#endif
