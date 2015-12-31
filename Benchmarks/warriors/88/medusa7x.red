;redcode
;name Medusa's v7X
;author W. Mintardjo
;assert 1

       MOV    #    13, $     7
       SUB    $    12, @     5
START  CMP    $  2691, $  2703
       SLT    #    26, @     3
       DJN    $    -3, <  2864
       MOV    $     7, @    -3
       MOV    $     4, <    -4
       DJN    $    -1, #    13
       ADD    #    13, @    -2
       JMZ    $    -9, $   -11
       SPL    $     0, <   -27
       MOV    $     2, <    -4
       JMP    $    -1, <   -27
       DAT    <   -28, <   -28
       END      START
