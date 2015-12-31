;redcode-tiny
;name clock strikes twelve
;author John Metcalf
;assert 1

       ORG      START
       DAT.F  $    82, $    65
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $    -5, $    10
       SPL.B  #  -300, $    20
       MOV.I  @     3, >    -5
       MOV.I  @     2, >    -6
       MOV.I  @     1, >    -7
       DJN.F  $    -2, {    -4
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       SUB.F  $     2, @     2
START  SNE.X  *   -16, @   -16
       SUB.X  #   -34, $   -17
       JMZ.F  $    -3, @   -18
       JMP.B  $   -15, <   -19
