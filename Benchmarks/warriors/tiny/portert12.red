;redcode-tiny
;name Tiny Porter (+12)
;author Franz
;assert 1

       ORG      START
       DAT.F  $   312, $   312
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       ADD.F  $    -5, $     1
START  SUB.F  $   318, $   306
       JMZ.F  $    -2, @    -1
       SPL.B  #     0, <     0
       MOV.I  $    -1, >    -3
       MOV.I  $     3, >    -9
       DJN.F  $    -1, >   -10
       DAT.F  $     0, $     0
       DAT.F  >  -266, $    20
