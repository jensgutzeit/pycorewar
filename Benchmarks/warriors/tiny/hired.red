;redcode-tiny
;name Hired Sword
;author John Metcalf
;assert 1

       ORG      START
       MOV.B  $     1, #  -263
       SUB.AB #   181, #  -180
       MOV.I  *     3, >    -2
       JMZ.F  $    -2, @    -2
       JMN.B  @     0, *    -4
       SPL.B  $     0, $     0
       MOV.I  $     2, >    -8
       DJN.F  $    -1, >    -9
       DAT.F  <     1, $    13
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
START  MOV.I  $    -5, <   309
       MOV.I  <   309, <   333
       MOV.I  <   333, <   354
       MOV.I  <   354, <   378
       MOV.I  <   378, <   399
       MOV.I  <   399, <  -380
       DJN.F  $   -16, <  -356
