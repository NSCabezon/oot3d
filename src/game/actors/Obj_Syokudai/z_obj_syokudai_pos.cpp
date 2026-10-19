extern "C" {
#include "z_obj_syokudai.hpp"
#include "model_manager.hpp"

// Own TU: FUN_003fb408 has a guarded function-local static whose guard is pinned at its ROM address
// (pin_static --guard-sel "z_obj_syokudai_pos.o(+DATA)"); the actor TU's .data sits elsewhere.
s32 FUN_003478bc(void* a, s32 b);
void FUN_00313c18(Actor* actor, s32 idx, Vec3f* out);

void FUN_003fb408(ObjSyokudai* self, Vec3f* out) {
    if (self->actor.scale.y >= 0.0f) {
        Vec3f tmp[3]; // callee writes a 0x24-byte block, only the first Vec3f is read
        s32 idx = FUN_003478bc(*(void**)((u8*)self + 0x27C), 0x10);
        FUN_00313c18(&self->actor, idx, &tmp[0]);
        *out = tmp[0];
    } else {
        static Vector3f sZero(0.0f, 0.0f, 0.0f);
        *out = *(Vec3f*)&sZero;
    }
}

}
