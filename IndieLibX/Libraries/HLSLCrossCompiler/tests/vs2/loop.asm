//
// Generated by Microsoft (R) HLSL Shader Compiler 9.30.9200.20499
//
///
// Parameters:
//
//   float4x4 g_mW2C;
//   float4x4 vJointMatrix[48];
//
//
// Registers:
//
//   Name         Reg   Size
//   ------------ ----- ----
//   vJointMatrix c0     192
//   g_mW2C       c192     4
//

    vs_2_0
    def c200, 4, 0, 1, 0
    def c196, 1, 0, 0, 0
    def c197, 0, 1, 0, 0
    def c198, 0, 0, 1, 0
    def c199, 0, 0, 0, 1
    defi i0, 4, 0, 1, 0
    dcl_position v0
    dcl_normal v1
    dcl_texcoord v2
    dcl_blendindices v3
    dcl_blendweight v4
    mov r0, c196.w
    mov r1.xyz, c196.w
    loop aL, i0
      dp4 r1.w, v3, c196[aL]
      dp4 r2.x, v4, c196[aL]
      frc r2.y, r1.w
      mov r2.y, -r2.y
      add r1.w, r1.w, r2.y
      mul r1.w, r1.w, c200.x
      mova a0.x, r1.w
      mov r3, c0[a0.x]
      mov r4, c1[a0.x]
      mov r5, c2[a0.x]
      mov r6, c3[a0.x]
      dp4 r7.x, v0, r3
      dp4 r7.y, v0, r4
      dp4 r7.z, v0, r5
      dp4 r7.w, v0, r6
      mul r6, r2.x, r7
      add r0, r0, r6
      dp3 r3.x, v1, r3
      dp3 r3.y, v1, r4
      dp3 r3.z, v1, r5
      mul r2.xyz, r2.x, r3
      add r1.xyz, r1, r2
    endloop
    dp4 r2.x, r0, c192
    dp4 r2.y, r0, c193
    dp4 r2.z, r0, c194
    dp4 r2.w, r0, c195
    mov r0.xy, v2
    mov r0.y, -r0.y
    add r0.y, r0.y, c196.x
    mov oPos, r2
    mov oT0.xyz, r1
    mov oT2.x, r0.x
    mov oT2.y, r0.y

// approximately 40 instruction slots used
