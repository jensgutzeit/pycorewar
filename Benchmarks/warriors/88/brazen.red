;redcode
;name Brazen hussy
;author Steve Gunnell
;assert 1

       MOV    #    20, $     7
       ADD    $    12, @     5
START  CMP    $  1538, $  1554
       SLT    #    33, @     3
       DJN    $    -3, <  1715
       MOV    $     7, @    -3
       MOV    $     4, <    -4
       DJN    $    -1, #    20
       ADD    #    20, @    -2
       JMZ    $    -9, $   -11
       SPL    $     0, <   -28
       MOV    $     2, <    -4
       JMP    $    -1, <   -30
       DAT    <   -29, <   -29
       END      START
