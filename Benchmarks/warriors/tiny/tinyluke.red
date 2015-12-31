;redcode-tiny
;name TinyLuke
;author Philip Thorne
;assert 1

       ORG      START
       DAT.F  $     1, $    15
       SPL.B  #    38, $    15
       DAT.F  $     0, $     0
       SPL.B  #   125, }    -9
       MOV.I  *    -4, >   -10
       MOV.I  *    -5, >   -11
       DJN.F  $    -2, }    -5
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
START  MOV.I  >    76, #     0
       MUL.AB #  -219, $     2
       JMZ.F  $    -1, @     1
       SPL.B  $   -12, $    73
       MOV.B  $    -1, $   -22
       MOV.I  <     3, @    -2
       DAT.F  $     0, $     0
       JMP.B  #     0, $     0
