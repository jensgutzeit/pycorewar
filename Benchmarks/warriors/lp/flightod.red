;redcode-lp
;name Flight of Dragons
;author Ian Oversby
;assert 1

       ORG      START
       MOV.I  <     3, <     2
       DJN.B  $    -1, #     6
       JMP.B  @     0, $  3200
       SPL.B  $     0, $     6
       ADD.AB #  2668, $     1
       JMP.B  @     0, $ -2667
       MOV.I  $     0, $  2667
       MOV.I  $     0, $  2667
       MOV.I  $     0, $  2667
       SPL.B  #    76, <   -76
       MOV.I  {    76, $   -74
       ADD.F  $    -2, $    -1
       DJN.F  $    -2, <  3730
       MOV.I  $    76, <   -76
       JMP.B  $     0, $     0
       DAT.F  #     0, #     0
START  JMP.B  $     1, $     0
       MOV.I  <    -2, <     2
       DJN.B  $    -1, #     6
       JMP.B  @     0, $ -2500
       DAT.F  <   -96, <   -96
       SPL.B  $     0, <   -95
       SPL.B  $     0, #     0
       MOV.I  <    94, $   -93
       ADD.F  $     2, $    -1
       DJN.B  $    -2, < -2570
       MOV.I  $    95, <   -95
