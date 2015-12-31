;redcode-lp
;name Plywood
;author Franz
;assert 1

       ORG      START
START  SPL.B  $     1, >  -254
       MOV.I  >     1, }     8
       MOV.I  $    -2, #    17
       MOV.I  <     4, <     6
       SPL.B  $     1, >  -381
       MOV.I  <     2, <     4
       DJN.B  >     2, #     1
       SPL.B  *     1, #    12
       DIV.F  #  2338, #  2337
       DIV.F  #  2301, #  2345
       DAT.F  >  2667, >    25
       MOV.I  $   -35, <     1
       MOV.I  >   467, {   623
       ADD.F  $     3, $    -1
       MOV.I  {    -3, {    -2
       JMZ.F  $    -4, }    -4
       SPL.B  #   466, >   466
       MOV.I  $    -7, >   -22
       DJN.F  $    -1, >   -23
       DAT.F  $     0, $     0
       DAT.F  $   156, <    10
