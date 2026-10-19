	EXPORT EnRiverSound_sState
	EXPORT EnRiverSound_sGanonVolTable
	EXPORT EnRiverSound_sSfxHandles
	EXPORT EnRiverSound_sVolFade
	EXPORT EnRiverSound_sSoundEffects
	EXPORT EnRiverSound_sSfxFreqs

; River sound global state (3DS 0x0054AC48, shared by the Destroy/Sfx helpers; not an image symbol)
EnRiverSound_sState EQU 0x0054AC48
; per-level Ganon tower BGM volume table (3DS 0x0054AD3D)
EnRiverSound_sGanonVolTable EQU 0x0054AD3D
; per-slot sfx handle table used by FUN_003ff980 (3DS 0x005AF450)
EnRiverSound_sSfxHandles EQU 0x005AF450
; volume fade state for the 0x0100015D river sfx (3DS 0x005B12C8)
EnRiverSound_sVolFade EQU 0x005B12C8
; Draw: per-params sfx table (3DS u32 ids, 0x0052E344) and conveyor frequency table {0.7f, 1.0f, 1.4f} (0x0052E3A0)
EnRiverSound_sSoundEffects EQU 0x0052E344
EnRiverSound_sSfxFreqs EQU 0x0052E3A0

	END
