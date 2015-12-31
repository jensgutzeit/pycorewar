;redcode-tiny
;name One Shot 'T'
;author Philip Kendall
;assert 1

       ORG      START
START  SPL.A  $     1, >  -100
       MOV.I  >     5, >     6
       MOV.I  >     4, >     5
       MOV.I  >     3, >     4
       MOV.I  >     2, >     3
       MOV.I  >     1, >     2
       MOD.A  #     1, #     4
       SPL.A  $   114, $   111
       DIV.F  $    -1, $    -1
       DAT.F  $     0, $     0
       DAT.F  $   -20, >    14
       SPL.A  #     0, >    14
       SUB.F  $     4, $     1
       SNE.I  $    28, $    40
       DJN.F  $    -2, <  -150
       MOV.AB $    -2, $    -8
       SPL.A  #   -24, >   -24
       MOV.I  @     2, >   -10
       MOV.I  @     1, >   -11
       DJN.B  $    -2, {    -8
