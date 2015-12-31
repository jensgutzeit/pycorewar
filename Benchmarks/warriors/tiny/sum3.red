;redcode-tiny
;name SUM 3
;author Sheep
;assert 1

       ORG      START
       ADD.AB #    10, $     8
START  JMZ.B  $    -1, @     7
       JMP.B  $     8, }   300
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  #     2, #    33
       DAT.F  #     1, #     8
       SPL.B  #     1, #     8
       MOV.I  *    -3, >    -3
       MOV.I  *    -4, >    -4
       DJN.B  $    -2, {   -40
