;redcode-94nop
;name Recovery
;author Ian Oversby
;assert 1

       ORG      START
       SPL.B  #     0, #     0
       SPL.B  #     0, < -3200
       MOV.I  $     4, @     2
       ADD.AB # -2616, $     1
       MOV.I  $     2, @ -2617
       DJN.F  $    -3, < -3204
       DAT.F  >     1, #  2692
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $ -2000, $  -400
       DAT.F  $  -800, $  -200
       DAT.F  $  3400, $  -600
       SPL.B  #    12, $   -12
       ADD.A  $    -3, $    -3
       ADD.A  @    -1, $     6
       ADD.BA *    -2, @    -1
       SNE.I  $    -9, *     4
       ADD.A  #  -100, $     3
       MOV.I  $    68, *     2
       ADD.AB $     1, $     1
       MOV.I  $  -345, $     6
       ADD.F  $    -9, $    -1
       MOV.I  $    64, *    -2
       DJN.B  $    -3, #     8
       JMP.B  $    33, } -2600
START  SEQ.I  $  -350, $  -450
       JMP.B  $   -10, }  -401
       SEQ.I  $  -552, $  -652
       JMP.B  $   -13, }  -603
       SEQ.I  $ -2954, $ -3054
       JMP.B  $   -15, {   -15
       SEQ.I  $  -756, $  -856
       JMP.B  $   -17, {   -19
       SEQ.I  $  -958, $ -1058
       JMP.B  $   -19, }   -21
       SEQ.I  $ -1160, $ -1260
       JMP.B  >   -21, } -1211
       SEQ.I  $ -1362, $ -1462
       JMP.B  $   -24, } -1413
       SEQ.I  $ -1564, $ -1664
       JMP.B  $   -26, {   -27
       SEQ.I  $ -1766, $ -1866
       JMP.B  $   -28, }   -29
       SEQ.I  $ -2368, $ -2468
       JMP.B  >   -29, <   -31
       SEQ.I  $ -2570, $ -2670
       JMP.B  $   -32, <   -33
       SEQ.I  $ -2772, $ -2872
       DJN.F  $   -34, $   -35
       SEQ.I  $  3026, $  2926
       JMP.B  >   -35, >   -37
       SEQ.I  $  2824, $  2724
       JMP.B  $   -38, >   -39
       SEQ.I  $ -3778, $ -3878
       JMP.B  $   -40, {   -39
       SEQ.I  $ -1980, $ -2080
       JMP.B  <   -41, } -2031
       SPL.B  $   -70, < -1100
       SPL.B  $   -71, < -1208
       SPL.B  $   -72, < -1316
       JMP.B  $    16, < -1425
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       SPL.B  $     1, < -1500
       SPL.B  $     1, < -1608
       SPL.B  $     1, < -1716
       SPL.B  @     0, <   117
       MOV.I  }    -1, >    -1
       SPL.B  @     0, <   771
       MOV.I  }    -1, >    -1
       MOV.I  $     3, }  3307
       MOV.I  {    -3, <     1
       JMP.B  @     0, <   748
       DAT.F  <  2667, < -2666
