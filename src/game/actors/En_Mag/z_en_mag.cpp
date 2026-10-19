extern "C" {
#include "z_en_mag.hpp"
#include "model_manager.hpp"

#define FLAGS 0x00000030

void EnMag_Init(EnMag* self, PlayState* play);
void EnMag_Destroy(Actor* thisx, PlayState* play);

// Virtual objects (slot 1 = Destroy). _ZTI8EnMagObj is an EQU in shared_syms.s (VFE key, --vfemode=force).
extern "C++" {
struct EnMagObj {
    virtual void unk_vf0();
    virtual void Destroy();
};
}
void EnMag_Update(Actor* actor, GameState* state);
void EnMag_Draw(Actor* actor, GameState* state);

ActorInit En_Mag_InitVars = {
    ACTOR_EN_MAG,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_MAG,
    sizeof(EnMag),
    (ActorFunc)EnMag_Init,
    (ActorFunc)EnMag_Destroy,
    (ActorFunc)EnMag_Update,
    (ActorFunc)EnMag_Draw,
};

GLOBAL_ASM("../src/game/actors/En_Mag/data_syms.s")

void EnMag_Destroy(Actor* thisx, PlayState* play) {
    EnMag* self = (EnMag*)thisx;

    if (self->unk_1A4 != NULL) {
        self->unk_1A4->Destroy();
    }
    self->unk_1A4 = NULL;
    if (self->unk_1A8 != NULL) {
        self->unk_1A8->Destroy();
    }
    self->unk_1A8 = NULL;
    if (self->unk_1AC != NULL) {
        self->unk_1AC->Destroy();
    }
    self->unk_1AC = NULL;
}



void FUN_0033d174(u8* self, s32 index, f32* a, f32* b, f32* c, f32* d) {
    f32* dst;
    f32 a0;
    if (index >= 3) {
        return;
    }
    a0 = a[0];
    dst = (f32*)(self + index * 0x60);
    dst[0x22] = a0;
    dst[0x26] = b[0];
    dst[0x2A] = c[0];
    dst[0x2E] = d[0];
    dst[0x23] = a[1];
    dst[0x27] = b[1];
    dst[0x2B] = c[1];
    dst[0x2F] = d[1];
    dst[0x24] = a[2];
    dst[0x28] = b[2];
    dst[0x2C] = c[2];
    dst[0x30] = d[2];
    dst[0x25] = a[3];
}

void FUN_0031487c(void* self, void* entry, void* arg);

void FUN_0033d220(u8* self, void* arg) {
    u8* base = self + 0x2000;
    s32 count = *(s32*)(base + 0x12C);
    if (count >= 8) {
        return;
    }
    {
        u8* entry = self + count * 8;
        entry += 0x2000;
        entry += 0x130;
        FUN_0031487c(self, entry, arg);
    }
    *(s32*)(base + 0x12C) = *(s32*)(base + 0x12C) + 1;
    {
        void (*fn)(void*) = *(void (**)(void*))((*(u8**)arg) + 8);
        fn(arg);
    }
}

typedef struct MagObj {
    /* 0x00 */ char unk_00[4];
    /* 0x04 */ void* unk_04;
    /* 0x08 */ f32 unk_08;
    /* 0x0C */ char unk_0C[4];
    /* 0x10 */ u8 unk_10;
} MagObj;

typedef struct MagHolder {
    /* 0x00 */ char unk_00[8];
    /* 0x08 */ MagObj* unk_08;
} MagHolder;

void FUN_003ff284(MagObj* obj, void** p);
void FUN_003ff2c4(MagObj* obj, f32 f);

void FUN_00348054(MagHolder* holder, f32 f) {
    if (holder->unk_08 != NULL) {
        FUN_003ff2c4(holder->unk_08, f);
    }
}

void FUN_00348068(MagHolder* holder, void** p) {
    if (holder->unk_08 != NULL) {
        FUN_003ff284(holder->unk_08, p);
    }
}

typedef struct MagWork {
    /* 0x000 */ char unk_000[4];
    /* 0x004 */ u8 unk_04;
    /* 0x005 */ u8 unk_05;
    /* 0x006 */ char unk_006;
    /* 0x007 */ u8 unk_07;
    /* 0x008 */ u8 unk_08;
    /* 0x009 */ char unk_009[0x6];
    /* 0x00F */ u8 unk_0F;
    /* 0x010 */ f32 unk_10;
    /* 0x014 */ f32 unk_14;
    /* 0x018 */ s32 unk_18;
    /* 0x01C */ char unk_01C[0x3C];
    /* 0x058 */ f32 unk_58;
    /* 0x05C */ char unk_05C[0x38];
    /* 0x094 */ f32 unk_94;
    /* 0x098 */ char unk_098[0x60];
    /* 0x0F8 */ f32 unk_F8;
    /* 0x0FC */ char unk_0FC[0xC];
    /* 0x108 */ f32 unk_108;
    /* 0x10C */ char unk_10C[0x38];
    /* 0x144 */ f32 unk_144;
    /* 0x148 */ char unk_148[0xB4];
    /* 0x1FC */ f32 unk_1FC;
    /* 0x200 */ char unk_200[0x38];
    /* 0x238 */ f32 unk_238;
    /* 0x23C */ char unk_23C[0x234];
    /* 0x470 */ s32 unk_470;
} MagWork;

extern char EnMag_sObj587934;
void FUN_0030765c(void* p);
void FUN_00307668(void* p);

void FUN_00347fbc(MagWork* self, s32 mode) {
    if (mode != 0) {
        self->unk_04 = 8;
        self->unk_58 = 14.0f;
        self->unk_94 = 168.0f;
        self->unk_1FC = 12.0f;
        self->unk_238 = 166.0f;
        self->unk_108 = 1.0f;
        self->unk_F8 = 1.0f;
        self->unk_144 = 0.0f;
        self->unk_0F = 1;
        self->unk_470 = 0;
        self->unk_10 = 0.0f;
    } else {
        self->unk_04 = 3;
        self->unk_470 = 0xD;
        self->unk_10 = 32.0f;
        self->unk_18 = 0;
    }
}

void FUN_0040a020(MagWork* self) {
    if (self->unk_05 == 0) {
        self->unk_08 = 1;
    } else {
        self->unk_08 = 2;
    }
    self->unk_04 = 4;
    self->unk_07 = 0;
    self->unk_470 = 0x14;
    self->unk_14 = 0.5f;
    self->unk_144 = 0.0f;
    if (self->unk_08 == 2) {
        FUN_0030765c(&EnMag_sObj587934);
    } else {
        FUN_00307668(&EnMag_sObj587934);
    }
}


typedef struct MagTable12 {
    u8 b[12];
} MagTable12;

u8 FUN_00410350(void* self) {
    s32 idx = *(s32*)((u8*)self + 0xF3C);
    MagTable12 table = *(MagTable12*)0x004ED37C;
    return table.b[idx];
}


// EnMag_Draw draft (own TU needed: FUN_0033d174/FUN_0033d220 are defined in this file). ~150 insn-diffs, blocker =
// MTX34_MakeTrans zero/one rematerialisation (ROM re-mov r0,#0 / r1,#0x3f800000 per matrix; armcc keeps r5/r6) + stack
// layout (0xf8 vs 0xe8 frame) + hoisted s17/s19/s18 constants. Needs data_syms.s EnMag_sColA..D (already there).
#if 0

// Virtual objects (slot 1 = Destroy, slot 2 = hide). _ZTI8EnMagObj is an EQU in shared_syms.s (VFE key).
extern "C++" {
struct EnMagObj {
    virtual void unk_vf0();
    virtual void Destroy();
    virtual void unk_vf8();
};
}

typedef struct MagVec4 {
    f32 v[4];
} MagVec4;

extern const MagVec4 EnMag_sColA;
extern const MagVec4 EnMag_sColB;
extern const MagVec4 EnMag_sColC[4];
extern const MagVec4 EnMag_sColD;

void FUN_0036c174(MTX34* dst, MTX34* a, MTX34* b);
void* FUN_003687a8(void* model);
void FUN_003589cc(void* obj, s32 idx);
void FUN_00358964(void* obj, s32 idx, f32* col);
void FUN_0033d200(void* obj);
void FUN_0033d174(void* obj, s32 index, f32* a, f32* b, f32* c, f32* d);
void FUN_0033d14c(void* obj, s32 idx, f32* v);
void FUN_0033d220(void* mgr, void* model);

#define MAG_MODEL(m) ((SkeletonAnimationModel*)(m))

void EnMag_Draw(Actor* thisx, GameState* state) {
    EnMag* self = (EnMag*)thisx;
    MTX34 m1;
    MTX34 m2;
    MTX34 m3;
    f32 zero = 0.0f;
    f32 one = 1.0f;
    f32 k255 = 0.003921569f;

    {
        Vector3f t1(zero, zero, -34.0f);
        MTX34_MakeTrans(&m1, (Vec3f*)&t1);
    }
    {
        Vector3f t2(zero, zero, 0.01f);
        MTX34_MakeTrans(&m2, (Vec3f*)&t2);
    }
    FUN_0036c174(&m2, &m1, &m2);
    {
        Vector3f t3(zero, -11.0f, zero);
        MTX34_MakeTrans(&m3, (Vec3f*)&t3);
    }
    FUN_0036c174(&m3, &m1, &m3);

    if (self->unk_1A8 != NULL) {
        if (self->unk_1D0 > zero) {
            void* o;
            MagVec4 col;
            col = EnMag_sColA;
            col.v[3] = self->unk_1D0 * k255;
            o = FUN_003687a8(self->unk_1A8);
            FUN_003589cc(o, 5);
            FUN_00358964(o, 5, col.v);
            MAG_MODEL(self->unk_1A8)->FUN_003721e0(&m2);
            MAG_MODEL(self->unk_1A8)->SetUnkAC(1);
            FUN_0033d220((u8*)&GetModelManager() + 0x180, self->unk_1A8);
        } else {
            self->unk_1A8->unk_vf8();
        }
    }
    if (self->unk_1A4 != NULL && self->unk_1D4 > zero) {
        void* o;
        MagVec4 col;
        MagVec4 c0;
        MagVec4 c1;
        MagVec4 c2;
        MagVec4 c3;
        Vec3f dir;
        Vec3f lit;
        f32 a;

        o = FUN_003687a8(self->unk_1A4);
        col = EnMag_sColB;
        col.v[3] = self->unk_1D4 * k255;
        FUN_003589cc(o, 5);
        FUN_00358964(o, 5, col.v);
        c0 = EnMag_sColC[0];
        c1 = EnMag_sColC[1];
        c2 = EnMag_sColC[2];
        c3 = EnMag_sColC[3];
        a = self->unk_1DC * k255;
        if (a > one) {
            a = one;
        }
        lit.x = a * 0.5f - one;
        lit.y = one - a * 0.5f;
        lit.z = -0.5f - a * 0.5f;
        dir.x = m1.data[0][0] * lit.x + m1.data[0][1] * lit.y + m1.data[0][2] * lit.z;
        dir.y = m1.data[1][0] * lit.x + m1.data[1][1] * lit.y + m1.data[1][2] * lit.z;
        dir.z = m1.data[2][0] * lit.x + m1.data[2][1] * lit.y + m1.data[2][2] * lit.z;
        {
            f32 inv = one / __sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
            dir.x *= inv;
            dir.y *= inv;
            dir.z *= inv;
        }
        FUN_0033d200(o);
        FUN_0033d174(o, 0, c0.v, c1.v, c2.v, c3.v);
        FUN_0033d14c(o, 0, &dir.x);
        MAG_MODEL(self->unk_1A4)->FUN_003721e0(&m1);
        MAG_MODEL(self->unk_1A4)->SetUnkAC(1);
        FUN_0033d220((u8*)&GetModelManager() + 0x180, self->unk_1A4);
    }
    if (self->unk_1AC != NULL && self->unk_1D8 > zero) {
        void* o;
        MagVec4 col;
        col = EnMag_sColD;
        col.v[3] = self->unk_1D8 * k255;
        o = FUN_003687a8(self->unk_1AC);
        FUN_003589cc(o, 5);
        FUN_00358964(o, 5, col.v);
        MAG_MODEL(self->unk_1AC)->FUN_003721e0(&m3);
        MAG_MODEL(self->unk_1AC)->SetUnkAC(1);
        FUN_0033d220((u8*)&GetModelManager() + 0x180, self->unk_1AC);
    }
}
#endif

GLOBAL_ASM("asm/EnMag_Draw.s")

// EnMag_Update draft (own TU needed). ~12 real insn-diffs left (+ jump-table words): state==2&&unk_08 predicated chain,
// csab link r0/r1 order, case0 arg order (csab then unk_1A4 in r2), final `unk_1C0==0` test predicated into the zero chain.
#if 0

typedef struct MagWork {
    /* 0x00 */ char unk_00[7];
    /* 0x07 */ u8 unk_07;
    /* 0x08 */ u8 unk_08;
    /* 0x09 */ char unk_09[2];
    /* 0x0B */ u8 unk_0B;
} MagWork;

typedef struct MagHolder MagHolder;

typedef struct MagCsabData {
    /* 0x00 */ char unk_00[0x10];
    /* 0x10 */ s32 unk_10;
} MagCsabData;

typedef struct MagCsabLink {
    /* 0x00 */ char unk_00[0x14];
    /* 0x14 */ s32 unk_14;
} MagCsabLink;

typedef struct MagCsab {
    /* 0x00 */ MagCsabLink* link;
} MagCsab;

void* ZAR_GetCSABByIndex(void* zar, s32 index);
s32 FUN_003487ec(void* p);
void FUN_00347fbc(MagWork* self, s32 mode);
void FUN_0040a020(MagWork* self);
void FUN_00348068(MagHolder* holder, void** p);
void FUN_00348054(MagHolder* holder, f32 f);
void FUN_0033d13c(s32 arg);
s32 Flags_GetEnv(PlayState* play, s32 flag);
extern char EnMag_sObj587934;
extern MagWork* EnMag_sWork;

#define MAG_SAVE_U8(off) (*(u8*)((u8*)&gSaveContext + (off)))
#define MAG_PLAY_U8(off) (*(u8*)((u8*)play + (off)))

void EnMag_Update(Actor* thisx, GameState* state) {
    EnMag* self = (EnMag*)thisx;
    PlayState* play = (PlayState*)state;
    MagWork* work;
    s16 st;

    if (gSaveContext.fileNum != 0xFEDC) {
        work = EnMag_sWork;
        st = self->unk_1C8;
        if (st <= 1) {
            if (work->unk_0B == 0) {
                goto L6C;
            }
            self->unk_1D4 = 255.0f;
            self->unk_1D8 = 255.0f;
            self->unk_1D0 = 255.0f;
            self->unk_1DC = 255.0f;
            {
                MagCsab* csab = (MagCsab*)ZAR_GetCSABByIndex((u8*)play->GetObjectStatus(self->actor.objBankIndex) + 0x10, self->unk_1CE);
                MagCsabLink* link;
                MagCsabData* data;
                s32 n;
                FUN_00348068((MagHolder*)self->unk_1A4, (void**)csab);
                link = csab->link;
                data = (MagCsabData*)((u8*)link + link->unk_14);
                n = data->unk_10 + 1;
                FUN_00348054((MagHolder*)self->unk_1A4, (f32)n);
            }
            MAG_SAVE_U8(0x15A6) = 1;
            MAG_SAVE_U8(0x15A7) = 255;
            if (FUN_003487ec(&EnMag_sObj587934) != 0) {
                Audio_PlaySoundGeneral(0x010004A8, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
                self->unk_1C8 = 2;
                self->unk_1C0 = 0x14;
                FUN_00347fbc(EnMag_sWork, 1);
                goto L6C;
            }
            if (play->sceneLoadFlag != 0x14) {
                FUN_0033d13c(0);
                Audio_PlaySoundGeneral(0x010004C0, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
            }
            if (self->unk_1C8 <= 1) {
                self->unk_1C0 = 0x19;
                self->unk_1C8 = 4;
            }
            self->unk_1C2 = 1;
            FUN_0040a020(EnMag_sWork);
            goto L6C;
        }
        if (st < 2) {
            goto LEE0;
        }
        if (self->unk_1C0 != 0) {
            self->unk_1C0--;
            goto L6C;
        }
        if (self->unk_1C2 != 0) {
            goto LE38;
        }
        if (st == 2) {
            if (work->unk_08 == 0) {
                work->unk_07 = 1;
            }
        }
        if (work->unk_08 == 0) {
            goto L6C;
        }
        if (play->sceneLoadFlag != 0x14) {
            FUN_0033d13c(0);
            Audio_PlaySoundGeneral(0x010004C0, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        }
        if (self->unk_1C8 == 2 || self->unk_1C8 == 5) {
            self->unk_1C0 = 0x19;
            self->unk_1C8 = 4;
        } else if (self->unk_1C8 == 3) {
            self->unk_1C0 = 0x19;
            self->unk_1C8 = 6;
        }
        self->unk_1C2 = 1;
    }
L6C:
    if (self->unk_1C8 != 1) {
        goto LE38;
    }
    switch (self->unk_1C4) {
        case 0: {
            MagCsab* csab = (MagCsab*)ZAR_GetCSABByIndex((u8*)play->GetObjectStatus(self->actor.objBankIndex) + 0x10, self->unk_1CE);
            {
                MagHolder* h = (MagHolder*)self->unk_1A4;
                FUN_00348068(h, (void**)csab);
            }
            self->unk_1C6 = 0x26;
            self->unk_1C4++;
            goto LF8;
        }
        case 1:
            self->unk_1C6--;
            if (self->unk_1C6 == 0) {
                self->unk_1C6 = 0x51;
                self->unk_1C4 = 2;
            }
            return;
        case 2:
            self->unk_1D4 += 3.0f;
            self->unk_1C6--;
            if (self->unk_1C6 != 0) {
                return;
            }
            self->unk_1D4 = 255.0f;
            self->unk_1C6 = 0x3C;
            self->unk_1C4 = 3;
            return;
        case 3:
            self->unk_1D0 += 4.25f;
            self->unk_1DC += 4.25f;
            self->unk_1C6--;
            if (self->unk_1C6 != 0) {
                return;
            }
            self->unk_1D0 = 255.0f;
            self->unk_1DC = 255.0f;
            self->unk_1C4 = 4;
            FUN_00347fbc(EnMag_sWork, 0);
            goto LF8;
        case 4:
            self->unk_1D8 += (f32)self->unk_1CA;
            if (self->unk_1D8 < 255.0f) {
                return;
            }
            self->unk_1D8 = 255.0f;
            self->unk_1C8 = 2;
            self->unk_1C4 = 5;
            self->unk_1C0 = 0x14;
            goto L44;
        default:
            goto LF8;
    }
LE38:
    if (self->unk_1C8 == 3) {
        {
            f32 nv = self->unk_1D0 - (f32)self->unk_1CC;
            self->unk_1D0 = nv;
            if (nv < 0.0f) {
                self->unk_1D0 = 0.0f;
            }
        }
        {
            f32 nv = self->unk_1D4 - (f32)self->unk_1CC;
            self->unk_1D4 = nv;
            if (nv < 0.0f) {
                self->unk_1D4 = 0.0f;
            }
        }
        {
            f32 nv = self->unk_1D8 - (f32)self->unk_1CC;
            self->unk_1D8 = nv;
            if (nv < 0.0f) {
                self->unk_1D8 = 0.0f;
            }
        }
        if (self->unk_1D0 == 0.0f && self->unk_1D4 == 0.0f && self->unk_1D8 == 0.0f) {
            self->unk_1C8 = 5;
        }
        return;
    }
LEE0:
    if (self->unk_1C8 == 4) {
        if (self->unk_1C0 > 0) {
            return;
        }
        if (play->sceneLoadFlag != 0x14) {
            gSaveContext.gameMode = 2;
            play->sceneLoadFlag = 0x14;
            play->fadeOutTransition = 2;
        }
        self->unk_1CA = 0xF;
        self->unk_1CC = 0x19;
        self->unk_1C8 = 3;
        return;
    }
    if (self->unk_1C8 == 6) {
        {
            f32 nv = self->unk_1D0 - (f32)self->unk_1CC;
            self->unk_1D0 = nv;
            if (nv < 0.0f) {
                self->unk_1D0 = 0.0f;
            }
        }
        {
            f32 nv = self->unk_1D4 - (f32)self->unk_1CC;
            self->unk_1D4 = nv;
            if (nv < 0.0f) {
                self->unk_1D4 = 0.0f;
            }
        }
        {
            f32 nv = self->unk_1D8 - (f32)self->unk_1CC;
            self->unk_1D8 = nv;
            if (nv < 0.0f) {
                self->unk_1D8 = 0.0f;
            }
        }
        if (self->unk_1D0 == 0.0f && self->unk_1D4 == 0.0f && self->unk_1D8 == 0.0f) {
            if (self->unk_1C0 == 0) {
                self->unk_1C8 = 5;
                if (play->sceneLoadFlag != 0x14) {
                    gSaveContext.gameMode = 2;
                    play->sceneLoadFlag = 0x14;
                    play->fadeOutTransition = 2;
                }
            }
        }
        return;
    }
LF8:
    if (self->unk_1C8 == 0) {
        if (Flags_GetEnv(play, 3)) {
            self->unk_1C6 = 0x28;
            self->unk_1C8 = 1;
        }
        return;
    }
    if (self->unk_1C8 != 2) {
        return;
    }
L44:
    if (Flags_GetEnv(play, 4)) {
        self->unk_1C8 = 3;
    }
}
#endif

GLOBAL_ASM("asm/EnMag_Update.s")

}
