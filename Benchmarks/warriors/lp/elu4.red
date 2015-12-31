;redcode-lp
;name paper
;author Simon Wainwright
;assert 1

       ORG      START
       MOV.AB #     6, #     6
       ADD.A  #  1113, $     4
START  MOV.I  <    -2, {     3
       JMN.B  $    -1, $    -3
       SPL.B  @    -4, }    -3
       JMZ.B  $  -653, *     0
