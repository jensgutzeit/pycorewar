;redcode-lp
;name Gelatinous Blob
;author Michal Janeczek
;assert 1

       ORG      START
       MOV.I  $     2, <  -130
       JMP.B  $    -1, <  -131
       DAT.F  <    -6, <     1
       DAT.F  <    -9, <     1
       DAT.F  <   -12, <     1
       DAT.F  <   -15, <     1
       DAT.F  <   -18, <     1
       DAT.F  <   -21, <     1
       DAT.F  <   -24, <     1
       DAT.F  <   -27, <     1
       DAT.F  <   -30, <     1
       DAT.F  <   -33, <     1
       DAT.F  <   -36, <     1
       DAT.F  <   -39, <     1
       DAT.F  <   -42, <     1
       ADD.AB #     7, $     7
       MOV.I  $     5, <     3
       MOV.I  <    -2, <     3
       JMN.B  $    -2, @    -3
       SPL.B  @    -4, <  2159
       JMZ.B  @     0, $  3039
       DAT.F  < -2666, <  2667
START  SPL.B  $    -5, <  2000
       SPL.B  $   -23, <  2500
       SPL.B  $     4, <  3000
       SPL.B  $     2, <  3500
       JMP.B  $     5, <  4000
       JMP.B  $  2671, < -3500
       SPL.B  $     2, < -3000
       JMP.B  $ -2664, < -2500
       JMP.B  $     2, < -2000
       MOV.I  $     0, $  2667
