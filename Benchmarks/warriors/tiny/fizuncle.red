;redcode-tiny
;name fizUncle
;author Christian Schmidt
;assert 1

       ORG      START
START  SPL.B  $     2, <   -64
       SPL.B  $     2, <  -162
       SPL.B  $     1, <  -278
       SPL.B  @     0, <    66
       MOV.I  }    -1, >    -1
       MOV.I  }    -2, >    -2
       SPL.B  @     0, <  -261
       MOV.I  }    -1, >    -1
       MOV.I  }   322, <  -364
       JMP.B  $    -2, <  -305
