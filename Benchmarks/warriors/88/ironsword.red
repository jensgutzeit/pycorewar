;redcode
;name Iron Sword
;author Wayne Sheppard
;assert 1

START  SUB    $    11, @     5
       CMP    $    34, $     0
       SLT    #    20, @     3
       DJN    $    -3, $     4
       MOV    $     3, @    -3
       MOV    $     3, <    -4
       SUB    $     3, $    -5
       JMN    @    -1, #   -20
       SPL    $     0, $     0
       MOV    @   -34, <   -35
       SUB    #  2666, @    -1
       DAT    <   -68, <   -68
       END      START
