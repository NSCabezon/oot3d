	EXPORT EnSkj_sStumpVars
	EXPORT EnSkj_sTalkOfferAccepted
	EXPORT EnSkj_sSmallStumpSkullKid
	EXPORT EnSkj_sActionFuncs
	EXPORT EnSkj_sSoundState
	EXPORT EnSkj_sSoundStateFlag
	EXPORT EnSkj_sOcarinaSkullKids
	EXPORT EnSkj_sOcarinaRewards
	EXPORT EnSkj_sAnimationInfo
	EXPORT EnSkj_sSeqVars
	EXPORT EnSkj_sColorWhite
	EXPORT EnSkj_sMsgVars
	EXPORT EnSkj_sInitChain
	EXPORT EnSkj_sColliderInit
	EXPORT EnSkj_sDamageTable
	EXPORT EnSkj_sSndBase
	EXPORT EnSkj_sSndTbl
	EXPORT EnSkj_sSndBufA
	EXPORT EnSkj_sSndBufB
	EXPORT EnSkj_sSndBufC
	EXPORT EnSkj_sSndBufD

; Stump/ocarina-minigame globals (3DS 0x005093E4, words at +0x3C/+0x40/+0x44)
EnSkj_sStumpVars EQU 0x005093E4
; D_80B01EA0: talk-offer result (s32)
EnSkj_sTalkOfferAccepted EQU 0x0052ECA4
; sSmallStumpSkullKid {u8 unk_0; EnSkj* skullkid} (3DS 0x0052ECA8)
EnSkj_sSmallStumpSkullKid EQU 0x0052ECA8
; action function table, indexed by action (3DS 0x0052ED98)
EnSkj_sActionFuncs EQU 0x0052ED98
; shared sound state (same block as EnRiverSound_sState, 3DS 0x0054AC48; +0x70 seq handle, +0x94)
EnSkj_sSoundState EQU 0x0054AC48
; byte +0x55 of the same block (3DS 0x0054AC9D), read as a return pointer
EnSkj_sSoundStateFlag EQU 0x0054AC9D

; ocarina minigame skull kid pair {u8 unk_0; EnSkj* skullkid} x2 (3DS 0x0059B634, stride 8)
EnSkj_sOcarinaSkullKids EQU 0x0059B634
; reward item table, indexed by ocarinaGameRoundNum (3DS 0x0052ED10)
EnSkj_sOcarinaRewards EQU 0x0052ED10
; animation table {void* animation; u8 mode; f32 morphFrames} x N (3DS 0x0052ED20, stride 12)
EnSkj_sAnimationInfo EQU 0x0052ED20
; RGBA {1,1,1,1} float color copied by EnSkj_Draw (3DS 0x004D9C38, rodata chunk 4D99DC)
EnSkj_sColorWhite EQU 0x004D9C38
; sound/sequence vars written by FUN_0032c570 (3DS 0x0050AF34; +0x34, +0x5C, +0x74)
EnSkj_sSeqVars EQU 0x0050AF34
; message-system vars (3DS 0x0053496C, u16 at +2 cleared by FUN_00334fa0)
EnSkj_sMsgVars EQU 0x0053496C
; sequence-player vars used by FUN_0033704c
EnSkj_sSndBase EQU 0x0054B5F2
EnSkj_sSndTbl EQU 0x0054BD72
EnSkj_sSndBufA EQU 0x005B12F8
EnSkj_sSndBufB EQU 0x005B1314
EnSkj_sSndBufC EQU 0x005B1330
EnSkj_sSndBufD EQU 0x005B134C
; InitChain (3DS 0x0052ECB0), cylinder collider init (0x0052ECB8), damage table (0x0052ECF0)
EnSkj_sInitChain EQU 0x0052ECB0
EnSkj_sColliderInit EQU 0x0052ECB8
EnSkj_sDamageTable EQU 0x0052ECF0

	END
