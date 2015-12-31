;redcode-tiny
;name tiny BiShot
;author Christian Schmidt
;assert 1

       ORG      START
START  ADD.F  $     4, $     8
       SNE.I  *     7, @     7
       JMP.B  $    -2, $     0
       JMP.B  $     7, $     0
       DAT.F  $    10, $    10
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       SPL.B  {    29, >  -371
       DAT.F  {    10, >  -390
       SPL.B  #    20, <  -100
       MOV.I  $    -1, >    -3
       MOV.I  $    -2, }    -4
       DJN.A  $    -2, <    -3
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
