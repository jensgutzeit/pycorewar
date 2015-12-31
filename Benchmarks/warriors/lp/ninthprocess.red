;redcode-lp
;name ~~ The 9th Process v8 ~~
;author Zul Nadzri
;assert 1

       ORG      START
       ADD.AB #  2891, #  2901
START  JMZ.F  $    -1, @    -1
       SLT.B  $    -2, #    10
       MOV.I  *     7, @    -3
       JMZ.A  $    -4, $    -6
       JMP.B  $     3, }     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       JMP.B  #     0, <    -2
       JMP.B  #     1, {     1
       DAT.F  #    -1, <     0
