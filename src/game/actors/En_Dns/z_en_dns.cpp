extern "C" {
#include "z_en_dns.hpp"
#include "model_manager.hpp"

#define FLAGS 0x00000009

void EnDns_Init(Actor* thisx, PlayState* play);
void EnDns_Destroy(Actor* thisx, PlayState* play);
void EnDns_Update(Actor* thisx, PlayState* play);
void EnDns_Draw(Actor* thisx, PlayState* play);

void Rupees_ChangeBy(s16 value);
s32 Inventory_HasEmptyBottle(void);
u8 Item_CheckObtainability(s32 itemId);

GLOBAL_ASM("data/z_En_Dns.data.s")
GLOBAL_ASM("../src/game/actors/En_Dns/data_syms.s")

extern u16 EnDns_sStartingTextIds[];
extern s32 EnDns_sAnimPtrs[];
extern InitChainEntry EnDns_sInitChain[];
extern ColliderCylinderInit EnDns_sCylinderInit;
extern DnsItemEntry* EnDns_sItemEntries[];
void Collider_SetCylinderType1(PlayState* play, ColliderCylinder* collider, Actor* actor, ColliderCylinderInit* init);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);

ActorInit En_Dns_InitVars = {
    ACTOR_EN_DNS,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SHOPNUTS,
    sizeof(EnDns),
    (ActorFunc)EnDns_Init,
    (ActorFunc)EnDns_Destroy,
    (ActorFunc)EnDns_Update,
    (ActorFunc)EnDns_Draw,
};

void FUN_0025697c(void) {
}

s32 FUN_00256974(void) {
    return 0;
}

void FUN_003c3bdc(EnDns* self) {
    Rupees_ChangeBy(-self->dnsItemEntry->itemPrice);
}

void FUN_003c3bf0(EnDns* self) {
    Rupees_ChangeBy(-self->dnsItemEntry->itemPrice);
}

void FUN_0010040c(EnDns* self) {
    gSaveContext.itemGetInf[0] |= 0x800;
    Rupees_ChangeBy(-self->dnsItemEntry->itemPrice);
}

// CanBuyDekuSeeds: ROM has it as TWO symbols, FUN_0014953c (early-out head) + FUN_0014955c (rest, own push/pop), but it is ONE armcc
// function (180 B, shared literal pool). The asm file FUN_0014953c.s contains both labels; nothing else references FUN_0014955c.
// Dead `s2` alias is needed for the register numbering.
u32 FUN_0014953c(EnDns* self) {
    EnDns* s2 = self;
    if (INV_CONTENT(ITEM_SLINGSHOT) == ITEM_NONE) {
        return DNS_CANBUY_RESULT_CANT_GET_NOW;
    }

    if (CUR_CAPACITY(UPG_BULLET_BAG) <= AMMO(ITEM_SLINGSHOT)) {
        return DNS_CANBUY_RESULT_CAPACITY_FULL;
    }

    if (gSaveContext.rupees < s2->dnsItemEntry->itemPrice) {
        return DNS_CANBUY_RESULT_NEED_RUPEES;
    }

    if (Item_CheckObtainability(ITEM_SEEDS) == ITEM_NONE) {
        return DNS_CANBUY_RESULT_SUCCESS_NEW_ITEM;
    }

    return DNS_CANBUY_RESULT_SUCCESS;
}

u32 FUN_00189644(EnDns* self) {
    return (gSaveContext.rupees < self->dnsItemEntry->itemPrice) ? DNS_CANBUY_RESULT_NEED_RUPEES : DNS_CANBUY_RESULT_SUCCESS;
}

void EnDns_Destroy(Actor* thisx, PlayState* play) {
    EnDns* self = (EnDns*)thisx;

    SkelAnime_Free2(&self->skelAnime);
    Collider_DestroyCylinder(play, &self->collider);
}

#define EQUIP_TYPE_SHIELD 1
#define EQUIP_INV_SHIELD_DEKU 0

void EnDns_Wait(EnDns* self, PlayState* play);
void EnDns_Talk(EnDns* self, PlayState* play);
void EnDns_Burrow(EnDns* self, PlayState* play);
void FUN_00363cb8(Actor* actor, PlayState* play); // Actor_OfferTalkNearColChkInfoCylinder
void EnDns_ChangeAnim(EnDns* self, u8 index);

