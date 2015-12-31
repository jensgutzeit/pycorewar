;redcode-tiny
;name Dirty Dog
;author Christian Schmidt
;assert 1

       ORG      START
START  SPL.B  $     2, <   -44
       SPL.B  $     1, <    68
       SPL.B  $     1, <   -37
       MOV.I  <     1, {     1
       SPL.B  $  -305, $    18
       MOV.I  <     1, {     1
       JMP.B  $   183, $     7
       SPL.B  @     0, <    72
       MOV.I  }    -1, >    -1
       SPL.B  @     0, <   229
       MOV.I  }    -1, >    -1
       MOV.I  #     1, <     1
       DJN.F  $    -2, <   177
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       SPL.B  #  -105, >   105
       MOV.I  >  -210, $   211
       ADD.F  $    -2, $    -1
       DJN.F  $    -2, >   102
