	EXPORT EnBw_sSlugGroup
	EXPORT EnBw_sCylinderInit1
	EXPORT EnBw_sCylinderInit2
	EXPORT EnBw_sDamageTable
	EXPORT EnBw_sIceOffsets

; Actor data chunk (z_En_Bw.data @ 0x00521514): slug group counter, the two cylinder inits and the damage table.
; C reads them through these EQUs.
EnBw_sSlugGroup EQU 0x00521514
EnBw_sCylinderInit1 EQU 0x00521518
EnBw_sCylinderInit2 EQU 0x00521550
EnBw_sDamageTable EQU 0x00521588
EnBw_sIceOffsets EQU 0x005215A8

	END