void EnDns_SetupWait(EnDns* self, PlayState* play) {
    if (self->skelAnime.curFrame >= self->skelAnime.endFrame) {
        self->actionFunc = EnDns_Wait;
        EnDns_ChangeAnim(self, DNS_ANIM_IDLE);
    }
}

u32 FUN_00189668(EnDns* self) {
    if (!Inventory_HasEmptyBottle()) {
        return DNS_CANBUY_RESULT_CAPACITY_FULL;
    }

    return (gSaveContext.rupees < self->dnsItemEntry->itemPrice) ? DNS_CANBUY_RESULT_NEED_RUPEES : DNS_CANBUY_RESULT_SUCCESS;
}

u32 FUN_001896a8(EnDns* self) {
    if (gSaveContext.inventory.equipment & gBitFlags[EQUIP_INV_SHIELD_DEKU + EQUIP_TYPE_SHIELD * 4]) {
        return DNS_CANBUY_RESULT_CAPACITY_FULL;
    }

    return (gSaveContext.rupees < self->dnsItemEntry->itemPrice) ? DNS_CANBUY_RESULT_NEED_RUPEES : DNS_CANBUY_RESULT_SUCCESS;
}

void EnDns_Wait(EnDns* self, PlayState* play) {
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 3, 2000, 0);
    self->actor.world.rot.y = self->actor.shape.rot.y;

    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->actionFunc = EnDns_Talk;
    } else {
        if ((self->collider.base.ocFlags1 & 2) || (*((u8*)self + 0x114) != 0)) {
            self->actor.flags |= 0x10000;
        } else {
            self->actor.flags &= ~0x10000;
        }
        if (self->actor.xzDistToPlayer < 130.0f) {
            FUN_00363cb8(&self->actor, play);
        }
    }
}

f32* EnDns_CurFramePtr(SkelAnime* skelAnime); // own TU (z_en_dns_anim.cpp), inlined by armlink

void EnDns_SetupBurrow(EnDns* self, PlayState* play) {
    f32 frameCount = Animation_GetLastFrame(&self->skelAnime, EnDns_sAnimPtrs[1]);

    if (*EnDns_CurFramePtr(&self->skelAnime) == frameCount) {
        Audio_PlayActorSound2(&self->actor, 0x010003F7);
        self->actor.flags |= 0x10;
        self->actionFunc = EnDns_Burrow;
        self->standOnGround = false;
        self->yInitPos = self->actor.world.pos.y;
    }
}

void EnDns_SetupBurrow(EnDns* self, PlayState* play);

void FUN_0012e264(EnDns* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == 6) && Message_ShouldAdvance(play)) {
        self->isColliderEnabled = false;
        self->actor.flags &= ~1;
        self->actor.flags |= 0x10;
        EnDns_ChangeAnim(self, DNS_ANIM_BURROW);
        self->actionFunc = EnDns_SetupBurrow;
    }
}

// NON_MATCHING draft: CanBuyArrows, 11/60 words off (capacity/gSaveContext base register choice).
u32 FUN_001616f0(EnDns* self) {
    if (Item_CheckObtainability(ITEM_BOW) == ITEM_NONE) {
        return DNS_CANBUY_RESULT_CANT_GET_NOW;
    }

    if (CUR_CAPACITY(UPG_QUIVER) <= AMMO(ITEM_BOW)) {
        return DNS_CANBUY_RESULT_CAPACITY_FULL;
    }

    return (gSaveContext.rupees < self->dnsItemEntry->itemPrice) ? DNS_CANBUY_RESULT_NEED_RUPEES : DNS_CANBUY_RESULT_SUCCESS;
}

// CanBuyDekuNuts: same story, FUN_0014949c (capacity check) + FUN_001494f0 (rest) are one 160 B armcc function.
u32 FUN_0014949c(EnDns* self) {
    EnDns* s2 = self;
    if ((CUR_CAPACITY(UPG_NUTS) != 0) && (AMMO(ITEM_NUT) >= CUR_CAPACITY(UPG_NUTS))) {
        return DNS_CANBUY_RESULT_CAPACITY_FULL;
    }

    if (gSaveContext.rupees < s2->dnsItemEntry->itemPrice) {
        return DNS_CANBUY_RESULT_NEED_RUPEES;
    }

    if (Item_CheckObtainability(ITEM_NUT) == ITEM_NONE) {
        return DNS_CANBUY_RESULT_SUCCESS_NEW_ITEM;
    }

    return DNS_CANBUY_RESULT_SUCCESS;
}

