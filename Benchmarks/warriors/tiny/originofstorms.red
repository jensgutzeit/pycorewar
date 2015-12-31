;redcode-tiny
;name Origin of Storms
;author John Metcalf
;assert 1

       ORG      START
START  SNE.I  $    40, $   244
       ADD.F  $     7, $    -1
       MOV.I  $     3, }    -2
       MOV.I  $     2, >    -3
       DJN.B  @    -2, #    43
       SPL.B  #     1, $     1
       MOV.I  $     2, >    -6
       DJN.F  $    -1, >    -7
       DAT.F  $    12, $    12
