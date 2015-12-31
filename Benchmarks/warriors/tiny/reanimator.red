;redcode-tiny
;name Reanimator
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
       MOV.I  #   267, *     0
START  SPL.B  $    -6, {  -217
       MOV.I  $    -2, $    68
       SPL.B  $     4, {  -240
       SPL.B  $     6, {   313
       SPL.B  $     4, {   291
       JMP.B  $    64, }     0
       SPL.B  $    -2, {   123
       SPL.B  $     3, {    41
       JMP.B  $   328, }     0
       SPL.B  $    -4, {  -270
       JMP.B  $  -207, }     0
