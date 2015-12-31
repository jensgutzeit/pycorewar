;redcode-tiny
;name Unknown
;author John Metcalf
;assert 1

       ORG      START
       DAT.F  $   158, $     0
       DAT.F  $     8, $     1
       DAT.F  $     0, $     0
       SPL.B  #     0, $     0
       MOV.I  $    -3, }    -4
       DJN.F  $    -1, }    -5
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       MOV.I  #   267, *     0
       SPL.B  $   335, $    68
       SPL.B  $    68, $  -199
       JMP.B  $  -199, $   334
START  SPL.B  $   -10, }    67
       MOV.I  $    -5, $    64
       SPL.B  $     1, }  -261
       SPL.B  $     1, {  -283
       SPL.B  $     2, {  -305
       JMP.B  @    -8, }     0
       JMP.B  *    -9, }     0
