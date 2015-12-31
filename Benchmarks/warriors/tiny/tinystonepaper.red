;redcode-tiny
;name Tiny Stone-paper
;author Lukasz Grabun
;assert 1

       ORG      START
START  MOV.I  $    15, $  -263
       SPL.B  $     1, $    14
       SPL.B  $     1, $     0
       MOV.I  <    -2, <    -3
       DJN.B  @    -4, #     4
       SPL.B  $     1, {     0
       SPL.B  *     2, {     0
       SPL.B  @     0, <   104
       MOV.I  }    -1, >    -1
       MOV.I  {    -2, <     1
       DJN.F  *    57, >    60
       SPL.B  #     0, #     0
       MOV.I  $     3, @     2
       ADD.AB #  -214, $     1
       DJN.F  $    -2, {  -217
       DAT.F  {     4, >     1
