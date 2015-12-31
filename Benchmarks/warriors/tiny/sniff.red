;redcode-tiny
;name Sniff
;author John Metcalf
;assert 1

       ORG      START
       DAT.F  $   367, $    55
       DAT.F  $     0, $     0
       DAT.F  >     0, $     0
       SPL.B  #     0, {     0
       MOV.I  *    -1, <    -4
       MOV.I  >    -5, >    -5
       JMN.F  $    -2, >    -6
       SUB.X  #   -11, $    -7
START  SNE.F  @    -8, *    -8
       SUB.F  $    -2, @    -2
       JMN.F  $     3, @   -10
       JMZ.F  $    -4, *   -11
       MOV.X  @    -5, @    -5
       SLT.B  @    -6, #    20
       JMP.B  $   -10, <   -14
       DJN.B  $    -8, #    13
       JMP.B  $    -9, {   -12
