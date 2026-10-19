extern "C" {
#include "z_demo_im.hpp"

#define FLAGS 0x00000011

void DemoIm_Init(Actor* actor, GameState* state);
void DemoIm_Destroy(Actor* thisx, PlayState* play);
void DemoIm_Update(Actor* thisx, PlayState* play);
void DemoIm_Draw(Actor* thisx, PlayState* play);

GLOBAL_ASM("data/z_Demo_Im.data.s")

ActorInit Demo_Im_InitVars = {
    ACTOR_DEMO_IM,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_IM,
    sizeof(DemoIm),
    (ActorFunc)DemoIm_Init,
    (ActorFunc)DemoIm_Destroy,
    (ActorFunc)DemoIm_Update,
    (ActorFunc)DemoIm_Draw,
};

#if 0 /* Draft, NOT matching: DemoIm_Init (127 lines asm). See STATUS.md. */
void DemoIm_Init(Actor* thisx, PlayState* play) {
    DemoIm* self = (DemoIm*)thisx;
    void* cmb;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    DemoIm_InitCollider(thisx, play);
    if (self->actor.params < 0x13 && play->objectCtx.status[self->actor.params].segment != NULL) {
        cmb = ZAR_GetCMBByIndex(...);
    }
    /* FUN_00353e78(actor, play, &skelAnime, idx, ...) loads the model; see asm */
    SkelAnime_SetFaceAnimations(...);
    FUN_0036932c(self->faceAnim.unk_28, 8); FUN_0037266c(...,7); FUN_0036932c(...,4); FUN_0036932c(...,6);
    self->unk_C32 = 0;
    self->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    switch (self->actor.params) {
        case 2: func_80985860(self, play); break;
        case 3: func_80985E60(self, play); break;
        case 4: func_8098652C(self, play); break;
        case 5: func_809869B0(self, play); break;
        case 6: func_80987018(self, play); break;
        default: break; /* default path: FUN_00343088 with 0x4d878c, see asm */
    }
    self->actor.shape.yOffset = 30.0f;  /* asm: stores 1.0-ish to r4+0xC8 */
}
#endif

GLOBAL_ASM("asm/DemoIm_Init.s")

// Tables live in the data dump (z_Demo_Im.data.s): sActionFuncs @0x0051BD40 (31 entries), sDrawFuncs @0x0051BDBC.
// No symbol for them yet, so they are addressed by raw address (tech debt, same as Demo_Geff).
void DemoIm_Destroy(Actor* thisx, PlayState* play) {
    DemoIm* self = (DemoIm*)thisx;

    SkelAnime_Free2((SkelAnime*)((u8*)self + 0x1A4));
    ((FaceAnimations*)((u8*)self + 0x9E0))->Destroy();
    Collider_DestroyCylinder(play, (ColliderCylinder*)((u8*)self + 0xBD8));
}

void DemoIm_Update(Actor* thisx, PlayState* play) {
    DemoIm* self = (DemoIm*)thisx;

    if ((u32)self->action < 31) {
        DemoImActionFunc* sActionFuncs = (DemoImActionFunc*)0x0051BD40;

        if (sActionFuncs[self->action] != NULL) {
            sActionFuncs[self->action](self, play);
        }
    }
}

void DemoIm_Draw(Actor* thisx, PlayState* play) {
    DemoIm* self = (DemoIm*)thisx;

    if ((u32)self->drawConfig < 3) {
        DemoImDrawFunc* sDrawFuncs = (DemoImDrawFunc*)0x0051BDBC;

        if (sDrawFuncs[self->drawConfig] != NULL) {
            sDrawFuncs[self->drawConfig](self, play);
        }
    }
}
}
