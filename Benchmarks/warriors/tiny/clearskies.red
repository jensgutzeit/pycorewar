;redcode-tiny
;name clear skies
;author Simon Wainwright
;assert 1

       ORG      START
START  MOV.I  $    19, *     4
       MOV.I  {    -1, {     3
       MOV.I  {    -2, {     2
       MOV.I  {    -3, {     1
       SPL.B  $   196, }  -279
       MOV.I  }   -31, {   -44
       MOV.I  }   -58, {   -71
       MOV.I  }   -85, {   -98
       MOV.I  }  -112, {  -125
       MOV.I  }  -139, {  -152
       MOV.I  }  -166, {  -179
       MOV.I  }  -193, {  -206
       MOV.I  }  -220, {  -233
       MOV.I  }  -247, {  -260
       MOV.I  }  -274, {  -287
       JMN.B  }   400, }   220
       SPL.B  #     0, $     0
       MOV.I  $     2, >    -5
       DJN.F  $    -1, >    -6
       DAT.F  <     1, <    13
