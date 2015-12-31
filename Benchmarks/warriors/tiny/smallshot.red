;redcode-tiny
;name SmallShot
;author Roy van Rijn
;assert 1

       ORG      START
       ADD.F  $     3, $     1
START  SNE.I  $   210, $   218
       DJN.F  $    -2, @    -1
       SPL.F  #    29, <    29
       MOV.I  @     2, }    -3
       DJN.F  $    -1, <    -4
       DAT.F  #     8, }     1
       SPL.B  #     7, {   360
