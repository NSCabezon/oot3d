; Project-wide absolute symbol aliases (EQU), pulled in ONCE by src/game/shared_syms.cpp.
; Only symbols that are NOT owned by one actor live here (runtime/code, compiler-generated names,
; engine globals still inside raw data dumps). Actor-owned data aliases stay in the actor dirs.
; Each name must be defined exactly once project-wide (a 2nd definition -> L6200E).

; ---- code (ROM functions still asm, referenced under their C/C++/AEABI names)
	EXPORT __aeabi_idivmod
__aeabi_idivmod EQU 0x00368D94, CODE32          ; FUN_00368d94: lets C `/` and `%` bind to the ROM routine
	EXPORT _ZN12ModelManagerC1Ev
_ZN12ModelManagerC1Ev EQU 0x0036788C, CODE32    ; GetModelManager() static ctor (FUN_0036788c); CODE32 required (L6805E)
	EXPORT _ZN12ModelManagerD1Ev
_ZN12ModelManagerD1Ev EQU 0x0048B1F0, CODE32    ; its dtor (V00)
	EXPORT _ZN15BlureInitVertexC1Ev
_ZN15BlureInitVertexC1Ev EQU 0x003FD780         ; blure vertex ctor (Bg_Haka_Sgami, En_Ik)
	EXPORT __dso_handle
__dso_handle EQU 0x00100000                     ; shown as __ctr_start

; ---- VFE placeholders (--no_rtti_data + --vfemode=force key VFE on _ZTI<class>; see CLAUDE.md "VFE fix")
; absolute, no image bytes; values only need to be unique
	EXPORT _ZTI17ItemEtceteraModel
_ZTI17ItemEtceteraModel EQU 0x7FFF0010
	EXPORT _ZTI17ItemOcarinaModel
_ZTI17ItemOcarinaModel EQU 0x7FFF0011
	EXPORT _ZTI15EnDekunutsModel
_ZTI15EnDekunutsModel EQU 0x7FFF0020
	EXPORT _ZTI12EffDustFreer
_ZTI12EffDustFreer EQU 0x7FFF0030
	EXPORT _ZTI16EnGanonMantFreer
_ZTI16EnGanonMantFreer EQU 0x7FFF0031
	EXPORT _ZTI11EnPartModel
_ZTI11EnPartModel EQU 0x7FFF0040
	EXPORT _ZTI21BgJyaBigmirrorManager
_ZTI21BgJyaBigmirrorManager EQU 0x7FFF0041
	EXPORT _ZTI12EnLightModel
_ZTI12EnLightModel EQU 0x7FFF0042
	EXPORT _ZTI15OceffStormModel
_ZTI15OceffStormModel EQU 0x7FFF0050
	EXPORT _ZTI15OceffStormFreer
_ZTI15OceffStormFreer EQU 0x7FFF0051

; ---- engine globals inside raw data dumps / .bss (no C definition yet)
	EXPORT gUnk_0050C058
gUnk_0050C058 EQU 0x0050C058                    ; data_4F9000: skin draw counter (En_Viewer)
	EXPORT gDbgCamEnabled
gDbgCamEnabled EQU 0x00516EAC                   ; z_camera.data (N64 gDbgCamEnabled; En_Holl)
	EXPORT gWeatherMode
gWeatherMode EQU 0x00531EB4                     ; data_531EB4 (En_Okarina_Effect, En_Weather_Tag)
	EXPORT gUnk_00549600
gUnk_00549600 EQU 0x00549600                    ; data_5495F4: letterbox size passed to ShrinkWindow_SetVal (En_In, En_Zl1)
	EXPORT gUnk_0054ABD4
gUnk_0054ABD4 EQU 0x0054ABD4                    ; data_54A9C4 (En_Holl)
	EXPORT gUnk_0054C8C0
gUnk_0054C8C0 EQU 0x0054C8C0                    ; data_54AC28: pointer variable (En_Fr ocarina notes)
	EXPORT gUnk_0054C8CC
gUnk_0054C8CC EQU 0x0054C8CC                    ; data_54AC28: read by ~46 fns (Bg_Mjin, Bg_Spot07_Taki, Bg_Toki_Hikari, Demo_Shd, En_Viewer, En_Vm)
	EXPORT gUnk_0055A1A8
gUnk_0055A1A8 EQU 0x0055A1A8                    ; data_54AC28: BgJyaBigmirrorManager* (Bg_Jya_Bigmirror)
	EXPORT gUnk_0055A1F8
gUnk_0055A1F8 EQU 0x0055A1F8                    ; data_54AC28: model freer object pointer (Eff_Dust, En_Ganon_Mant)
	EXPORT gSaveContext_004
