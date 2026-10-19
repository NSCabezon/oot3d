#ifndef _Z_ITEM_INBOX_H_
#define _Z_ITEM_INBOX_H_

#include "global.hpp"

struct ItemInbox;

typedef void (*ItemInboxActionFunc)(struct ItemInbox* self, PlayState* play);

typedef struct ItemInbox {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ItemInboxActionFunc actionFunc;
    /* 0x01A8 */ SkeletonAnimationModel* model;
} ItemInbox; // size = 0x1AC

extern ActorInit Item_Inbox_InitVars;

#endif
