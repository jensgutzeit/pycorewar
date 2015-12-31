;redcode
;name Illusion
;author Randy Graham
;assert 1

       DAT    <  2667, #   -10
       DAT    #     0, #     0
       ADD    #  3044, $     3
START  JMZ    $    -1, @     2
       ADD    $     1, $     5
       SPL    $     0, #  3044
       MOV    @     4, <     3
       JMP    $    -1, <   -11
       SPL    $    -1, $   -10
       DAT    #     0, #     0
       DAT    #     0, #     1
       SPL    $    -1, $    -2
       END      START
