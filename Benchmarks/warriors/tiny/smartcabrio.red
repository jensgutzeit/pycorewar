;redcode-tiny
;name Smart Cabrio
;author LAchi
;assert 1

       ORG      START
       SPL.B  #   -76, #    76
       MOV.I  $    -2, <   -13
       ADD.F  $    -2, $     1
       MOV.I  >     0, $    -3
       DJN.F  $    -2, {  -150
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
START  SNE.I  $   366, $  -177
       SEQ.I  $   323, }     3
       JMP.B  $     2, {     2
       JMZ.F  $   -12, <     1
       MUL.X  #   240, #  -379
       SNE.I  >  -242, @    -1
       MUL.F  #    -2, $    -2
       MOV.I  <  -285, }    -3
       MOV.I  }    -1, <    -4
       DJN.B  $    -2, #    17
       JMP.B  $   -19, <  -200
