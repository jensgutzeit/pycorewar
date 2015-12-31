;redcode-tiny
;name Sneaky Spike 2
;author Roy van Rijn
;assert 1

       ORG      START
       ADD.F  $     3, @     5
START  SNE.I  }   -65, >   -72
       DJN.F  $    -2, <   173
       SPL.B  #   357, {   357
       MOV.I  @     2, >    -3
       MOV.I  @     1, >    -4
       DJN.F  $    -2, {     2
       DAT.F  }  -181, }    11
       SPL.B  #  -181, }    12
