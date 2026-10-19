; Handwritten aliases to verified existing callback entry and vtable; no emitted bytes.
	EXPORT EnTorch2_PostLimbDrawEntry
EnTorch2_PostLimbDrawEntry EQU 0x004C1C8C, CODE32
	EXPORT EnTorch2_sBaseLimbVtable
EnTorch2_sBaseLimbVtable EQU 0x004EC030
	EXPORT EnTorch2_sDerivedLimbVtable
EnTorch2_sDerivedLimbVtable EQU 0x004EC008
	EXPORT EnTorch2_sDamageTable
EnTorch2_sDamageTable EQU 0x0052F9C0
; Existing sword jump adjustment float, read by EnTorch2_Update.
	EXPORT EnTorch2_sSwordYOffset
EnTorch2_sSwordYOffset EQU 0x0052F9B8
; Existing SaveContext + 0x1500 view; magic state is u16 at view + 0x80.
	EXPORT EnTorch2_sSaveRuntime
EnTorch2_sSaveRuntime EQU 0x00588E58
; Native C++ callback table header: two zero words, then Override/Post slots.
; Constructor's verified relocation adds 8 to reach address point 0x004EC008.
	EXPORT _ZTV20EnTorch2LimbCallback
_ZTV20EnTorch2LimbCallback EQU 0x004EC000
	END
