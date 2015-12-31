;redcode-tiny
;name little goonie
;author David Moore
;assert 1

       ORG      START
       DAT.F  $     0, $     0
       DAT.F  <  -266, <   267
       DAT.F  $    -1, $    11
       SPL.B  #   367, $    13
       MOV.I  $    -1, >    -4
       MOV.I  *    -1, >    -5
       DJN.A  $    -2, $    -6
       DJN.A  $    -3, *    -1
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
START  SPL.B  $     3, >  -198
       SUB.F  $     4, $     1
       SNE.I  $    19, $    25
       JMP.B  $    -2, {  -172
       MOV.AB $    -2, @   -13
       DJN.F  $   -15, #   -15
