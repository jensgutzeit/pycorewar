;redcode-tiny
;name Cream and Chocolate
;author LAchi
;assert 1

       ORG      START
       JMN.F  $    11, @     9
       ADD.F  #     8, $     8
       JMZ.F  $    -2, *     7
       DJN.F  $     8, $     6
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $    34, $  -362
       DAT.F  $    10, *  -390
       SPL.F  #    20, $    -5
       MOV.I  $    -1, }    -3
       MOV.I  $    -2, >    -4
       DJN.A  $    -2, <    -3
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
START  MOV.I  >   252, {   344
       MOV.I  >  -357, {  -253
       DJN.F  $   -19, {  -161
