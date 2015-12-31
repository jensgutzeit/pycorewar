;redcode-lp
;name LP-Scan v0.1
;author John Metcalf
;assert 1

       ORG      START
       ADD.AB #  2191, #  2196
START  JMZ.F  $    -1, @    -1
       SLT.B  $    -2, #     5
       MOV.I  *     2, @    -3
       JMP.B  $    -4, <  4000
       DAT.F  $     1, $     1
       JMP.B  #     2, <  2667
