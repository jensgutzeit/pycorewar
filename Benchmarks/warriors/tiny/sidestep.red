;redcode-tiny
;name Sidestep
;author Christian Schmidt
;assert 1

       ORG      START
       ADD.F  $    13, $     3
START  SNE.I  *     2, @     2
       ADD.F  $    11, $     1
       SNE.I  $   -53, $   -63
       DJN.F  $    -4, <   194
       SUB.AB #    19, $    -2
       MOV.I  <    14, <    -3
       MOV.I  <    13, <    -4
       MOV.I  <    12, <    -5
       MOV.I  <    11, <    -6
       SPL.B  @    -7, <   129
       MOV.I  <     9, <    -8
       MOV.I  <     8, <    -9
       DAT.F  #   -30, <   -30
       DAT.F  $     0, $     7
       DAT.F  $     1, $    11
       SPL.B  #   -38, $    13
       MOV.I  @     2, >    -3
       MOV.I  @     1, >    -4
       DJN.F  $    -2, {    -3
