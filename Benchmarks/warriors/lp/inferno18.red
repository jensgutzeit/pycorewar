;redcode-lp
;name Inferno 1.8
;author Philip Kendall
;assert 1

       ORG      START
       DAT.F  > -2665, $    15
       MOV.I  $  3205, $     1
       JMP.A  #     1, $     1
START  MOV.I  $    -1, *     2
       MOV.I  $    -3, @     1
       MOV.I  $  3207, * -1588
       ADD.F  $     2, @    -2
       JMZ.F  $    -4, $ -3205
       SPL.A  #  1615, >  1615
       MOV.I  $    -9, >   -10
       DJN.F  $    -1, >   -11
       JMP.A  $    -3, >   -12
