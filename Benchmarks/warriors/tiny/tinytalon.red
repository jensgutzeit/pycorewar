;redcode-tiny
;name Tiny Talon
;author Robert Macrae
;assert 1

       ORG      START
       SPL.B  #    -4, $    83
START  MOV.I  $    -1, >    -1
       ADD.F  $     4, $     1
       SEQ.I  }     4, $     0
       MOV.B  $    -1, @    -3
       DJN.B  $    -4, #    99
       SPL.B  #   -24, >   -24
       MOV.I  $     2, >   -12
       DJN.F  $    -1, >   -13
       DAT.F  $     1, #    18
