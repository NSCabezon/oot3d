extern "C" {
#include "global.hpp"

typedef struct DupRiseActor {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x84];
    /* 0x0228 */ char unk_228[0x1CC];
    /* 0x03F4 */ char unk_3F4[0x8];
    /* 0x03FC */ s32 updateIndex;
} DupRiseActor;

static inline void DupRise_Rise(DupRiseActor* self) {
    f32* y = &self->actor.shape.yOffset;
    *y += 250.0f / 3.0f;
}
static inline void DupRise_EndRise(DupRiseActor* self) {
    f32* y = &self->actor.shape.yOffset;
    if (self->actor.shape.yOffset >= 0.0f) {
        self->updateIndex = 3;
        *y = 0.0f;
    }
}
void caseD_2_00243f54(DupRiseActor* self, PlayState* play) {
    DupRise_Rise(self);
    FUN_00370734((SkelAnime*)self->unk_1A4);
    DupRise_EndRise(self);
}

}
