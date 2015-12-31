;redcode-lp
;name Romulan Warbird
;author Philip Kendall
;assert 1

       ORG      START
       DAT.F  $   196, $   196
       ADD.F  $    -1, @     5
START  SEQ.I  $   195, $    97
       SLT.AB #    11, @     3
       DJN.F  $    -3, < -1308
       MOV.I  $     6, @    -3
       MOV.I  $     3, <    -4
       ADD.F  $     6, $    -5
       JMN.A  @    -2, $    -8
       SPL.A  #     0, }   -11
       MOV.I  $     2, >   -12
       JMP.A  $    -1, }   -13
       DAT.F  >  2667, >    17
       DAT.F  $    98, $    99
