;redcode-tiny
;name Windkeeper
;author Christian Schmidt
;assert 1

       ORG      START
START  MOV.I  $    15, $   137
       SPL.B  $     1, <   397
       SPL.B  $     1, <    46
       MOV.I  <     2, {     2
       MOV.I  <     2, {     2
       SPL.B  $   -93, $    10
       JMP.B  $  -197, $    14
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       SPL.B  #  -202, >   202
       MOV.I  {  -202, {   203
       ADD.F  $    -2, $    -1
       DJN.F  @     0, {    -2
       MOV.I  #   267, *     0
       SPL.B  #     0, <  -114
       ADD.A  #   267, $     1
       SPL.B  $    62, <  -310
       DJN.F  $   100, <  -294
