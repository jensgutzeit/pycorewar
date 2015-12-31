;redcode-tiny
;name Djinn Test Tiny 326
;author Steve Gunnell
;assert 1

       ORG      START
       DAT.F  $     0, $   333
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
START  SPL.B  #     0, >    -7
       MOV.I  $    11, >    -8
       DJN.F  $    -1, >    -9
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  <   267, $    19
       DAT.F  <   267, $    19
