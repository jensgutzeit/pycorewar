;redcode-lp
;name Tetraquad
;author David Moore
;assert 1

       ORG      START
START  SPL.B  $     8, $     0
       MOV.AB #   -18, $  2885
       SPL.B  $     1, <    -1
       MOV.I  <     5, <     1
       SPL.B  @     0, $  3003
       JMP.B  @    -1, <     0
       MOV.I  $  -119, <  -119
       JMP.B  $    -1, <  -120
       SPL.B  $     1, $     0
       MOV.AB #    10, #     0
       MOV.I  <    -1, <     3
       MOV.I  <    -2, <     2
       MOV.I  <    -3, <     1
       SUB.AB #  1075, $ -3365
       MOV.I  <    -5, <    -1
       MOV.I  <    -6, <    -2
       SPL.B  @    -3, <   -10
       JMZ.B  @     0, $    -8
