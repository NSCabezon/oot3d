#ifndef _Z_BOSS_DODONGO_H_
#define _Z_BOSS_DODONGO_H_

#include "global.hpp"
#include <stddef.h>

struct BossDodongo;

typedef struct BossDodongoEffect {
    Vec3f pos,velocity,accel;
    u8 active,timer;
    u8 color[3];
    s16 alpha;
    f32 scale;
    SkeletonAnimationModel* model;
} BossDodongoEffect;
typedef char BossDodongoEffectSizeCheck[(sizeof(BossDodongoEffect)==0x34)?1:-1];

typedef struct BossDodongo {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ void* modelPointers[80];
    /* 0x0368 */ void* texture;
    /* 0x036C */ char unk_36C[0x94];
    /* 0x0400 */ char unk_400[0x300];
    /* 0x0700 */ char unk_700[0x5C];
    s16 unk_75C; char unk_75E[2];
    /* 0x0760 */ void (*actionFunc)(struct BossDodongo*, PlayState*);
    /* 0x0764 */ s16 unk_764;
    /* 0x0766 */ s16 unk_766;
    /* 0x0768 */ s16 unk_768;
    /* 0x076A */ s16 unk_76A;
    /* 0x076C */ s16 unk_76C;
    /* 0x076E */ s16 unk_76E;
    /* 0x0770 */ s16 unk_770;
    /* 0x0772 */ s16 unk_772;
    /* 0x0774 */ s16 unk_774;
    /* 0x0776 */ s16 unk_776;
    /* 0x0778 */ s16 unk_778;
    /* 0x077A */ s16 unk_77A;
    /* 0x077C */ s16 unk_77C;
    /* 0x077E */ s16 unk_77E;
    /* 0x0780 */ s16 unk_780;
    /* 0x0782 */ s16 unk_782;
    /* 0x0784 */ s16 unk_784;
    /* 0x0786 */ s16 unk_786;
    /* 0x0788 */ s16 unk_788;
    /* 0x078A */ s16 unk_78A;
    /* 0x078C */ s16 unk_78C;
    /* 0x078E */ s16 unk_78E;
    /* 0x0790 */ s16 unk_790;
    /* 0x0792 */ s16 unk_792;
    /* 0x0794 */ s16 unk_794;
    /* 0x0796 */ s16 unk_796;
    /* 0x0798 */ s16 unk_798;
    /* 0x079A */ s16 unk_79A;
    /* 0x079C */ s16 unk_79C;
    /* 0x079E */ s16 unk_79E;
    /* 0x07A0 */ s16 unk_7A0;
    /* 0x07A2 */ s16 unk_7A2;
    /* 0x07A4 */ s16 unk_7A4;
    /* 0x07A6 */ s16 unk_7A6;
    /* 0x07A8 */ s16 unk_7A8;
    /* 0x07AA */ s16 unk_7AA;
    /* 0x07AC */ s16 unk_7AC;
    /* 0x07AE */ s16 unk_7AE;
    /* 0x07B0 */ s16 unk_7B0;
    /* 0x07B2 */ u8 unk_7B2;
    /* 0x07B3 */ s8 unk_7B3;
    /* 0x07B4 */ f32 unk_7B4;
    /* 0x07B8 */ f32 unk_7B8;
    /* 0x07BC */ f32 unk_7BC;
    /* 0x07C0 */ f32 unk_7C0;
    /* 0x07C4 */ f32 unk_7C4;
    /* 0x07C8 */ f32 unk_7C8;
    /* 0x07CC */ f32 unk_7CC;
    /* 0x07D0 */ f32 unk_7D0;
    /* 0x07D4 */ f32 unk_7D4;
    /* 0x07D8 */ f32 unk_7D8;
    /* 0x07DC */ f32 unk_7DC;
    /* 0x07E0 */ f32 unk_7E0;
    /* 0x07E4 */ f32 unk_7E4;
    /* 0x07E8 */ f32 unk_7E8;
    /* 0x07EC */ f32 unk_7EC;
    /* 0x07F0 */ f32 unk_7F0;
    /* 0x07F4 */ f32 unk_7F4;
    /* 0x07F8 */ f32 unk_7F8;
    /* 0x07FC */ f32 unk_7FC;
    /* 0x0800 */ f32 unk_800;
    /* 0x0804 */ f32 unk_804;
    /* 0x0808 */ f32 unk_808;
    /* 0x080C */ f32 unk_80C;
    /* 0x0810 */ f32 unk_810;
    /* 0x0814 */ f32 unk_814;
    /* 0x0818 */ f32 unk_818;
    /* 0x081C */ f32 unk_81C;
    /* 0x0820 */ char unk_820[0xC];
    /* 0x082C */ f32 unk_82C[50];
    /* 0x08F4 */ f32 unk_8F4[50];
    /* 0x09BC */ f32 unk_9BC;
    /* 0x09C0 */ f32 unk_9C0;
    /* 0x09C4 */ f32 unk_9C4;
    /* 0x09C8 */ f32 unk_9C8;
    /* 0x09CC */ f32 unk_9CC;
    /* 0x09D0 */ f32 unk_9D0;
    /* 0x09D4 */ f32 unk_9D4;
    /* 0x09D8 */ f32 unk_9D8;
    /* 0x09DC */ f32 unk_9DC;
    /* 0x09E0 */ f32 unk_9E0;
    /* 0x09E4 */ f32 unk_9E4;
    /* 0x09E8 */ f32 unk_9E8;
    /* 0x09EC */ f32 unk_9EC;
    /* 0x09F0 */ f32 unk_9F0;
    /* 0x09F4 */ f32 unk_9F4;
    /* 0x09F8 */ f32 unk_9F8;
    /* 0x09FC */ f32 unk_9FC;
    /* 0x0A00 */ f32 unk_A00;
    /* 0x0A04 */ f32 unk_A04;
    /* 0x0A08 */ f32 unk_A08;
    /* 0x0A0C */ f32 unk_A0C;
    /* 0x0A10 */ char collider[0x20];
    /* 0x0A30 */ char colliderElements[0x5F0];
    /* 0x1020 */ BossDodongoEffect effects[80];
    /* 0x2060 */ char unk_2060[0xC];
} BossDodongo; // size = 0x206C

