;redcode-lp
;name Elusive
;author John Metcalf
;assert 1

       ORG      START
       MOV.AB #     8, #     8
       ADD.A  #  3359, $     5
START  MOV.I  <    -2, {     4
       MOV.I  <    -3, {     3
       JMN.B  $    -2, $    -4
       SPL.B  $    -5, {  4000
       JMZ.B  $  3039, *     0
       DAT.F  }     1, >     1
