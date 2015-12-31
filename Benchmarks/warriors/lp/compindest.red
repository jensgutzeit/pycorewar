;redcode-lp
;name Completely Indestructible v1.0
;author Ian Sutton
;assert 1

       ORG      START
START  MOV.AB #     7, #     0
       MOV.I  <    -1, {     2
       JMN.B  $    -1, @    -2
       SPL.B  $ -3237, $     1
       ADD.A  #  3337, $    -1
       JMP.B  $    -5, $     1
       DAT.F  $     1, $     1
