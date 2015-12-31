;redcode-tiny
;name dark skies
;author Simon Wainwright
;assert 1

       ORG      START
       DAT.F  $    11, $     6
       DAT.F  $     1, $     8
       SPL.B  #   -70, $    10
       MOV.I  @     2, >    -3
       MOV.I  @     1, >    -4
       DJN.F  $    -2, {    -3
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       SPL.B  #   271, <   272
START  SEQ.I  $   271, $   263
       ADD.F  $    -1, }     2
       ADD.F  $    -3, *   -13
       DJN.F  *   -14, }    -3
