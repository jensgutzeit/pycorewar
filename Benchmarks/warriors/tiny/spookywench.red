;redcode-tiny
;name Spooky Wench
;author John Metcalf
;assert 1

       ORG      START
       SPL.B  #     0, $     0
       MOV.I  $     3, @     2
       ADD.AB #    59, $     1
       DJN.F  $    -2, *    57
       DAT.F  $     1, >     1
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
START  SPL.B  $    -8, {  -301
       MOV.I  $    10, *     4
       SPL.B  $     4, {  -237
       SPL.B  $     6, {   279
       SPL.B  @     4, {  -352
       DJN.F  $   133, }     0
       SPL.B  $    -2, {  -194
       SPL.B  @     3, {  -216
       DJN.F  $   397, }     0
       SPL.B  @    -4, {    75
       DJN.F  $  -138, }     0
       MOV.I  #   268, *     1
