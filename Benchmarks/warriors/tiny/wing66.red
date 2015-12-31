;redcode-tiny
;name WingShot 0.66
;author Ben Ford
;assert 1

       ORG      START
       ADD.F  $     4, $     8
START  SNE.I  *     7, @     7
       DJN.B  $    -2, #    80
       DJN.F  $     8, @    -2
       DAT.F  #    10, #    10
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       SPL.B  {    41, >  -363
       DAT.F  $     0, $     0
       DAT.F  {    10, >   400
       SPL.B  #    10, <   -20
       MOV.I  $    -1, }    -4
       MOV.I  $    -2, >    -5
       DJN.A  $    -2, <    -3
