;redcode-tiny
;name Tiny Star v0.10
;author brx
;assert 1

       ORG      START
       SPL.B  #     8, #     0
       SPL.B  #     2, #    14
       DAT.F  #     0, #    14
       DAT.F  $     0, $     0
       ADD.F  $     8, $     1
START  SNE.I  $    38, $    48
       DJN.F  $    -2, *    -1
       MOV.AB $    -2, $    -7
       SPL.B  #     1, #    14
       MOV.I  *    -9, >    -9
       MOV.I  *   -10, >   -10
       DJN.F  $    -2, <  -211
       DAT.F  #    20, #    20
