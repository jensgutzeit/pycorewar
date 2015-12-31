;redcode-lp
;name Raidar II
;author Unknown :-)
;assert 1

       ORG      START
       MOV.AB #    28, #    28
       MOV.F  $    17, $    16
START  MOV.I  $    17, >    15
       MOV.I  $    16, {    14
       DJN.B  @     9, @     6
       MOV.AB @     5, @     5
       MOV.F  $    12, @    -4
       MOV.I  *    -5, >   388
       SNE.I  >     9, {     9
       DJN.B  $    -2, $    -9
       JMZ.B  $    -5, $   -10
       MOV.AB #    20, $   -11
       MOV.A  #   526, @   -10
       SNE.I  <     4, *   -11
       MOV.A  #  -540, @   -12
       MOV.I  <   -15, {     2
       JMN.B  $    -1, @    -6
       JMP.B  $  -202, <   188
       DAT.F  $  -202, $   188
       DAT.F  $     1, $     1
       DAT.F  $    19, $    19
