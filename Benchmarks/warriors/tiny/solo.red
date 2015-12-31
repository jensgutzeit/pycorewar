;redcode-tiny
;name SoLo (Mini+no 1,-1)
;author Roy van Rijn
;assert 1

       ORG      START
       DAT.F  $    10, $  -390
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       MOV.I  $    13, <    -3
       MOV.I  >    -4, >    -4
       JMN.F  $    -2, >    -5
START  SUB.X  #    -6, $    -6
       SNE.I  *    -7, @    -7
       SUB.X  *     3, @    -2
       JMN.F  $     3, @    -9
       JMZ.F  $    -4, *   -10
       MOV.X  @    -5, @    -5
       SLT.B  @    -6, #    23
       DJN.B  $   -10, @    -7
       DJN.B  *    -3, #    14
       NOP.F  {     3, }     4
       SPL.B  #     0, {     0
       MOV.I  $     2, >   -16
       DJN.F  $     0, {   -17
       DAT.F  >     0, $    20
