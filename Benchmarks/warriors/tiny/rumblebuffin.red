;redcode-tiny
;name Rumblebuffin
;author John Metcalf
;assert 1

       ORG      START
       SPL.B  #   -52, >    52
       MOV.I  >   -52, $    53
       ADD.F  $     2, $    -1
       DJN.F  @    -1, >    49
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
START  MOV.I  {  -191, {  -222
       MOV.I  {  -191, {    77
       SPL.B  $     1, {    94
       SPL.B  $     1, {   134
       SPL.B  {   -11, {  -331
       MOV.I  {     2, {     1
       DJN.F  *   243, {  -136
       SPL.B  @     4, <   105
       MOV.I  }    -1, >    -1
       MOV.I  {    -2, <     1
       DJN.F  *   -44, >   -41