gSaveContext_004 EQU 0x0058795C                 ; gSaveContext+0x004 (Bg_Toki_Hikari)
	EXPORT gSaveContext_E00
gSaveContext_E00 EQU 0x00588758                 ; gSaveContext+0xE00, armcc shared flag base (En_Heishi4)
	EXPORT gSaveContext_F00
gSaveContext_F00 EQU 0x00588858                 ; gSaveContext+0xF00, armcc shared flag base (En_Fr)
	EXPORT gUnk_005C1878
gUnk_005C1878 EQU 0x005C1878                    ; GetModelManager() object + 0x32C0 (End_Title, En_fHG)

	EXPORT gCustomLensFlareOn
gCustomLensFlareOn EQU 0x00531EBA               ; u8 (En_Fhg_Fire LightningBurst; BossGanon, Actor1D8)
	EXPORT gLensFlareScale
gLensFlareScale EQU 0x00531EC2                  ; s16
	EXPORT gLensFlareGlareStrength
gLensFlareGlareStrength EQU 0x00531EC4          ; s16
	EXPORT gLensFlareColorIntensity
gLensFlareColorIntensity EQU 0x00531EEC         ; f32
	EXPORT gCustomLensFlarePos
gCustomLensFlarePos EQU 0x005A2AD4              ; Vec3f

	EXPORT gTimeSpeed
gTimeSpeed EQU 0x00531EBE                       ; u16 engine time-speed global (Oceff_Spot)
	EXPORT _ZTI14ArrowFireModel
_ZTI14ArrowFireModel EQU 0x7FFF0061                ; VFE key (Arrow_Fire)
	EXPORT _ZTI13ArrowIceModel
_ZTI13ArrowIceModel EQU 0x7FFF0070                 ; VFE key (Arrow_Ice)
	EXPORT _ZTI15EnMThunderFreer
_ZTI15EnMThunderFreer EQU 0x7FFF0060                ; VFE key (En_M_Thunder)
	EXPORT FLOAT_0054acb0
FLOAT_0054acb0 EQU 0x0054ACB0                   ; f32 (sword-charge sfx, inside data_54AC28; FUN_00314988)
	EXPORT _ZTI15ArrowLightModel
_ZTI15ArrowLightModel EQU 0x7FFF0062               ; VFE key (Arrow_Light)
	EXPORT _ZTI14MagicFireModel
_ZTI14MagicFireModel EQU 0x7FFF0080               ; VFE key (Magic_Fire)
	EXPORT _ZTI13EnExItemModel
_ZTI13EnExItemModel EQU 0x7FFF0090                 ; VFE key (En_Ex_Item)
	EXPORT _ZTI13EnWood02Model
_ZTI13EnWood02Model EQU 0x7FFF00B0                 ; VFE key (En_Wood02)
	EXPORT gUnk_0055A1C0
gUnk_0055A1C0 EQU 0x0055A1C0                    ; data_54AC28: model freer object pointer (Door_Shutter, Demo_Effect)
	EXPORT _ZTI16ObjSyokudaiModel
_ZTI16ObjSyokudaiModel EQU 0x7FFF00C0                 ; VFE key (Obj_Syokudai)
	EXPORT _ZTI10EfcErupcFx
_ZTI10EfcErupcFx EQU 0x7FFF00D0                 ; VFE key (Efc_Erupc)
	EXPORT _ZTI14EnIceHonoModel
_ZTI14EnIceHonoModel EQU 0x7FFF00E0                 ; VFE key (En_Ice_Hono)
	EXPORT _ZTI15DemoEffectModel
_ZTI15DemoEffectModel EQU 0x7FFF0860                 ; VFE key (Demo_Effect)
	EXPORT _ZTI8EnMagObj
_ZTI8EnMagObj EQU 0x7FFF00F0                 ; VFE key (En_Mag)
	EXPORT _ZTI11EnGomaModel
_ZTI11EnGomaModel EQU 0x7FFF0890                 ; VFE key (En_Goma)
	EXPORT _ZTI14MagicDarkModel
_ZTI14MagicDarkModel EQU 0x7FFF0910                 ; VFE key (Magic_Dark)
	EXPORT _ZTI15EnHintnutsModel
_ZTI15EnHintnutsModel EQU 0x7FFF0102                 ; VFE key (En_Hintnuts)
	EXPORT _ZTI21BgSpot00HanebasiModel
_ZTI21BgSpot00HanebasiModel EQU 0x7FFF00A0                 ; VFE key (Bg_Spot00_Hanebasi)
	END
