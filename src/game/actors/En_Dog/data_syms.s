	EXPORT EnDog_sDrawColors
	EXPORT EnDog_sCylinderInit
	EXPORT EnDog_sColChkInfoInit
	EXPORT EnDog_sAnimationInfo

; Draw colour table (Color_RGBAf[2], 0x20 B) in the rodata chunk at 0x004D943C; C reads it through this EQU
EnDog_sDrawColors EQU 0x004D943C

EnDog_sCylinderInit EQU 0x005229DC
EnDog_sColChkInfoInit EQU 0x00522A14
EnDog_sAnimationInfo EQU 0x00522A20

	END
