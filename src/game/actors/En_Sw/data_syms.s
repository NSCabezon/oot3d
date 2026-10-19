	EXPORT EnSw_sData
	EXPORT EnSw_sFogColor
	EXPORT EnSw_sAnimInfo

; Actor data block at 0x0052F258 (word [1] is the anim index passed to Animation_GetLastFrame).
EnSw_sData EQU 0x0052F258

; Fog colour (Color_RGBAf, 16 B) in rodata, copied to the stack in EnSw_Draw.
EnSw_sFogColor EQU 0x004D9D10

; Anim table (3DS AnimationInfo entries) in the actor data block.
EnSw_sAnimInfo EQU 0x0052F280

	END
