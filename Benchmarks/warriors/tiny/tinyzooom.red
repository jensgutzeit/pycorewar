;redcode-tiny
;name Tiny Zooom...
;author John Metcalf
;assert 1

       ORG      START
       MOV.I  }     3, *     0
       MOV.I  *     5, >    -1
       SUB.AB #   214, #  -216
       JMZ.F  $    -1, @    -1
       MOV.B  $    -2, $    -4
       JMN.A  {    -2, *    -5
       SPL.B  #     0, $     0
       MOV.I  $     2, >    -4
       DJN.F  $    -1, {    -5
       DAT.F  $     1, $     8
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
START  MOV.I  >   246, {   338
       MOV.I  >  -361, {  -259
       DJN.F  $   -16, {  -167
