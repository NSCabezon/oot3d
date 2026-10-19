	EXPORT EnFw_sAnimationInfo
	EXPORT EnFw_sJntSphInit
	EXPORT EnFw_sColChkInfoInit

; FlareDancerCore animation table (struct_80034EC0_Entry[3]) in the TU data chunk; C reads it through this EQU
EnFw_sAnimationInfo EQU 0x00523AC8

; Collider/CollisionCheck init tables in the TU data chunk (ColliderJntSphInit, CollisionCheckInfoInit2)
EnFw_sJntSphInit EQU 0x00523AAC
EnFw_sColChkInfoInit EQU 0x00523ABC

	END
