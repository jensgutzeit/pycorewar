;redcode-lp
;name Toxic Club
;author Christian Schmidt
;assert 1

       ORG      START
       DAT.F  $    19, $    19
       JMP.B  #  1443, #  1443
       SPL.B  #     0, $     0
       MOV.I  $    -2, >    -4
       MOV.I  *    -1, >    -5
       DJN.A  $    -2, $     7
       SUB.F  $    -5, @     3
START  SNE.I  $ -2887, $ -2893
       SUB.F  $    -7, $    -1
       SEQ.I  *    -2, @    -2
       SLT.A  #    20, $    -3
       DJN.B  $    -5, #  1400
       SUB.BA #     0, #    -7
       MOV.AB @    -4, @   -10
       JMN.B  *   -10, $    -3
       DJN.A  <    -2, {    -1
