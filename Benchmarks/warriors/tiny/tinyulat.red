;redcode-tiny
;name Tiny U-Lat
;author Zul Nadzri
;assert 1

       ORG      START
START  SPL.B  $     1, }   -88
       SPL.B  $     1, }   -66
       MOV.I  $    -1, $     0
       SPL.B  @     0, }   145
       MOV.I  }    -1, >    -1
       SPL.B  @     0, }   345
       MOV.I  }    -1, >    -1
       MOV.I  }   267, <  -266
       MOV.I  {    -3, <     1
       JMP.B  @     0, <  -333
