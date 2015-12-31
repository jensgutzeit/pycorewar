;redcode-tiny
;name 3p(c)
;author Sascha Zapf
;assert 1

       ORG      START
       DAT.F  {    -1, }    -2
START  MOV.I  $    -1, <  -105
       MOV.I  <  -105, <    67
       MOV.I  <    67, <   242
       MOV.I  <   242, <  -332
       MOV.I  <  -332, <   237
       MOV.I  <   237, <  -296
       MOV.I  <  -296, <  -327
       MOV.I  <  -327, <  -383
       SPL.B  $     2, }  -292
       SPL.B  $     1, }   103
       SPL.B  $     1, }   122
       MOV.I  {     2, {     1
       JMP.B  $    91, }  -312
       SPL.B  @     6, <    38
       MOV.I  }    -1, >    -1
       SPL.B  @     0, <   125
       MOV.I  }    -1, >    -1
       MOV.I  #     1, <     1
       DJN.B  $    -2, #   355
