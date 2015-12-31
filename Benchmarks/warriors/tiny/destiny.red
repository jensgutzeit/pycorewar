;redcode-tiny
;name desTiny
;author Sascha Zapf
;assert 1

       ORG      START
START  MOV.I  <  -372, $  -258
       MOV.I  <   -99, $    40
       MOV.I  <  -112, $  -319
       MOV.I  <   125, $   327
       MOV.I  <  -117, $  -244
       MOV.I  <   231, $   378
       MOV.I  <   367, $   261
       MOV.I  <  -361, $    84
       MOV.I  <  -158, $    69
       SPL.B  $     2, <  -104
       SPL.B  $     1, <  -202
       SPL.B  $     1, <  -180
       MOV.I  {     2, {     1
       DJN.F  $   183, <   219
       SPL.B  @     6, <   104
       MOV.I  }    -1, >    -1
       SPL.B  #   374, >   272
       MOV.I  {  -240, {   160
       ADD.F  $    -2, $    -1
       DJN.F  @     0, {    -2
