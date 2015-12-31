;redcode-tiny
;name Betadine
;author M Joonas Pihlaja
;assert 1

       ORG      START
       DAT.F  $    45, $    35
       DAT.F  #   -10, $     9
       SPL.B  #   -10, $     9
       MOV.I  @     3, >    -3
       MOV.I  @     2, >    -4
       MOV.I  @     1, >    -5
       DJN.F  $    -3, {    -4
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $    20, $    20
       ADD.F  $    -1, $   -16
START  SNE.I  @   -17, *   -17
       DJN.F  $    -2, <  -205
       JMP.B  $   -17, <   -19
