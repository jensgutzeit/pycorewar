;redcode-lp
;name Mimp
;author Ben Ford
;assert 1

       ORG      START
       MOV.I  $    10, $ -3990
       SPL.B  $     1, $     0
       SPL.B  $     1, $     0
       SPL.B  $     2, $     0
       JMP.B  @     5, {     0
       JMP.B  *     4, {     0
       JMP.B  $     7, $ -3993
       JMP.B  $ -3995, $     5
       JMP.B  $     3, $ -3997
       JMP.B  $ -3999, $     1
       MOV.I  #    11, $ -3999
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
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
START  SEQ.I  $ -3739, $   261
       DJN.F  $    17, { -3731
       SNE.I  $ -2781, $  1219
       SEQ.I  $   978, <    22
       JMP.B  $    13, >    21
       SNE.I  $ -2064, $  1936
       SEQ.I  $  1695, <    18
       JMP.B  $    10, {    10
       SNE.I  $ -1587, $  2413
       SEQ.I  $  2172, <    17
       JMP.B  $     7, }     7
       SEQ.I  $ -3510, $   490
       JMP.B  $     5, <    13
       SEQ.I  $ -2552, $  1448
       DJN.B  $     3, {     3
       SNE.I  $ -3274, $   726
       JMZ.F  $   -81, $ -1122
       MUL.B  $     8, $     4
       SNE.I  <  1000, @     3
       ADD.AB #  4000, $     2
       MOV.I  $     6, @     1
       MOV.I  $     5, *   240
       SUB.AB #     7, $    -1
       DJN.B  $    -3, #     8
       DJN.F  $   -89, #  -292
       DAT.F  {    46, $  -296
       DAT.F  {    46, $  -290
