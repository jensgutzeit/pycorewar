;redcode-tiny
;name Little Toy
;author Christian Schmidt
;assert 1

       ORG      START
       SPL.B  $     2, <  -153
       SPL.B  $     2, {  -306
       SPL.B  $     1, <    86
       SPL.B  @     0, <  -342
       MOV.I  }    -1, >    -1
       MOV.I  }    -2, >    -2
       SPL.B  @     0, <   130
       MOV.I  }    -1, >    -1
       MOV.I  #     1, {     1
       MOV.I  #  -190, <     1
       DJN.F  $    -3, <  -170
START  MOV.I  {  -132, <     8
       MOV.I  {     8, <   288
       MOV.I  {   148, <  -232
       MOV.I  {   288, <    48
       MOV.I  {  -372, <   328
       MOV.I  {  -232, <  -192
       MOV.I  {   -92, <    88
       MOV.I  {    48, <   368
       DJN.F  $   -19, <   377
