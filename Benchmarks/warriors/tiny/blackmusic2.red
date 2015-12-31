;redcode-tiny
;name BlackMusic2
;author Nenad Tomasev
;assert 1

       ORG      START
       DAT.F  $     0, $     0
       DAT.F  $     1, $     8
       SPL.B  #    28, $    28
       MOV.I  *    -2, >    -3
       MOV.I  *    -3, >    -4
       DJN.F  $    -2, }    -3
       SUB.F  $    -4, $     1
START  SNE.I  }   -24, }   -30
       DJN.F  $    -2, >     2
       MOV.B  $    -2, $    -9
       JMP.B  $    -8, $    29
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       SPL.B  #     1, $     1
       SPL.B  #     1, }     1
       SPL.B  #     1, {     1
       SPL.B  #     1, *     1
       SPL.B  #     1, @     1
       SPL.B  #     1, #     1
