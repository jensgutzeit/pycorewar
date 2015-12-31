;redcode-tiny
;name Provenance
;author John Metcalf
;assert 1

       ORG      START
       DAT.F  $   204, $   196
       DAT.F  $     2, $     9
       SPL.B  #   -30, $    14
       MOV.I  @     2, >    -3
       MOV.I  @     1, >    -4
       DJN.F  $    -2, {    -3
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       SUB.F  $     8, $    -9
START  SEQ.I  *   -10, @   -10
       DJN.F  $    -9, $   -11
       JMZ.F  $    -3, $    -6
       MOV.I  @     2, {     2
       DJN.B  $    -1, $     1
       DJN.F  *   -52, #     5
       DAT.F  <     1, $    11
       SPL.B  #   -48, }   -48
       MOV.I  $    -2, >    -3
       DJN.F  $    -1, >    -4
