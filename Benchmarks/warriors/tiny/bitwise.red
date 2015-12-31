;redcode-tiny
;name Bitwise
;author Roy van Rijn
;assert 1

       ORG      START
START  SNE.X  $  -200, $  -136
       SEQ.X  $   183, $   -41
       DJN.F  @     9, @     9
       SNE.X  $  -295, $  -320
       SEQ.X  $   -96, }     4
       DJN.B  @     6, {     3
       SNE.X  $  -248, $   128
       JMZ.F  $     7, <     1
       MUL.X  #   300, #  -175
       JMZ.F  @     2, >    -1
       MOV.I  {  -281, >    -2
       MOV.I  }    -1, {    -3
       SEQ.I  {    -4, >    -4
       DJN.F  $    -3, >    -5
       SPL.B  }     1, {   203
       SPL.B  $     0, {   274
       SPL.B  $   183, {     2
       MOV.I  }     1, }    -1
       MOV.I  *     2, }    -2
       JMZ.F  $    -3, *    -1
