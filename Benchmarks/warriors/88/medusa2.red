;redcode
;name Medusa's v2
;author W. Mintardjo
;assert 1

       SUB    $    12, $     1
START  CMP    $  2691, $  2703
       SLT    #    24, $    -1
       DJN    $    -3, <  2864
       MOV    #    12, $     2
       MOV    $     4, <    -4
       DJN    $    -1, #     0
       ADD    #    12, $    -6
       JMZ    $    -8, $    -9
       SPL    $     0, <   -27
       MOV    $     2, <    -2
       JMP    $    -1, <   -29
       DAT    <   -28, <   -28
       END      START
