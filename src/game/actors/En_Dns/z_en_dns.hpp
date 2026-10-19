#ifndef _Z_EN_DNS_H_
#define _Z_EN_DNS_H_

#include "global.hpp"

struct EnDns;

typedef enum EnDnsCanBuyResult {
    /*  0 */ DNS_CANBUY_RESULT_NEED_RUPEES,
    /*  1 */ DNS_CANBUY_RESULT_CAPACITY_FULL,
    /*  2 */ DNS_CANBUY_RESULT_SUCCESS_NEW_ITEM,
    /*  3 */ DNS_CANBUY_RESULT_CANT_GET_NOW,
    /*  4 */ DNS_CANBUY_RESULT_SUCCESS
} EnDnsCanBuyResult;

typedef enum EnDnsAnimation {
    /*  0 */ DNS_ANIM_IDLE,
    /*  1 */ DNS_ANIM_BURROW,
    /*  2 */ DNS_ANIM_IDLE_TRANSITION
} EnDnsAnimation;

typedef void (*EnDnsActionFunc)(struct EnDns*, PlayState*);
typedef u32 (*EnDnsCanBuyFunc)(struct EnDns*);
typedef void (*EnDnsPaymentFunc)(struct EnDns*);

typedef struct DnsItemEntry {
    /* 0x00 */ s16 itemPrice;
    /* 0x02 */ u16 itemAmount;
    /* 0x04 */ s32 getItemId;
    /* 0x08 */ EnDnsCanBuyFunc canBuy;
    /* 0x0C */ EnDnsPaymentFunc payment;
} DnsItemEntry; // size = 0x10

// 3DS layout. Offsets confirmed from ROM: collider 0x1A8, dnsItemEntry 0x208, skelAnime 0x210 (0x84),
// jointTable 0x294, morphTable 0x5A0. The rest is not mapped yet.
typedef struct EnDns {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnDnsActionFunc actionFunc;
    /* 0x01A8 */ ColliderCylinder collider;
    /* 0x0200 */ s16 dustTimer;
    /* 0x0202 */ u8 unk_202;
    /* 0x0203 */ u8 isColliderEnabled;
    /* 0x0204 */ u8 standOnGround;
    /* 0x0205 */ u8 dropCollectible;
    /* 0x0206 */ char unk_206[0x2];
    /* 0x0208 */ DnsItemEntry* dnsItemEntry;
    /* 0x020C */ f32 yInitPos;
    /* 0x0210 */ SkelAnime skelAnime;
    /* 0x0294 */ char jointTable[0x30C];
    /* 0x05A0 */ char morphTable[0x30C];
} EnDns; // size = 0x08AC


extern ActorInit En_Dns_InitVars;

#endif
