extern "C" {
#include "global.hpp"

static inline void Nb_SetVec(Vec3f* v, f32 x, f32 y, f32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

void FUN_00394288(PlayState* play, s32 limbIndex, MTX34* mtx, Actor* self) {
    if (limbIndex == 0xA) {
        Actor* s = self;
        Vec3f vec1;
        Vec3f vec2;
        Nb_SetVec(&vec1, 0.0f, 10.0f, 0.0f);
        VEC3TransformAsm((Vector3f*)&vec2, mtx, (Vector3f*)&vec1);
        s->focus.pos = vec2;
        s->focus.rot.x = s->world.rot.x;
        s->focus.rot.y = s->world.rot.y;
        s->focus.rot.z = s->world.rot.z;
    }
}

}
