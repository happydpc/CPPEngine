//
// Generated by Microsoft (R) HLSL Shader Compiler 9.29.952.3111
//
//
//   fxc /T ps_4_0 /Fc constTexCoord.asm /Fo constTexCoord.o constTexCoord
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim Slot Elements
// ------------------------------ ---------- ------- ----------- ---- --------
// TextureSampler                    sampler      NA          NA    0        1
// Texture0                          texture  float4          2d    0        1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// no Input
//
// Output signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// SV_Target                0   xyzw        0   TARGET  float   xyzw
//
ps_4_0
dcl_sampler s0, mode_default
dcl_resource_texture2d (float,float,float,float) t0
dcl_output o0.xyzw
dcl_temps 1
sample r0.xyzw, l(0.500000, 0.500000, 0.000000, 0.000000), t0.xyzw, s0
mov o0.xyzw, r0.xxxx
ret 
// Approximately 3 instruction slots used
