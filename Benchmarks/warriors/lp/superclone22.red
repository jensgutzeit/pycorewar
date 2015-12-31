;redcode-lp
;name Super clone 2.2
;author Compudemon
;assert 1

       ORG      START
START  MOV.I  }    20, >    20
       MOV.I  }    19, >    19
       MOV.I  }    18, >    18
       MOV.I  }    17, >    17
       MOV.I  }    16, >    16
       MOV.I  }    15, >    15
       MOV.I  }    14, >    14
       MOV.I  }    13, >    13
       MOV.I  }    12, >    12
       MOV.I  }    11, >    11
       MOV.I  }    10, >    10
       MOV.I  }     9, >     9
       MOV.I  }     8, >     8
       MOV.I  }     7, >     7
       MOV.F  $     7, $     6
       JMP.B  $   -15, }    -1
       SPL.B  $  1103, $     1
       SPL.B  $  1202, $     1
       SPL.B  $  1301, $     1
       JMP.B  $  1400, $     1
       DAT.F  $     6, $  1096
       DAT.F  $     6, $  1196
       DAT.F  $     6, $  1296
       DAT.F  $     6, $  1396
       DAT.F  $     5, $   -19
       JMP.B  $    15, $     1
       DAT.F  $    14, $  3904
       MOV.A  #    14, $    -1
       ADD.AB #   743, $    -2
       MOV.I  {    -3, <    -3
       MOV.I  {    -4, <    -4
       JMN.A  $    -2, *    -5
       SPL.B  $    -5, >    -6
       JMP.B  >    -7, $     0
       DAT.F  $     1, $     1
       DAT.F  $     1, $     1
       DAT.F  $     1, $     1
       DAT.F  $     1, $     1
       DAT.F  $     1, $     1
       DAT.F  $     1, $     1
