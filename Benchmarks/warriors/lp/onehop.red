;redcode-lp
;name One Hop
;author David Moore
;assert 1

       ORG      START
       ADD.AB #  2891, #  2898
START  JMZ.F  $    -1, @    -1
       SLT.B  $    -2, #     7
       MOV.I  *     4, @    -3
       JMP.B  $    -4, $     0
       DAT.F  $     0, $     0
       JMP.B  #     1, {     1
       DAT.F  #    -1, <     0
