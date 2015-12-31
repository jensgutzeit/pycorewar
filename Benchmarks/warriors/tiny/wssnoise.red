;redcode-tiny
;name White Spikes Sneaky Noise
;author Zul Nadzri
;assert 1

       ORG      START
       ADD.F  $     3, @     4
START  SNE.I  }   -67, $   -73
       DJN.F  $    -2, {    50
       SPL.B  #   -26, {   -26
       MOV.I  @     3, >    -3
       MOV.I  @     2, >    -3
       MOV.I  @     1, >    -4
       DJN.X  $    -2, {     2
       DAT.F  #  -181, }     8
       SPL.B  #   -25, $    12
