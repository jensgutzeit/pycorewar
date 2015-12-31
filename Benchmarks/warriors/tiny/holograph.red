;redcode-tiny
;name holograph
;author Simon Wainwright
;assert 1

       ORG      START
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $   -30, $    10
       SPL.B  #  -100, $    12
       MOV.I  @     3, >    -4
       MOV.I  @     2, >    -5
       MOV.I  @     1, >    -6
       DJN.F  $    -2, {    -4
       DAT.F  $     0, $     0
       SPL.B  #   -48, <   -48
       SUB.F  $    -1, $     1
START  SNE.I  $   173, $    53
       SUB.F  $    -3, $    -1
       SNE.I  *    -2, @    -2
       DJN.F  $    -4, @    -3
       MOV.F  @    -2, $   -15
       SNE.I  @    -5, <  -197
       ADD.X  #   120, $   -17
       DJN.F  $   -15, <   195
