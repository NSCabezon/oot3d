#ifndef _MODEL_MANAGER_HPP_
#define _MODEL_MANAGER_HPP_

#include "global.hpp"

// Shared singleton reached through a function-local static in an inline function. armcc inlines the
// guard check + ctor + atexit registration into every caller (~384 ROM functions reference it).
//   object  GetModelManager()::sInstance  @ 0x005BE5B8 (.bss__ZZ15GetModelManagervE9sInstance, pinned in oot3d.ld)
//   guard   (same, _ZGV...)                @ 0x0055A21C (.data__ZZ15GetModelManagervE9sInstance, pinned in oot3d.ld)
//   ctor    FUN_0036788c (returns this), dtor V00 @ 0x0048B1F0 (aliases in src/game/model_manager/model_manager_syms.s)
// __cxa_guard_release / __aeabi_atexit are empty -> linker-inlined `mov r0, r0`.

extern "C++" {

struct SkeletonAnimationModel;

class ModelAllocator {
public:
    virtual void vf00();
    virtual void vf04();
    virtual SkeletonAnimationModel* Create(void* cmb, s32 arg);

    /* 0x04 */ s32 unk_04;
    /* 0x08 */ void* owner;
};

class ModelManager {
public:
    /* 0x000 */ char unk_000[0x17C];
    /* 0x17C */ ModelAllocator* allocator;
    /* 0x180 */ char unk_180[0x247C - 0x180];
    /* 0x247C */ void* unk_247C; // used by En_Jsjutan (Destroy/Update model frees)
    // size >= 0x2480 (ctor touches fields up to ~0x5000)

    ModelManager();
    ~ModelManager();
};

inline ModelManager& GetModelManager() {
    static ModelManager sInstance;
    return sInstance;
}

} // extern "C++"

#endif
