	AREA |.text|, CODE, READONLY
	EXPORT EnMThunder_sCylinderInit
	EXPORT EnMThunder_sDmgTable
	EXPORT EnMThunder_sSfxIds
	EXPORT EnMThunder_sSpinChargeScale
; Actor data in the dump (GLOBAL_ASM data/z_En_M_Thunder.data.s): no scatter symbols, so absolute EQUs.
;   sCylinderInit 0x0052C09C (0x2C), spin/jump dmg flags {u32[3], u32[3]} 0x0052C0D4, sfx ids u32[4] 0x0052C0EC,
;   spin charge scale f32[8] 0x0052C0FC
EnMThunder_sCylinderInit EQU 0x0052C09C
EnMThunder_sDmgTable EQU 0x0052C0D4
EnMThunder_sSfxIds EQU 0x0052C0EC
EnMThunder_sSpinChargeScale EQU 0x0052C0FC

	END
