//
// Generated by Microsoft (R) HLSL Shader Compiler 9.29.952.3111
//
//
//   fxc /T vs_5_0 /Fo mad_imm.o /Fc mad_imm.asm mad_imm
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// POSITION                 0   xyzw        0     NONE  float   xyzw
// TEXCOORD                 0   xyzw        1     NONE  float   xyzw
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// SV_Position              0   xyzw        0      POS  float   xyzw
//
vs_5_0
dcl_globalFlags refactoringAllowed
dcl_input v0.xyzw
dcl_input v1.xyzw
dcl_output_siv o0.xyzw, position
mad o0.xyzw, v1.xyzw, l(0.100000, 0.200000, 0.300000, 0.400000), v0.xyzw
ret 
// Approximately 2 instruction slots used
