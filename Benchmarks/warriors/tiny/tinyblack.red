;redcode-tiny
;name Tiny Black Knight
;author Christian Schmidt
;assert 1

       ORG      START
START  MOV.I  $    14, $  -235
       MOV.I  $    14, $   128
       SPL.B  $     1, {    39
       SPL.B  $     1, }   -47
       MOV.I  <     2, {     2
       MOV.I  <     2, {     2
       SPL.B  $  -192, $     8
       JMP.B  $   314, $    13
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       SPL.B  #     0, }  -201
       MOV.I  $   -46, $   -85
       ADD.AB {     0, }     0
       DJN.F  $    -2, <  -379
       DAT.F  >    -1, >     1
       MOV.I  #   267, *     0
       SPL.B  #     0, >  -365
       ADD.A  #   267, $     1
       SPL.B  $   342, <  -342
       DJN.F  $   290, <  -358
