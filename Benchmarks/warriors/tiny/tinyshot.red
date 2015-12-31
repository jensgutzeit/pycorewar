;redcode-tiny
;name Tinyshot
;author John Metcalf
;assert 1

       ORG      START
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       SUB.F  $     4, $     2
START  MOV.I  $    10, @     1
       SNE.I  $    54, *    36
       DJN.F  $    -3, <   -79
       SPL.B  #   -26, <   -26
       MOV.I  @     3, >    -3
       MOV.I  @     2, >    -4
       MOV.I  @     1, >    -5
       DJN.F  $    -2, {     2
       DAT.F  #   -15, $     8
       SPL.B  #   -15, $     9
       DAT.F  {    11, $     1
