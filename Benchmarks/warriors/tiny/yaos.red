;redcode-tiny
;name Y.A.O.S.
;author Ben Ford
;assert 1

       ORG      START
       DAT.F  <    -1, {     7
       SPL.B  #    -2, {     8
       SPL.B  #   307, {   314
       MOV.I  @     2, >    -4
       MOV.I  @     1, >    -5
       DJN.B  $    -2, {    -3
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       ADD.F  $     5, $    -8
START  SNE.I  *    -9, @    -9
       DJN.B  $    -2, #   161
       MOV.AB @    -2, $   -14
       JMP.B  @    -3, <   -15
       DAT.F  #   310, #   310
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
