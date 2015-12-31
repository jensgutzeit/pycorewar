;redcode-lp
;name I KILL U
;author LAchi
;assert 1

       ORG      START
       DAT.F  #     0, #     0
       DAT.F  #     0, #     0
       DAT.F  #     0, #     0
START  SPL.B  $    15, $     0
       JMP.B  $     2, $     0
       DAT.F  #     0, #     0
       MOV.AB #  2216, $    -6
       MOV.AB #    21, $    -5
       JMP.B  $     2, $     0
       DAT.F  #     0, #     0
       MOV.I  @    -8, <   -10
       DJN.B  $    -1, $    -9
       JMP.B  $     2, $     0
       DAT.F  #     0, #     0
       SPL.B  @   -14, $     0
       ADD.AB #  1129, $   -15
       JMP.B  $    -9, $     0
       DAT.F  #     0, #     0
       ADD.AB #    88, $     4
       MOV.I  $     3, @     3
       JMP.B  $    -2, $     0
       DAT.F  #     0, #     0
       DAT.F  #     0, #    -1
