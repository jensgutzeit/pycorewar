;redcode-tiny
;name Sunbeam Scan
;author Simon Wainwright
;assert 1

       ORG      START
       MOV.I  $     6, >    -5
       MOV.I  $     5, >    -6
       SEQ.I  }   135, }   130
START  MOV.AB $    -1, @    -3
       ADD.F  $     2, $    -2
       DJN.B  $    -5, #    79
       SPL.B  #   130, }   130
       MOV.I  $     2, >   -12
       DJN.F  $    -1, >   -13
       DAT.F  <   267, $    17