extern Color_RGBA8 DAT_00520234;
extern Color_RGBA8 DAT_00520238;
void FUN_003356d4(PlayState* play, s32 type, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor,
                  Color_RGBA8* envColor, s16 scale, s16 scaleStep, s16 life, s32 unk);

#define EN_DNS_R_UPDATE_RATE SREG(30)

// NEAR 10 words + data blocker: ROM derives &DAT_00520234 from &DAT_00520238 (`ldr r3,=DAT_00520238; sub r2,r3,#4`), i.e. both are
// statics of THIS TU's .data in ROM; here they are extern symbols owned by En_Horse. Remaining diff: r5/r6 (srcPos/&pos) swap.
#if 0
void FUN_0027f3f8(PlayState* play, Vec3f* srcPos, f32 randScale) {
    s32 i;
    Vec3f pos;
    Vec3f velocity;
    Vec3f accel;

    for (i = 0; i < 20; i++) {
        s16 randAngle;
        f32 rand;

        rand = Rand_ZeroOne() * randScale;
        randAngle = (Rand_ZeroOne() * 65536.0f);

        pos = *srcPos;

        pos.x += Math_SinS(randAngle) * rand;
        pos.z += Math_CosS(randAngle) * rand;

        velocity.y = 1.0f;
        velocity.x = Math_SinS(randAngle);
        velocity.z = Math_CosS(randAngle);

        accel.x = accel.y = accel.z = 0.0f;
        FUN_003356d4(play, 4, &pos, &velocity, &accel, &DAT_00520234, &DAT_00520238, 100, 30,
                     (s16)(21.0f / EN_DNS_R_UPDATE_RATE + 0.5f), 0);
    }
}
#endif
GLOBAL_ASM("asm/FUN_0027f3f8.s")

void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skel, void* cmb, void* p, s32 b, void* jointTable,
                  void* morphTable, s32 limbCount);
void* ZAR_GetCMBByIndex(void* zar, s32 index);

static inline u8* EnDns_GetObjBank(PlayState* play, u32 idx) {
    if (idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

void EnDns_Init(Actor* thisx, PlayState* play) {
    EnDns* self = (EnDns*)thisx;
    u8* zar;
    s16 type;
    EnDns* s2 = self;

    if (self->actor.params < 0) {
        Actor_Kill(&self->actor);
        return;
    }

    if ((self->actor.params == 6) && (gSaveContext.linkAge != 0)) {
        self->actor.params = 3;
    }

    Actor_ProcessInitChain(&self->actor, EnDns_sInitChain);

    zar = EnDns_GetObjBank(play, (u8)self->actor.objBankIndex) + 0x10;
    GetModelManager();
    FUN_00353e78(zar, play, &self->skelAnime, ZAR_GetCMBByIndex(zar, 0), self->actor.unk_178, EnDns_sAnimPtrs[2],
                 self->jointTable, self->morphTable, 0xF);

    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinderType1(play, &self->collider, &self->actor, &EnDns_sCylinderInit);

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 35.0f);
    thisx->textId = EnDns_sStartingTextIds[self->actor.params];
    Actor_SetScale(&self->actor, 0.01f);

    self->actor.colChkInfo.mass = 0xFF;
    self->actor.speedXZ = 0.0f;
    self->actor.gravity = -1.0f;
    self->actor.velocity.y = 0.0f;
    self->isColliderEnabled = true;
    self->standOnGround = true;
    self->dropCollectible = false;
    type = self->actor.params;
    self->dnsItemEntry = EnDns_sItemEntries[type];

    s2->actionFunc = EnDns_SetupWait;
    self->skelAnime.unk_75[0] = 0;
    self->actor.unk_19A = 1;
}

void EnDns_Update(Actor* thisx, PlayState* play) {
    EnDns* self = (EnDns*)thisx;

    self->dustTimer++;
    self->actor.textId = EnDns_sStartingTextIds[self->actor.params];

    Actor_SetFocus(&self->actor, 60.0f);
    Actor_SetScale(thisx, 0.01f);
    FUN_003731e0(&self->skelAnime);
    Actor_MoveForward(&self->actor);

    self->actionFunc(self, play);

    if (self->standOnGround) {
        Actor_UpdateBgCheckInfo(play, &self->actor, 20.0f, 20.0f, 20.0f, 4);
    }

    if (self->isColliderEnabled) {
        Collider_UpdateCylinder(&self->actor, &self->collider);
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    }
}

void EnDns_Draw(Actor* thisx, PlayState* play) {
    EnDns* self = (EnDns*)thisx;

    GraphicsContext_UnknownStub(play->state.gfxCtx);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, (OverrideLimbDrawOpa)FUN_00256974,
                      (PostLimbDrawOpa)FUN_0025697c, &self->actor, 0);
}

