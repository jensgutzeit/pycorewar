;redcode-lp
;name Qwill v3
;author Ryan Coleman
;assert 1

       ORG      START
       DAT.F  $  2000, $   400
       DAT.F  $   800, $   200
       DAT.F  $ -3400, $   600
       JMP.B  $  -200, $    15
       DAT.F  $    15, $   -15
       ADD.A  $    -4, $    -4
       ADD.AB @    -1, $     5
       ADD.B  *    -2, @    -1
       SNE.I  $   189, @     3
       ADD.AB #   100, $     2
       MOV.I  $    -7, @     1
       MOV.I  $    -7, @   407
       ADD.BA $    -1, $    -1
       MOV.I  $   -10, *    -2
       ADD.F  $   -10, $    -3
       MOV.I  $   -12, @    -4
       DJN.B  $    -3, #     6
       JMP.B  $    57, }  -300
START  SEQ.I  $   400, $   500
       JMP.B  $   -11, }  2600
       SEQ.I  $   598, $   698
       JMP.B  $   -14, }   647
       SEQ.I  $   796, $   896
       JMP.B  $   -16, {   -18
       SEQ.I  $   994, $  1094
       JMP.B  $   -18, }   -20
       SEQ.I  $  2992, $  3092
       JMP.B  $   -20, {   -20
       SEQ.I  $  1190, $  1290
       JMP.B  >   -22, }  1239
       SEQ.I  $  1388, $  1488
       JMP.B  $   -25, }  1437
       SEQ.I  $  1586, $  1686
       JMP.B  $   -27, {   -28
       SEQ.I  $  1784, $  1884
       JMP.B  $   -29, }   -30
       SEQ.I  $  2382, $  2482
       JMP.B  >   -30, <   -32
       SEQ.I  $  2580, $  2680
       JMP.B  $   -33, <   -34
       SEQ.I  $  2778, $  2878
       DJN.F  $   -35, $   -36
       SEQ.I  $ -3024, $ -2924
       JMP.B  >   -36, >   -38
       SEQ.I  $ -2826, $ -2726
       JMP.B  $   -39, >   -40
       SEQ.I  $  3772, $  3872
       JMP.B  $   -41, {   -40
       SEQ.I  $  1970, $  2070
       JMP.B  <   -42, }  2019
       SEQ.I  $  2168, $  2268
       JMP.B  $   -46, }  2217
       SEQ.I  $  3366, $  3466
       JMP.B  $   -48, <   -48
       SEQ.I  $  3564, $  3664
       JMP.B  $   -50, {   -50
       SEQ.I  $ -3638, $ -3538
       DJN.F  <   -50, $   -52
       SEQ.I  $ -3440, $ -3340
       JMP.B  $   -54, {   -52
       SEQ.I  $ -3242, $ -3142
       DJN.F  $   -56, $   -56
       SEQ.I  $ -2244, $ -2144
       JMP.B  <   -56, >   -58
       SEQ.I  $ -2046, $ -1946
       JMP.B  $   -60, >   -60
       SEQ.I  $ -1648, $ -1548
       JMP.B  $   -62, }   -62
       JMP.B  $     6, $     6
       DAT.F  #  2862, # -2862
       JMP.B  $     0, $     0
       ADD.F  $    -2, $     1
       MOV.I  <     0, $     0
       JMP.B  $    -2, $     0
       MOV.I  $    -5, $   995
       MOV.I  $    -5, $   995
       MOV.I  $    -5, $   995
       MOV.I  $    -5, $   995
       MOV.I  $    -5, $   995
       SPL.B  $   992, $   992
       MOV.I  $   -11, $ -3001
       MOV.I  $   -11, $ -3001
       MOV.I  $   -11, $ -3001
       MOV.I  $   -11, $ -3001
       MOV.I  $   -11, $ -3001
       JMP.B  $ -3004, $ -3004
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $    -1, $    -1
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
