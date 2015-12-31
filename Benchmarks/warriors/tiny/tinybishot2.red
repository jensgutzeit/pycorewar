;redcode-tiny
;name Tiny BiShot 2.0
;author Christian Schmidt
;assert 1

       ORG      START
       ADD.F  #     8, $     8
START  SNE.I  *     7, @     7
       DJN.F  $    -2, @     6
       DJN.F  $     7, $     5
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $    34, $  -362
       DAT.F  $    10, $  -390
       SPL.B  #    20, $  -200
       MOV.I  $    -1, }    -3
       MOV.I  $    -2, >    -4
       DJN.A  $    -2, <    -3
