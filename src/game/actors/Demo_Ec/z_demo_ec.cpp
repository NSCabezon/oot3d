#include <string.h>
extern "C" {
#include "z_demo_ec.hpp"

#define FLAGS 0x00000010




void DemoEc_Init(struct DemoEc*, PlayState*);
void DemoEc_Destroy(struct DemoEc*, PlayState*);
void DemoEc_Update(struct DemoEc*, PlayState*);
void DemoEc_Draw(struct DemoEc*, PlayState*);

GLOBAL_ASM("data/z_Demo_Ec.data.s")

ActorInit Demo_Ec_InitVars = {
    ACTOR_DEMO_EC,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_EC,
    sizeof(DemoEc),
    (ActorFunc)DemoEc_Init,
    (ActorFunc)DemoEc_Destroy,
    (ActorFunc)DemoEc_Update,
    (ActorFunc)DemoEc_Draw,
};

struct DemoEcModelArg {
    u8 rest[0xa4];
};

typedef void (*DemoEcFreeModelFn)(Actor*, u32, u32, u32, DemoEcModelArg, s32);

void DemoEc_Init(DemoEc* self, PlayState* play) {
    self->unk_4FC = (s16)self->actor.params >> 8;
    *(u16*)&self->actor.params = *(u16*)&self->actor.params & 0xFF;
    if (*(u16*)&self->actor.params >= 0x23) {
        Actor_Kill(&self->actor);
    } else {
        self->updateMode = 0;
        self->drawConfig = 0;
    }
}

void DemoEc_Destroy(DemoEc* self, PlayState* play) {
    ((DemoEcFreeModelFn)FUN_003508b8)(&self->actor, self->model[0], self->model[1], self->model[2], *(DemoEcModelArg*)&self->model[3], 0);
    SkelAnime_Free2((SkelAnime*)((u8*)self + 0x1a4));
    ((FaceAnimations*)((u8*)self + 0x1330))->Destroy();
}

void DemoEc_Update(DemoEc* self, PlayState* play) {
    typedef void (*Fn)(DemoEc*, PlayState*);
        if ((u32)self->updateMode < 0x1d) {
        Fn* t = (Fn*)0x0051b67c;
        Fn f = t[self->updateMode];
        if (f != 0) {
            f(self, play);
        }
    }
}

void DemoEc_Draw(DemoEc* self, PlayState* play) {
    typedef void (*Fn)(DemoEc*, PlayState*);
        if ((u32)self->drawConfig < 0x18) {
        Fn* t = (Fn*)0x0051b6f0;
        Fn f = t[self->drawConfig];
        if (f != 0) {
            f(self, play);
        }
    }
}

void DemoEc_OverrideLimbDraw(void) {
}

}
