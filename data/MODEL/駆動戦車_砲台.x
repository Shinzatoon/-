xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 32;
 -4.93220;21.65603;-9.10673;,
 5.14057;21.65603;-9.10673;,
 5.54540;17.83749;-9.10673;,
 -5.33438;17.83749;-9.10673;,
 10.13761;17.83749;6.83824;,
 10.13761;17.83749;-4.85718;,
 10.13761;21.65603;-4.48255;,
 10.13761;21.65603;6.46317;,
 -9.86239;21.65603;6.38478;,
 -9.86239;21.65603;-4.46808;,
 -9.86239;17.83749;-4.84648;,
 -9.86239;17.83749;6.75923;,
 -5.64422;17.83749;10.89327;,
 5.99586;17.83749;10.89327;,
 5.61276;21.65603;10.89327;,
 -5.26215;21.65603;10.89327;,
 -5.26215;21.65603;10.89327;,
 5.61276;21.65603;10.89327;,
 -9.86239;21.65603;6.38478;,
 10.13761;21.65603;6.46317;,
 10.13761;21.65603;-4.48255;,
 -9.86239;21.65603;-4.46808;,
 -4.93220;21.65603;-9.10673;,
 5.14057;21.65603;-9.10673;,
 5.99586;17.83749;10.89327;,
 -5.64422;17.83749;10.89327;,
 10.13761;17.83749;6.83824;,
 -9.86239;17.83749;6.75923;,
 10.13761;17.83749;-4.85718;,
 -9.86239;17.83749;-4.84648;,
 -5.33438;17.83749;-9.10673;,
 5.54540;17.83749;-9.10673;;
 
 20;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;9,0,3,10;,
 4;6,5,2,1;,
 4;14,13,4,7;,
 4;8,11,12,15;,
 3;16,17,18;,
 3;17,19,18;,
 3;19,20,18;,
 3;18,20,21;,
 3;21,20,22;,
 3;20,23,22;,
 3;24,25,26;,
 3;25,27,26;,
 3;26,27,28;,
 3;27,29,28;,
 3;29,30,28;,
 3;30,31,28;;
 
 MeshMaterialList {
  1;
  20;
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
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.380000;0.380000;0.380000;;
  }
 }
 MeshNormals {
  10;
  0.363977;0.038588;-0.930608;,
  0.915816;0.039211;-0.399678;,
  -0.917460;0.039231;-0.395888;,
  -0.367815;0.038739;-0.929092;,
  0.377057;0.037828;0.925417;,
  0.921377;0.037994;0.386810;,
  -0.921477;0.037908;0.386580;,
  -0.377294;0.037751;0.925324;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;0.000000;;
  20;
  4;3,0,0,3;,
  4;5,1,1,5;,
  4;6,2,2,6;,
  4;7,4,4,7;,
  4;2,3,3,2;,
  4;1,1,0,0;,
  4;4,4,5,5;,
  4;6,6,7,7;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;;
 }
 MeshTextureCoords {
  32;
  0.246510;0.000000;,
  0.750150;0.000000;,
  0.770390;1.000000;,
  0.226400;1.000000;,
  0.797250;1.000000;,
  0.212480;1.000000;,
  0.231210;0.000000;,
  0.778500;0.000000;,
  0.225430;0.000000;,
  0.768070;0.000000;,
  0.786990;1.000000;,
  0.206700;1.000000;,
  0.789090;1.000000;,
  0.207090;1.000000;,
  0.226240;0.000000;,
  0.769990;0.000000;,
  0.230010;0.000000;,
  0.773760;0.000000;,
  0.000000;0.225430;,
  1.000000;0.221510;,
  1.000000;0.768790;,
  0.000000;0.768070;,
  0.246510;1.000000;,
  0.750150;1.000000;,
  0.792910;1.000000;,
  0.210910;1.000000;,
  1.000000;0.797250;,
  0.000000;0.793300;,
  1.000000;0.212480;,
  0.000000;0.213010;,
  0.226400;0.000000;,
  0.770390;0.000000;;
 }
 MeshVertexColors {
  32;
  0;1.000000;1.000000;1.000000;1.000000;,
  1;1.000000;1.000000;1.000000;1.000000;,
  2;1.000000;1.000000;1.000000;1.000000;,
  3;1.000000;1.000000;1.000000;1.000000;,
  4;1.000000;1.000000;1.000000;1.000000;,
  5;1.000000;1.000000;1.000000;1.000000;,
  6;1.000000;1.000000;1.000000;1.000000;,
  7;1.000000;1.000000;1.000000;1.000000;,
  8;1.000000;1.000000;1.000000;1.000000;,
  9;1.000000;1.000000;1.000000;1.000000;,
  10;1.000000;1.000000;1.000000;1.000000;,
  11;1.000000;1.000000;1.000000;1.000000;,
  12;1.000000;1.000000;1.000000;1.000000;,
  13;1.000000;1.000000;1.000000;1.000000;,
  14;1.000000;1.000000;1.000000;1.000000;,
  15;1.000000;1.000000;1.000000;1.000000;,
  16;1.000000;1.000000;1.000000;1.000000;,
  17;1.000000;1.000000;1.000000;1.000000;,
  18;1.000000;1.000000;1.000000;1.000000;,
  19;1.000000;1.000000;1.000000;1.000000;,
  20;1.000000;1.000000;1.000000;1.000000;,
  21;1.000000;1.000000;1.000000;1.000000;,
  22;1.000000;1.000000;1.000000;1.000000;,
  23;1.000000;1.000000;1.000000;1.000000;,
  24;1.000000;1.000000;1.000000;1.000000;,
  25;1.000000;1.000000;1.000000;1.000000;,
  26;1.000000;1.000000;1.000000;1.000000;,
  27;1.000000;1.000000;1.000000;1.000000;,
  28;1.000000;1.000000;1.000000;1.000000;,
  29;1.000000;1.000000;1.000000;1.000000;,
  30;1.000000;1.000000;1.000000;1.000000;,
  31;1.000000;1.000000;1.000000;1.000000;;
 }
}
