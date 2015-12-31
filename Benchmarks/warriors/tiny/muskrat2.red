;redcode-tiny
;name Muskrat II
;author John Metcalf
;assert 1

       ORG      START
       MOV.I  $   -35, $     1
       MOV.I  *     1, @     1
       MOV.I  $    -2, *   -33
       SUB.AB #    70, $    -1
       DJN.B  $    -3, #    79
       SPL.B  #     0, $     0
       MOV.I  $     2, >    -3
       DJN.F  $    -1, >    -4
       DAT.F  $     1, $     8
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
START  MOV.I  <   113, {   134
       MOV.I  <   134, {   154
       MOV.I  <   154, {   167
       MOV.I  <   167, {   187
       MOV.I  <   187, {   210
       MOV.I  <   210, {   230
       MOV.I  <   230, {   253
       DJN.F  $   -18, <   273
