;redcode-lp
;name oldtimer
;author bjoern guenzel
;assert 1

       ORG      START
       DAT.F  $     0, $     0
       ADD.AB #    91, $     7
START  MOV.I  $     6, $    -2
       MOV.I  *    -3, <    -3
       MOV.I  $     4, }  2667
       DJN.A  $    -2, $    -5
       SPL.B  $    -5, $     0
       JMP.B  @    -7, $     0
       DAT.F  <     8, <   144
