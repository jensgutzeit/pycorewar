;redcode-94nop
;name slime test 1.00
;author David Houston
;assert 1

       ORG      START
       DAT.F  >   -87, >  -149
       SPL.B  $     2, $     0
       SPL.B  $     2, $  -463
       SPL.B  $     1, $  3380
       SPL.B  $     1, $  -777
       MOV.I  <     1, {     1
       JMP.B  $  1725, $    13
       DAT.F  $   -50, $  2752
       DAT.F  $   -50, $ -1405
       SPL.B  @     0, $  -512
       MOV.I  }    -1, >    -1
       SPL.B  @     0, $  2408
       MOV.I  }    -1, >    -1
       MOV.I  #     1, {     1
       MOV.I  # -3523, <     1
       MOV.I  $     3, < -2092
       MOV.I  {    -5, <     1
       JMP.B  @     0, >  3671
       DAT.F  <  2667, < -2666
START  SNE.I  $  1539, $ -3081
       SEQ.I  <   -20, $  3210
       JMP.B  $    25, }    25
       SNE.I  $   288, $  3668
       SEQ.I  <   -15, $  1959
       JMP.B  $    22, {    22
       SNE.I  $ -3559, $  -179
       SEQ.I  <   -19, $ -1888
       DJN.A  $    19, {    19
       SNE.I  $   906, $   129
       SEQ.I  <   -25, $ -1581
       JMP.B  $    16, }    18
       SNE.I  $  1215, $   752
       SEQ.I  <   -30, $  -958
       JMP.B  $    13, {    15
       SNE.I  $ -2944, $   436
       SEQ.I  <   -32, $ -1274
       JMP.B  $    10, $     0
       SEQ.I  >    13, $ -2900
       JMP.B  $     9, <    12
       SEQ.I  $  -339, $  3040
       JMP.B  $     6, }     6
       SEQ.I  $ -2845, $   534
       JMP.B  $     4, {     4
       SEQ.I  $ -2534, $   845
       DJN.A  $     2, {     2
       JMZ.F  $   -44, $  2095
       MUL.B  *     2, $     4
       SNE.I  *   -39, @     3
       ADD.B  $   -45, $     2
       MOV.I  $   -49, @     1
       MOV.I  $   -50, }  1708
       SUB.AB #    -7, $    -1
       DJN.B  $    -3, #    14
       JMP.B  $   -52, $     0
