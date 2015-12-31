;redcode-tiny
;name whispered in a dream
;author John Metcalf
;assert 1

       ORG      START
       SPL.B  #  -132, >   132
       MOV.I  >  -264, $   265
       ADD.F  $    -2, $    -1
       DJN.F  $    -2, >   129
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
START  SPL.B  $    -8, {   340
       MOV.I  $    10, *     4
       SPL.B  $     4, {  -308
       SPL.B  $     6, {  -261
       SPL.B  @     4, {  -396
       DJN.F  $    37, }     0
       SPL.B  $    -2, {  -363
       SPL.B  @     3, {   224
       DJN.F  $   301, }     0
       SPL.B  @    -4, {  -344
       DJN.F  $  -234, }     0
       MOV.I  #   133, $   268
