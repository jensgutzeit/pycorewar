;redcode-lp
;name Can't think of any ...
;author Philip Kendall
;assert 1

       ORG      START
       DAT.F  $   -20, $    16
       DAT.F  $     2, $     2
START  MOV.I  $    -1, *     2
       MOV.I  $     9, @     1
       MOV.I  $  3207, * -1588
       ADD.F  $     2, @    -2
       JMZ.F  $    -4, $ -3205
       SPL.A  #  1615, >  1615
       MOV.I  $    -8, >    -9
       MOV.I  $    -9, >   -10
       DJN.F  $    -2, >   -11
       DAT.F  $     0, $     0
       MOV.I  $  3205, $     1
