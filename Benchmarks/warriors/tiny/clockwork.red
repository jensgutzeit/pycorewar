;redcode-tiny
;name Clockwork
;author John Metcalf
;assert 1

       ORG      START
       DAT.F  $   138, $   125
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       SPL.B  #     1, {     1
       MOV.I  $    -1, <    -7
       MOV.I  >    -8, >    -8
       JMN.F  $    -2, >    -9
       MOV.BA @     3, @     3
       SUB.F  $     2, @     2
START  SNE.X  *   -12, @   -12
       SUB.X  #   -26, $   -13
       JMZ.F  $    -3, @   -14
       SLT.B  $   -15, #    21
       DJN.B  $    -9, @    -3
       DJN.B  $    -7, #    13
       JMP.B  $    -8, {   -11