;redcode-tiny
;name Digital Swarm
;author John Metcalf
;assert 1

       ORG      START
START  SNE.X  $  -200, $  -136
       SEQ.X  $   183, $   -41
       DJN.F  @     9, @     9
       SNE.X  $  -295, $  -320
       SEQ.X  $   -96, }     4
       JMP.B  @     6, {     3
       SNE.X  $  -248, $   127
       JMZ.F  $     7, <     1
       MUL.X  #   300, #  -175
       JMZ.F  @     2, >    -1
       MOV.I  {  -281, >    -2
       MOV.I  }    -1, {    -3
       SEQ.I  {    -4, >    -4
       DJN.F  $    -3, >    -5
       SPL.B  $     1, {     0
       SPL.B  *     2, {     0
       SPL.B  @     0, <   104
       MOV.I  }    -1, >    -1
       MOV.I  {    -2, <     1
       DJN.F  *    57, >    60
