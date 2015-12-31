;redcode-tiny
;name Diptera
;author Nenad Tomasev
;assert 1

       ORG      START
       DAT.F  $    45, $   395
       DAT.F  $    18, <   267
       SPL.B  #    20, >  -170
       MOV.I  $    -1, >    -3
       MOV.I  $    -2, }    -4
       DJN.A  $    -2, <    -3
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       SUB.X  #   -15, $   -14
START  SNE.I  *   -15, <   -15
       DJN.B  $    -2, <  -340
       JMP.B  $   -15, <   -17
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
