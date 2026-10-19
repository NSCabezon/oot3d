extern "C" {
#include "z_en_mb.hpp"

void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);
void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_0010b5d0(EnMb* self, PlayState* play);
void FUN_003e9230(EnMb* self, PlayState* play);

f32 FUN_0036d260(Vec3f* a, Vec3f* b) {
    return Math_Vec3f_DistXZ(a, b);
}

void FUN_0019f61c(EnMb* self) {
    Animation_PlayLoop(&self->skelAnime, 14);
    self->actor.speedXZ = 0.0f;
    self->timer1 = Rand_S16Offset_003702c8(30, 50);
    self->state = 6;
    self->actionFunc = FUN_0010b5d0;
}

void FUN_00181510(EnMb* self) {
    Animation_Change_00375c08(&self->skelAnime, 3, 0.0f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 3), 0, -4.0f);
    self->actor.speedXZ = 0.59999996f;
    self->timer1 = Rand_S16Offset_003702c8(50, 70);
    self->unk_900 = 1;
    self->state = 9;
    self->actionFunc = FUN_003e9230;
}

typedef struct EnMbPathEntry {
    u8 count;
    u8 pad[3];
    Vec3s* points;
} EnMbPathEntry;

void FUN_0010b9f8(EnMb* self, PlayState* play);

void FUN_0036cbc4(EnMb* self, PlayState* play, s32 flag) {
    Animation_MorphToLoop(&self->skelAnime, 1, -4.0f);
    self->actor.speedXZ = 0.0f;
    self->timer1 = Rand_S16Offset_003702c8(40, 80);
    self->state = 6;
    if (flag != 0) {
        EnMbPathEntry* path = *(EnMbPathEntry**)((u8*)play + 0x5C20) + self->unk_929;
        Vec3s* waypointPos;

        if (self->unk_928 == 0) {
            self->unk_92A = 1;
        } else if (self->unk_928 == (s8)(path->count - 1)) {
            self->unk_92A = -1;
        }
        self->unk_928 += self->unk_92A;
        waypointPos = path->points + self->unk_928;
        self->unk_910.x = waypointPos->x;
        self->unk_910.y = waypointPos->y;
        self->unk_910.z = waypointPos->z;
    }
    self->actionFunc = FUN_0010b9f8;
}
}
