;redcode-tiny
;name WingShot T
;author Ben Ford
;assert 1

       ORG      START
       SPL.B  {    39, >  -364
       DAT.F  {    10, >   400
       SPL.B  #    10, <   -20
       MOV.I  $    -1, >    -3
       MOV.I  $    -2, }    -4
       DJN.A  $    -2, <    -3
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       SUB.F  #   -10, $   -10
START  SNE.I  *   -11, @   -11
       SUB.X  $    -2, @    -2
       SNE.I  *   -13, @   -13
       DJN.B  $    -4, #    80
       DJN.F  $   -13, @    -4
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
