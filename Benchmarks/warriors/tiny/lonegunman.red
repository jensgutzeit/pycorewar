;redcode-tiny
;name Lone Gunman
;author John Metcalf
;assert 1

       ORG      START
       SUB.B  $     2, #    39
       JMZ.F  $    -1, @    -1
       SPL.B  #     5, <   -12
       MOV.I  *    -3, >    -3
       DJN.F  $    -1, <   -11
       DAT.F  $     5, $     6
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
START  MOV.I  <  -226, {  -263
       MOV.I  <  -263, {  -300
       MOV.I  <  -300, {  -337
       MOV.I  <  -337, {  -374
       MOV.I  <  -374, {   389
       MOV.I  <   389, {   352
       DJN.F  $   -17, <  -197
