;redcode-tiny
;name Tiny Boss
;author Neogryzor
;assert 1

       ORG      START
       SUB.F  $     3, $     1
START  SNE.I  $    35, $    30
       DJN.B  $    -2, #    79
       SPL.B  #   -30, <   -30
       MOV.I  @     2, >    -3
       MOV.I  @     1, >    -4
       DJN.F  $    -2, {     2
       DAT.F  #   -10, $     7
       SPL.B  #   -10, $     8
