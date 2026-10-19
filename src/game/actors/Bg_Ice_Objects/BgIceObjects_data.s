	EXPORT BgIceObjects_sInitChain
	EXPORT BgIceObjects_sXStarts
	EXPORT BgIceObjects_sZStarts
	EXPORT BgIceObjects_sZStops
	EXPORT BgIceObjects_sXStops
	EXPORT BgIceObjects_sBlock
	EXPORT BgIceObjects_sColors
	EXPORT BgIceObjects_sZeroVec
; Tables live in the raw data dump of Bg_Hidan_Syoku (data/z_Bg_Hidan_Syoku.data.s covers 0x511F80..0x511FE8).
BgIceObjects_sInitChain EQU 0x00511F8C
BgIceObjects_sXStarts EQU 0x00511F94
BgIceObjects_sZStarts EQU 0x00511FA2
BgIceObjects_sZStops EQU 0x00511FB0
BgIceObjects_sXStops EQU 0x00511FCC
BgIceObjects_sBlock EQU 0x00511F84
BgIceObjects_sColors EQU 0x00511F88
BgIceObjects_sZeroVec EQU 0x005699E0

	END
