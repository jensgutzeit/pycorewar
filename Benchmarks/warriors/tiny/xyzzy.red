;redcode-tiny
;name Xyzzy
;author John Metcalf
;assert 1

       ORG      START
       SPL.B  #   108, >  -108
       MOV.I  <  -216, {   216
       ADD.X  $    -2, $    -1
       DJN.F  $    -2, <  -243
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
START  MOV.I  >  -137, {  -158
       MOV.I  >  -180, {  -201
       MOV.I  >  -223, {  -244
       SPL.B  $     1, >   -98
       SPL.B  $   -12, }   328
       SPL.B  $     1, {   306
       MOV.I  {     2, <     2
       DJN.F  @     1, <   283
       SPL.B  @     4, >    64
       MOV.I  }    -1, >    -1
       MOV.I  {    -2, {     1
       DJN.F  *  -355, {   355
