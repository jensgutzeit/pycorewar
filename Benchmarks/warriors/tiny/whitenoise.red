;redcode-tiny
;name White Noise (RBv1.5r10)
;author The MicroGP Corewars Collective
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
       SPL.X  #     1, $     1
       MOV.I  @     2, >   145
       SPL.B  #  -330, >   394
       SPL.B  #   238, >   396
       SPL.X  $     1, $     1
       SPL.X  #     1, $     1
       MOV.I  @     2, >   145
       SPL.X  #     1, $     1
       SPL.X  $     1, }     1
       SPL.X  $     1, $     1
