	EXPORT EnTr_sTables

; En_Tr s32 anim-index tables inside z_En_Tr.data (@ 0x0052FB08), two entries (Koume, Kotake) each:
; [0..1] unused, [2..3] D_80B24368 (turn look-over), [4..5] D_80B24370 (looking-over),
; [6..7] D_80B24378 (fly), [8..9] D_80B24380 (cast spellcast)
EnTr_sTables EQU 0x0052FB08

	END
