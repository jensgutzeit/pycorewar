;redcode-lp
;name red eye
;author bjoern guenzel
;assert 1

       ORG      START
START  ADD.F  $     8, $     1
       SNE.I  $     1, $    69
       DJN.F  $    -2, < -1002
       MOV.I  $     4, *    -2
       MOV.I  $     3, @    -3
       JMP.B  *    -3, < -1005
       DAT.F  $     0, $     0
       JMP.B  #    10, >    -1
       DAT.F  #   136, $   136
       DAT.F  >  2667, #     7
       DAT.F  $     0, $     0
       SPL.B  #     0, <     0
       MOV.I  $    -3, >    -4
       JMP.B  $    -1, >  2662
