;redcode
;name Iron Gate
;author Wayne Sheppard
;assert 1

START  ADD    $    12, @     5
       CMP    $    97, $    -1
       SLT    #    14, @     3
       DJN    $    -3, < -1000
       MOV    $     6, @    -3
       MOV    $     3, <    -4
       SUB    $     5, @    -1
       JMN    $    -6, $    -7
       SPL    $     0, <   -97
       MOV    $     2, <    -2
       JMP    $    -1, $     0
       DAT    <   -98, <   -99
       DAT    #   196, #   196
       END      START
