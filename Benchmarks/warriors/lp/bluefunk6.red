;redcode-lp
;name Blue Funk 6
;author Steven Morrell
;assert 1

       ORG      START
       DAT.F  $     0, $     7
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     1
       DAT.F  $     0, $     0
       SPL.B  # -3044, <  3044
START  MOV.I  > -3044, $  3035
       ADD.F  $    -2, $    -1
       DJN.F  $    -2, <  -453
       JMP.B  $    -3, $     0
