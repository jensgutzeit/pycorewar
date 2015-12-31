;redcode-tiny
;name moondrops burst
;author John Metcalf
;assert 1

       ORG      START
       SUB.F  $     4, $   188
       MOV.I  $   187, *   187
       MOV.I  $   186, @   186
       JMZ.F  $    -3, $  -185
       SPL.B  #   215, <  -215
       MOV.I  @     2, >    -3
       MOV.I  @     1, >    -4
       DJN.F  $    -2, {     3
       DAT.F  $     0, $     0
       DAT.F  $   -11, $    10
       SPL.B  #  -160, $    11
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  <   400, <    30
START  MOV.I  $    -1, $   170
       DJN.F  $   -18, {   227