void FUN_0027f3f8(PlayState* play, Vec3f* pos, f32 scale); // func_80028990 (dust)

void EnDns_Burrow(EnDns* self, PlayState* play) {
    f32 depthInGround = self->yInitPos - self->actor.world.pos.y;
    Vec3f initPos;
    s32 i;

    if ((self->dustTimer & 3) == 0) {
        initPos.x = self->actor.world.pos.x;
        initPos.y = self->yInitPos;
        initPos.z = self->actor.world.pos.z;
        FUN_0027f3f8(play, &initPos, 20.0f);
    }

    self->actor.shape.rot.y += 0x2000;

    if (depthInGround > 400.0f) {
        if (self->dropCollectible) {
            initPos.x = self->actor.world.pos.x;
            initPos.y = self->yInitPos;
            initPos.z = self->actor.world.pos.z;

            for (i = 0; i < 3; i++) {
                Item_DropCollectible(play, &initPos, 3);
            }
        }
        Actor_Kill(&self->actor);
    }
}

// STATIC blocker: function-local static (guard 0x522330, object 0x5223BC, both inside the data chunk). ROM addresses the guard
// and the object relative to sAnimPtrs (0x522324): `ldr r5,=sAnimPtrs; add r7,r5,#0x98; ldr r0,[r5,#0xc]` -> sAnimPtrs, guard, the
// other TU statics and the object live in ONE .data section in that order. armcc here puts the guard FIRST in the TU .data and the
// local object LAST, so the shape (and size 168 vs 156) differs. Needs every TU static as real C data in ROM order (data chunk split).
#if 0
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);

typedef struct EnDnsAnimInfo {
    s32 animation;
    u8 mode;
    f32 morphFrames;
} EnDnsAnimInfo;

void EnDns_ChangeAnim(EnDns* self, u8 index) {
    static EnDnsAnimInfo sAnimInfo[3] = {
        { EnDns_sAnimPtrs[0], 0, 0.0f },
        { EnDns_sAnimPtrs[1], 2, 0.0f },
        { EnDns_sAnimPtrs[2], 2, 0.0f },
    };
    f32 frameCount = Animation_GetLastFrame(&self->skelAnime, sAnimInfo[index].animation);

    self->unk_202 = index;
    Animation_Change_00375c08(&self->skelAnime, sAnimInfo[index].animation, 1.0f, 0.0f, frameCount,
                              sAnimInfo[index].mode, sAnimInfo[index].morphFrames);
}
#endif
GLOBAL_ASM("asm/EnDns_ChangeAnim.s")

void FUN_0012e264(EnDns* self, PlayState* play); // EnDns_SetupNoSaleBurrow
void FUN_00161828(EnDns* self, PlayState* play); // EnDns_SetupSale

void EnDns_Talk(EnDns* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == 4) && Message_ShouldAdvance(play)) {
        s32 choice = FUN_00369f3c(play);

        if (choice != 0) {
            if (choice == 1) {
                Message_ContinueTextbox(play, 0x10A4);
                self->actionFunc = FUN_0012e264;
            }
        } else {
            switch (self->dnsItemEntry->canBuy(self)) {
                case DNS_CANBUY_RESULT_NEED_RUPEES:
                    Message_ContinueTextbox(play, 0x10A5);
                    self->actionFunc = FUN_0012e264;
                    break;

                case DNS_CANBUY_RESULT_CAPACITY_FULL:
                    Message_ContinueTextbox(play, 0x10A6);
                    self->actionFunc = FUN_0012e264;
                    break;

                case DNS_CANBUY_RESULT_CANT_GET_NOW:
                    Message_ContinueTextbox(play, 0x10DE);
                    self->actionFunc = FUN_0012e264;
                    break;

                case DNS_CANBUY_RESULT_SUCCESS_NEW_ITEM:
                case DNS_CANBUY_RESULT_SUCCESS:
                    Message_ContinueTextbox(play, 0x10A7);
                    self->actionFunc = FUN_00161828;
                    break;
            }
        }
    }
}


}
