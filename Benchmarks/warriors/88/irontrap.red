;redcode
;name Iron Trap
;author Wayne Sheppard
;assert 1

START  ADD    $    16, @     5
       CMP    $    97, $    -1
       SLT    #    20, @     3
       DJN    $    -3, < -1000
       MOV    $    10, @    -3
       MOV    $     7, <    -4
       SUB    $     9, @    -1
       CMP    @    -6, $    10
       JMN    $    -7, $    -8
       MOV    $     3, <    -8
       ADD    #  2668, @    -1
       DJN    $    -2, #   100
       SPL    $     0, <   -97
       MOV    $     2, <    -3
       JMP    $    -1, $     0
       DAT    <   -98, <   -99
       DAT    <   196, <   196
       MOV    $     0, $  2667
       END      START
