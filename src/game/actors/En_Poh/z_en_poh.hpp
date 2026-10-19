#ifndef _Z_EN_POH_H_
#define _Z_EN_POH_H_

#include "global.hpp"

struct EnPoh;

typedef void (*EnPohActionFunc)(struct EnPoh* self, PlayState* play);

// Joint-sphere element as the draw code reads it (world sphere centre at +0x38).
typedef struct EnPohJntSphElement {
    /* 0x00 */ char unk_00[0x38];
    /* 0x38 */ Vec3f worldCenter;
} EnPohJntSphElement;

typedef struct EnPoh {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x6E8];
    /* 0x0910 */ SkeletonAnimationModel* skelAnimModel[2];
    /* 0x0918 */ EnPohActionFunc actionFunc;
    /* 0x091C */ u8 unk_91C;
    /* 0x091D */ u8 unk_91D;
    /* 0x091E */ s8 unk_91E;
    /* 0x091F */ u8 infoIdx;
    /* 0x0920 */ s16 unk_920;
    /* 0x0922 */ s16 unk_922;
    /* 0x0924 */ s16 unk_924;
    /* 0x0926 */ u8 lightColorR;
    /* 0x0927 */ u8 lightColorG;
    /* 0x0928 */ u8 lightColorB;
    /* 0x0929 */ u8 lightColorA;
    /* 0x092A */ u8 envColorR;
    /* 0x092B */ u8 envColorG;
    /* 0x092C */ u8 envColorB;
    /* 0x092D */ u8 envColorA;
    /* 0x092E */ char unk_92E[0x5];
    /* 0x0933 */ u8 infoLightR;
    /* 0x0934 */ u8 infoLightG;
    /* 0x0935 */ u8 infoLightB;
    /* 0x0936 */ u8 limbIndexToGlow;
    /* 0x0937 */ char unk_937[0x5];
    /* 0x093C */ void* unk_93C;
    /* 0x0940 */ void* unk_940;
    /* 0x0944 */ char unk_944[0x4];
    /* 0x0948 */ void* unk_948;
    /* 0x094C */ SkeletonAnimationModel* skelAnimModelDeath;
    /* 0x0950 */ SkeletonAnimationModel* skelAnimModelSoul;
    /* 0x0954 */ void* lanternDisplayList;
    /* 0x0958 */ void* lightNode;
    /* 0x095C */ char lightInfo[0x18];
    /* 0x0974 */ ColliderCylinder colliderCylinder;
    /* 0x09CC */ char unk_9CC[0x1C];
    /* 0x09E8 */ EnPohJntSphElement* jntSphElements;
    /* 0x09EC */ char unk_9EC[0x50];
    /* 0x0A3C */ MTX34 limbMtxCopy;
} EnPoh; // size = 0xA6C

extern ActorInit En_Poh_InitVars;

#endif
