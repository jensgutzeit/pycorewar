;redcode-tiny
;name Evasion
;author Christian Schmidt
;assert 1

       ORG      START
       ADD.F  $    11, $     3
START  SNE.I  *     2, @     2
       ADD.F  $     9, $     1
       SNE.I  $   381, $   371
       DJN.F  $    -4, <  -226
       SUB.AB #    21, $    -2
       MOV.I  <     2, <    -3
       DJN.B  $    -1, #     5
       SPL.B  @    -5, $    12
       MOV.I  <    -1, <    -6
       MOV.I  <    -2, <    -7
       DAT.F  #   164, <   164
       DAT.F  $     0, $     0
       DAT.F  $   204, }    24
       DAT.F  >  -213, {    13
       SPL.B  #  -213, $    13
       MOV.I  @     3, >    -3
       MOV.I  @     2, >    -4
       MOV.I  @     1, >    -5
       DJN.F  $    -3, {    -4
