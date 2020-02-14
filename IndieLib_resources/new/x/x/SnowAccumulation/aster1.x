xof 0303txt 0032
template XSkinMeshHeader {
 <3cf169ce-ff7c-44ab-93c0-f78f62d172e2>
 WORD nMaxSkinWeightsPerVertex;
 WORD nMaxSkinWeightsPerFace;
 WORD nBones;
}

template VertexDuplicationIndices {
 <b8d65549-d7c9-4995-89cf-53a9a8b031e3>
 DWORD nIndices;
 DWORD nOriginalVertices;
 array DWORD indices[nIndices];
}

template SkinWeights {
 <6f0d123b-bad2-4167-a0d0-80224f25fabb>
 STRING transformNodeName;
 DWORD nWeights;
 array DWORD vertexIndices[nWeights];
 array FLOAT weights[nWeights];
 Matrix4x4 matrixOffset;
}


Frame SCENE_ROOT {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,-1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Frame groundPlane_transform {
  

  FrameTransformMatrix {
   12.000000,0.000000,0.000000,0.000000,0.000000,0.000000,12.000000,0.000000,0.000000,-1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
  }
 }

 Frame persp {
  

  FrameTransformMatrix {
   0.815128,0.000000,-0.579281,0.000000,-0.093997,0.986747,-0.132266,0.000000,0.571604,0.162264,0.804325,0.000000,2.104742,0.597484,2.961659,1.000000;;
  }
 }

 Frame top {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-1.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,100.000000,0.000000,1.000000;;
  }
 }

 Frame front {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,100.000000,1.000000;;
  }
 }

 Frame side {
  

  FrameTransformMatrix {
   0.000000,0.000000,-1.000000,0.000000,0.000000,1.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,100.000000,0.000000,0.000000,1.000000;;
  }
 }

 Frame pCube1 {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
  }

  Mesh pCubeShape1 {
   23;
   -0.652374;-0.123337;0.394877;,
   0.000416;-0.435163;0.384343;,
   0.317830;-0.418071;0.333369;,
   -0.859417;0.260545;0.534103;,
   0.098472;0.092601;0.505021;,
   0.526061;0.324228;0.627715;,
   -0.573408;0.453451;-0.737067;,
   -0.068530;0.656634;-0.723058;,
   0.495434;0.501328;-0.418443;,
   -0.362280;-0.600467;-0.275925;,
   -0.042661;-0.541332;-0.470303;,
   0.474026;-0.501479;-0.244165;,
   -0.728095;0.702681;0.561647;,
   0.178370;0.429075;0.595343;,
   -0.024140;0.909902;-0.266099;,
   -0.493510;0.789925;-0.646746;,
   0.000416;-0.435163;0.384343;,
   -0.652374;-0.123337;0.394877;,
   0.317830;-0.418071;0.333369;,
   0.474026;-0.501479;-0.244165;,
   0.495434;0.501328;-0.418443;,
   -0.362280;-0.600467;-0.275925;,
   -0.573408;0.453451;-0.737067;;
   14;
   4;3,4,1,0;,
   4;4,5,2,1;,
   4;15,14,13,12;,
   4;7,8,5,4;,
   4;9,10,7,6;,
   4;10,11,8,7;,
   4;17,16,10,9;,
   4;16,18,11,10;,
   4;5,20,19,2;,
   4;22,3,0,21;,
   4;12,13,4,3;,
   4;13,14,7,4;,
   4;14,15,6,7;,
   4;15,12,3,6;;

   MeshNormals {
    16;
    -0.770893;-0.179757;0.611074;,
    -0.669928;0.113712;-0.733666;,
    -0.560971;-0.728006;-0.394104;,
    -0.527068;-0.698459;0.484102;,
    -0.523460;0.607260;0.597682;,
    -0.384060;0.875620;-0.292894;,
    -0.051965;-0.726654;0.685035;,
    0.064415;-0.866189;-0.495548;,
    0.222046;0.051203;0.973691;,
    0.398654;0.370529;-0.838918;,
    0.487620;0.292758;0.822508;,
    0.491844;-0.679520;0.544373;,
    0.531025;0.782933;-0.324080;,
    0.591360;0.481322;0.647011;,
    0.701953;-0.629092;-0.333925;,
    0.832951;0.428904;-0.349621;;
    14;
    4;0,8,6,3;,
    4;8,10,11,6;,
    4;5,12,13,4;,
    4;9,15,10,8;,
    4;2,7,9,1;,
    4;7,14,15,9;,
    4;3,6,7,2;,
    4;6,11,14,7;,
    4;10,15,14,11;,
    4;1,0,3,2;,
    4;4,13,8,0;,
    4;13,12,9,8;,
    4;12,5,1,9;,
    4;5,4,0,1;;
   }

   MeshTextureCoords {
    23;
    0.250000;1.000000;,
    0.500000;1.000000;,
    0.750000;1.000000;,
    0.182995;0.808883;,
    0.700000;0.750000;,
    0.850000;0.700000;,
    0.000000;0.500000;,
    0.500000;0.300000;,
    1.000000;0.500000;,
    0.000000;0.250000;,
    0.450000;0.150000;,
    1.000000;0.300000;,
    0.243748;0.716878;,
    0.554025;0.695309;,
    0.468322;0.395957;,
    0.137437;0.508122;,
    0.500000;0.000000;,
    0.000000;0.000000;,
    1.000000;0.000000;,
    1.000000;1.000000;,
    1.000000;0.550000;,
    0.000000;1.000000;,
    0.000000;0.550000;;
   }

   MeshMaterialList {
    1;
    14;
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0;

    Material initialShadingGroup {
     0.400000;0.400000;0.400000;1.000000;;
     0.000000;
     0.400000;0.400000;0.400000;;
     0.000000;0.000000;0.000000;;
    }
   }

   VertexDuplicationIndices {
    23;
    16;
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10,
    11,
    12,
    13,
    14,
    15,
    1,
    0,
    2,
    11,
    8,
    9,
    6;
   }
  }
 }
}

AnimationSet {
 
}