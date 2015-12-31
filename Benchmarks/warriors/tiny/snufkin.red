;redcode-tiny
;name Snufkin
;author John Metcalf
;assert 1

       ORG      START
START  SEQ.I  $   307, $   257
       DJN.F  $     6, $     6
       SNE.I  $  -365, $   190
       SEQ.I  $   132, }     4
       JMP.B  $     3, {     3
       SNE.I  $  -172, $  -311
       JMZ.F  $     8, <     1
       MUL.X  #   -64, #  -249
       SNE.I  >  -127, @    -1
       MUL.F  #    -2, $    -2
       MOV.I  <   355, >    -3
       MOV.I  }    -1, {    -4
       MOV.I  {    -5, >    -5
       DJN.B  $    -3, #    13
       SPL.B  $     1, {  -226
       SPL.B  $     1, >  -297
       SPL.B  @     0, {    60
       MOV.I  }    -1, >    -1
       MOV.I  {    -2, {     1
       DJN.F  $  -355, {   355
