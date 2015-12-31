;redcode
;name Sixth Sense
;author Wayne Sheppard
;assert 1

START  ADD    $    12, @     5
       CMP    $    75, $    -1
       SLT    #    14, @     3
       DJN    $    -3, < -1000
       MOV    $     6, @    -3
       MOV    $     3, <    -4
       SUB    $     5, @    -1
       JMN    $    -6, $    -7
       SPL    $     0, <   -75
       MOV    $     2, <    -2
       JMP    $    -1, $     0
       DAT    <   -76, <   -77
       DAT    #   152, #   152
       END      START
