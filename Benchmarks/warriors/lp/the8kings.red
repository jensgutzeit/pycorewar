;redcode-lp
;name The 8 Kings
;author Christian Schmidt
;assert 1

       ORG      START
START  MOV.I  $    30, $  1032
       MOV.I  $    28, <    -1
       MOV.I  $    26, <    -2
       SPL.B  @    -3, $     0
       MOV.I  $    26, $  3032
       MOV.I  $    24, <    -1
       MOV.I  $    22, <    -2
       SPL.B  @    -3, $     0
       MOV.I  $    22, $ -2968
       MOV.I  $    20, <    -1
       MOV.I  $    18, <    -2
       SPL.B  @    -3, $     0
       MOV.I  $    18, $  -968
       MOV.I  $    16, <    -1
       MOV.I  $    14, <    -2
       SPL.B  @    -3, $     0
       MOV.I  $    14, $  2016
       MOV.I  $    12, <    -1
       MOV.I  $    10, <    -2
       SPL.B  @    -3, $     0
       MOV.I  $    10, $ -3984
       MOV.I  $     8, <    -1
       MOV.I  $     6, <    -2
       SPL.B  @    -3, $     0
       MOV.I  $     6, $ -1984
       MOV.I  $     4, <    -1
       MOV.I  $     2, <    -2
       SPL.B  @    -3, $     0
       MOV.I  <    21, $  2235
       SUB.F  $     1, $    -1
       JMP.B  $    -2, < -2234
