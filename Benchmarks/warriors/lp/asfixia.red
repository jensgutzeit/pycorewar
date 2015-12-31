;redcode-lp
;name Asfixia
;author G.Labarga
;assert 1

       ORG      START
       MOV.I  *     1, *     3
       MOV.I  $    11, @     2
       ADD.F  $    10, $     1
START  SNE.I  $  1186, $  1192
       ADD.F  $     8, $    -1
       SEQ.I  *    -2, @    -2
       SLT.AB #    18, $    -3
       DJN.B  $    -5, #  1400
       JMN.B  $    -8, $    -1
       MOV.I  $     2, >   -12
       JMP.B  $    -1, >   -14
       DAT.F  <  2667, $    15
       JMP.B  #  2372, $  2372
