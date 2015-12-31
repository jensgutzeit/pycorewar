;redcode-tiny
;name Table Scan
;author John Metcalf
;assert 1

       ORG      START
START  SNE.I  *    19, @    19
       DJN.F  {     0, }     0
       ADD.X  @    -2, }     0
       SPL.B  #     0, $     0
       MOV.I  @     2, >    -4
       MOV.I  @     1, >    -5
       DJN.F  $    -2, {     2
       DAT.F  $   -20, $     9
       SPL.B  #  -158, $    11
       SPL.B  #    51, <    71
       SPL.B  #    90, <   110
       SPL.B  #   129, <   149
       SPL.B  #   168, <   188
       SPL.B  #   207, <   227
       SPL.B  #   246, <   266
       SPL.B  #   285, <   305
       SPL.B  #   324, <   344
       SPL.B  #   363, <   383
       SPL.B  #  -398, <  -378
       SPL.B  #  -359, <  -339
