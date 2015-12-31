;redcode-lp
;name Golden Gate 3.1
;author Franz
;assert 1

       ORG      START
START  MOV.I  }    22, >    22
       MOV.I  }    21, >    21
       MOV.I  }    20, >    20
       MOV.I  }    19, >    19
       MOV.I  }    18, >    18
       MOV.I  }    17, >    17
       MOV.I  }    16, >    16
       MOV.I  }    15, >    15
       MOV.F  #   333, $  2277
       MOV.I  $    15, $  2278
       MOV.I  $    15, $  2395
       SPL.B  $     5, >  1000
       SPL.B  $     7, >  3000
       SPL.B  $     2, >  4000
       JMP.B  }     9, > -1500
       JMP.B  }     8, > -2500
       SPL.B  $     2, > -1000
       JMP.B  }     6, >  1500
       JMP.B  }     5, >  3500
       SPL.B  $     2, > -3000
       JMP.B  }     3, > -3500
       JMP.B  $  2340, > -2000
       DAT.F  $     4, $  2339
       DAT.F  $  2338, $     0
       DAT.F  $  2366, $    -1
       DAT.F  >  2667, >    50
       MOV.I  $   -74, <     1
       MOV.I  >  2367, { -3267
       ADD.F  $     3, $    -1
       MOV.I  >   -77, {    -2
       JMZ.F  $    -4, <   -78
       SPL.B  #  -904, >  -904
       MOV.I  $    38, >   -42
       DJN.F  $    -1, >   -43
