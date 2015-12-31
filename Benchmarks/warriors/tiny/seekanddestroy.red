;redcode-tiny
;name Seek and Destroy
;author Sascha Zapf
;assert 1

       ORG      START
       DAT.F  $    79, $    60
       DAT.F  #  -115, $    22
       SPL.B  #  -115, $    22
       MOV.I  @     3, >    -3
       MOV.I  @     2, >    -4
       MOV.I  @     1, >    -5
       DJN.F  $    -3, {    -4
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $    26, $    26
       ADD.F  $    -1, $   -14
START  SNE.I  @   -15, *   -15
       ADD.F  $    -3, $   -16
       SNE.I  @   -17, *   -17
       DJN.F  $    -4, <  -112
       JMP.B  $   -17, <   -19
