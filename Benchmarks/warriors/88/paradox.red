;redcode
;name Paradox
;author Mike Nonemacher
;assert 1

       MOV    $     0, $  2667
       MOV    $     0, $  2668
START  SPL    $    47, $     0
       SPL    $    11, $     0
       MOV    $    -3, $   229
       MOV    $    -4, $   229
       MOV    $    -5, $   229
       SPL    $     1, $     0
       SPL    $     1, $     0
       MOV    $    -1, $     0
       SPL    $     2, $     0
       JMP    @     0, $   222
       ADD    #  2668, $    -1
       DAT    #     0, #     0
       SPL    $    10, $     0
       MOV    $   -14, $  2885
       MOV    $   -15, $  2885
       SPL    $     1, $     0
       SPL    $     1, $     0
       MOV    $    -1, $     0
       SPL    $     2, $     0
       JMP    @     0, $  2879
       ADD    #  2668, $    -1
       DAT    #     0, #     0
       SPL    $     9, $     0
       MOV    $   -24, $ -2458
       SPL    $     1, $     0
       SPL    $     1, $     0
       MOV    $    -1, $     0
       SPL    $     2, $     0
       JMP    @     0, $ -2463
       ADD    #  2668, $    -1
       DAT    #     0, #     0
       SPL    $     8, $     0
       SPL    $     1, $     0
       SPL    $     1, $     0
       MOV    $    -1, $     0
       SPL    $     2, $     0
       JMP    @     0, $   196
       ADD    #  2668, $    -1
       DAT    #     0, #     0
       MOV    $   -41, $   190
       SPL    $     1, $     0
       SPL    $     1, $     0
       MOV    $    -1, $     0
       SPL    $     2, $     0
       JMP    @     0, $   185
       ADD    #  2667, $    -1
       DAT    #     0, #     0
       SPL    $     1, $     0
       SPL    $     1, $     0
       SPL    $     1, $     0
       SPL    $    30, $     0
       SPL    $    20, $     0
       SPL    $     1, $     0
       MOV    #    16, $    16
       MOV    <    -1, <     1
       SPL    @     0, # -2395
       MOV    $     4, <    -1
       MOV    $     4, <    -2
       MOV    $     4, <    -3
       JMZ    @     0, $    -6
       DAT    <  2667, < -2666
       MOV    $     0, <     1
       JMP    $    50, <  2667
       MOV    $    -3, <   -60
       SPL    $    -1, <   -60
       JMP    $    -2, <   -61
       JMP    $    -3, <  2667
       JMP    $    -4, <  2667
       JMP    $    -5, <  2667
       DAT    #     0, #     0
       MOV    #     8, $     8
       MOV    <    -1, <     1
       SPL    @     0, #  1067
       MOV    #     8, $   -98
       SUB    <  -107, $   -99
       MOV    $     3, <  -100
       MOV    $     2, <  -101
       JMZ    @     0, $    -7
       DAT    #     0, #     0
       MOV    #     8, $     8
       MOV    <    -1, <     2
       JMN    $     5, $    -2
       SPL    @     0, # -2995
       MOV    $     3, <    -1
       MOV    $     2, <    -2
       JMZ    @     0, $    -6
       DAT    <  2667, < -2666
       END      START