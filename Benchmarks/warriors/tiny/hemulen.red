;redcode-tiny
;name Hemulen
;author John Metcalf
;assert 1

       ORG      START
       MOV.I  $   -61, }   -61
       MOV.I  $     4, @     1
       MOV.I  $    -2, }  -253
       ADD.AB #  -255, $    -1
       DJN.B  $    -3, #   131
       SPL.B  #    61, $     1
       MOV.I  $     2, >    -9
       DJN.F  $    -1, {   -10
       DAT.F  >     1, $    15
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
START  MOV.I  $    -4, {   315
       MOV.I  <   315, {   339
       MOV.I  <   339, {   363
       MOV.I  <   363, {   387
       MOV.I  <   387, {  -389
       MOV.I  <  -389, {  -365
       MOV.I  <  -365, {  -341
       DJN.F  $   -18, <  -317
