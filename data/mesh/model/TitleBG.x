xof 0303txt 0032

Frame Root {
  FrameTransformMatrix {
     1.000000, 0.000000, 0.000000, 0.000000,
     0.000000,-0.000000, 1.000000, 0.000000,
     0.000000, 1.000000, 0.000000, 0.000000,
     0.000000, 0.000000, 0.000000, 1.000000;;
  }
  Frame obj1 {
    FrameTransformMatrix {
       1.000000, 0.000000, 0.000000, 0.000000,
       0.000000, 1.000000, 0.000000, 0.000000,
       0.000000, 0.000000, 1.000000, 0.000000,
       0.000000, 0.000000, 0.000000, 1.000000;;
    }
    Mesh { // obj1 mesh
      4;
      -0.500000; 0.000000; 0.500000;,
      -0.500000;-0.000000;-0.500000;,
       0.500000;-0.000000;-0.500000;,
       0.500000; 0.000000; 0.500000;;
      1;
      4;3,2,1,0;;
      MeshNormals { // obj1 normals
        4;
         0.000000;-1.000000; 0.000000;,
         0.000000;-1.000000; 0.000000;,
         0.000000;-1.000000; 0.000000;,
         0.000000;-1.000000; 0.000000;;
        1;
        4;3,2,1,0;;
      } // End of obj1 normals
      MeshTextureCoords { // obj1 UV coordinates
        4;
         0.000000; 0.000000;,
         0.000000; 1.000000;,
         1.000000; 1.000000;,
         1.000000; 0.000000;;
      } // End of obj1 UV coordinates
      MeshMaterialList { // obj1 material list
        1;
        1;
        0;
        Material mat1 {
           0.800000; 0.800000; 0.800000; 0.000000;;
            7.843137;
           0.000000; 0.000000; 0.000000;;
           0.000000; 0.000000; 0.000000;;
          TextureFilename {"TitleBG01.bmp";}
        }
      } // End of obj1 material list
    } // End of obj1 mesh
  } // End of obj1
} // End of Root
