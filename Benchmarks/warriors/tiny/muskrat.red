;redcode-tiny
;name Muskrat
;author John Metcalf
;assert 1

       ORG      START
       MOV.I  $   165, $     1
       MOV.I  *     1, @     1
       MOV.I  $    -2, *   167
       ADD.AB #   330, $    -1
       DJN.B  $    -3, #    79
       SPL.B  #     0, $     0
       MOV.I  $     2, >    -9
       DJN.F  $    -1, >   -10
       DAT.F  >     1, $    15
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
START  MOV.I  <   288, {   309
       MOV.I  <   309, {   329
       MOV.I  <   329, {   342
       MOV.I  <   342, {   362
       MOV.I  <   362, {   385
       MOV.I  <   385, {  -395
       MOV.I  <  -395, {  -372
       DJN.F  $   -18, <  -352
