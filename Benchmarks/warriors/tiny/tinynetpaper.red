;redcode-tiny
;name Tiny Netpaper
;author David Houston
;assert 1

       ORG      START
START  MOV.I  <     6, {     6
       SPL.B  $     2, <  -188
       SPL.B  $     2, <   -60
       SPL.B  $     1, <  -162
       MOV.I  <     2, {     2
       MOV.I  <     1, {     1
       JMP.B  $   304, $    12
       SPL.B  $     1, <   -72
       SPL.B  @     0, {    66
       MOV.I  }    -1, >    -1
       SPL.B  @     0, <   171
       MOV.I  }    -1, >    -1
       MOV.I  $     5, >  -358
       MOV.I  $     4, <   170
       ADD.AB #    -5, $   340
       MOV.I  {    -5, <     1
       DJN.F  @     0, >   -99
       DAT.F  >   267, >   138