typedef char BossDodongoSizeCheck[(sizeof(BossDodongo) == 0x206C) ? 1 : -1];

typedef char BossDodongoSkelOffsetCheck[(offsetof(BossDodongo, skelAnime) == 0x1A4) ? 1 : -1];
typedef char BossDodongoActionOffsetCheck[(offsetof(BossDodongo, actionFunc) == 0x760) ? 1 : -1];
typedef char BossDodongoColliderOffsetCheck[(offsetof(BossDodongo, collider) == 0xA10) ? 1 : -1];

typedef char BossDodongoOffset_modelPointers[(offsetof(BossDodongo, modelPointers) == 0x228) ? 1 : -1];
typedef char BossDodongoOffset_texture[(offsetof(BossDodongo, texture) == 0x368) ? 1 : -1];
typedef char BossDodongoOffset_unk_766[(offsetof(BossDodongo, unk_766) == 0x766) ? 1 : -1];
typedef char BossDodongoOffset_unk_768[(offsetof(BossDodongo, unk_768) == 0x768) ? 1 : -1];
typedef char BossDodongoOffset_unk_77A[(offsetof(BossDodongo, unk_77A) == 0x77A) ? 1 : -1];
typedef char BossDodongoOffset_unk_7AA[(offsetof(BossDodongo, unk_7AA) == 0x7AA) ? 1 : -1];
typedef char BossDodongoOffset_unk_7B4[(offsetof(BossDodongo, unk_7B4) == 0x7B4) ? 1 : -1];
typedef char BossDodongoOffset_unk_7C8[(offsetof(BossDodongo, unk_7C8) == 0x7C8) ? 1 : -1];
typedef char BossDodongoOffset_unk_7D8[(offsetof(BossDodongo, unk_7D8) == 0x7D8) ? 1 : -1];
typedef char BossDodongoOffset_unk_818[(offsetof(BossDodongo, unk_818) == 0x818) ? 1 : -1];
typedef char BossDodongoOffset_unk_81C[(offsetof(BossDodongo, unk_81C) == 0x81C) ? 1 : -1];
typedef char BossDodongoOffset_effects[(offsetof(BossDodongo, effects) == 0x1020) ? 1 : -1];

extern ActorInit Boss_Dodongo_InitVars;

#endif
