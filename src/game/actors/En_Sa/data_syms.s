	EXPORT EnSa_sFaceRemap
	EXPORT EnSa_sRgbaConst
	EXPORT EnSa_sCylinderInit
	EXPORT EnSa_sColChkInfoInit
EnSa_sFaceRemap EQU 0x004D9C20   ; data_4D9C20: u8[5] face-anim index remap (EnSa_Draw)
EnSa_sRgbaConst EQU 0x004D9C28   ; data_4D9C28: f32[4] {1,1,1,0} (EnSa_Draw sp+8 buffer)
EnSa_sCylinderInit EQU 0x0052E740   ; data_52E740: ColliderCylinderInit
EnSa_sColChkInfoInit EQU 0x0052E778   ; data_52E778: CollisionCheckInfoInit2

	END
