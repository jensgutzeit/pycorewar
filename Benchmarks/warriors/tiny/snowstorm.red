;redcode-tiny
;name Snowstorm
;author John Metcalf
;assert 1

       ORG      START
       SPL.B  #  -105, >   105
       MOV.I  >  -210, $   211
       ADD.F  $    -2, $    -1
       DJN.F  $    -2, >   102
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
START  SPL.B  $    -8, >   370
       SPL.B  $     1, >   390
       SPL.B  $     1, <  -390
       SPL.B  $     1, }  -370
       SPL.B  $     5, {  -350
       DJN.F  $     6, {  -300
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       ADD.F  #   267, $    -4
       DAT.F  $     0, $     0
       MOV.I  #   267, *     0
