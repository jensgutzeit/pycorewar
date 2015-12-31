;redcode-tiny
;name Four Winds
;author John Metcalf
;assert 1

       ORG      START
       SUB.F  $     3, $     1
START  SNE.B  >   381, >   376
       DJN.F  $    -2, <    -1
       SPL.B  #   106, <   105
       MOV.I  @     2, >    -3
       MOV.I  @     1, >    -4
       DJN.F  $    -2, {     2
       DAT.F  $    -5, $     8
       SPL.B  #  -101, $    16
