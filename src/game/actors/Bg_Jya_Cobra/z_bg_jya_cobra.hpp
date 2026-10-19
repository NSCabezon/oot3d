#ifndef _Z_BG_JYA_COBRA_H_
#define _Z_BG_JYA_COBRA_H_
#include "global.hpp"
struct BgJyaCobra;
typedef void (*BgJyaCobraActionFunc)(BgJyaCobra*, PlayState*);
typedef struct BgJyaCobra {
    DynaPolyActor dyna; // 0x000
    BgJyaCobraActionFunc actionFunc; // 0x1BC
    s16 pushTimer; // 0x1C0
    s16 direction; // 0x1C2
    s16 rotationIndex; // 0x1C4
    s16 rotationSpeed; // 0x1C6
    s16 rotation; // 0x1C8
    u8 movePlayer; // 0x1CA
    u8 pad_1CB;
    Vec3f playerOffset; // 0x1CC
    Vec3f source; // 0x1D8
    f32 intensity; // 0x1E4
    f32 length; // 0x1E8
    char shadowTextureBuffer[0x2020]; // 0x1EC
    u16* shadowTexture; // 0x220C
    SkeletonAnimationModel* models[3]; // 0x2210
    void* object; // 0x221C
    SkeletonAnimationModel_unk_0C materialAnim; // 0x2220
    void* textureModel; // 0x22B8
    void* managedModel; // 0x22BC
    char textureState[2][0x54]; // 0x22C0
    char textureInfo[0x24]; // 0x2368
    s32 textureIndex; // 0x238C
    s32 unk_2390;
} BgJyaCobra;
extern ActorInit Bg_Jya_Cobra_InitVars;
#endif
