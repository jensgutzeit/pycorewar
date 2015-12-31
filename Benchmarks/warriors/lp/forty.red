;redcode-lp
;name Forty
;author Anton Marsden
;assert 1

       ORG      START
       ADD.AB #    91, $     6
START  MOV.I  $     5, $    -2
       MOV.I  *    -3, <    -3
       DJN.A  $    -1, $    -4
       SPL.B  $    -4, >  2667
       JMP.B  @    -6, >  2687
       DAT.F  <     7, <   143
