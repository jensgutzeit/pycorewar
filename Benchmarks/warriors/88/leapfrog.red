;redcode
;name Leapfrog
;author David Moore
;assert 1

       DAT    <  2667, < -2666
       DAT    <  2663, <  2663
       DAT    <  2663, <  2663
       DAT    <  2663, <  2663
       MOV    $    -4, <  1486
       DJN    $    -1, #    17
       SPL    $     1, <    -9
       DJN    $    -1, <   -10
       JMP    $    -1, $     0
       SPL    $     0, <   -12
       SPL    $     0, <   -13
       MOV    $   -11, <   -12
       DJN    $    -1, <  3850
       JMP    $    -1, $     0
       SUB    @  1467, $  1476
       ADD    $  1466, $  1475
       MOV    $   -16, <  1474
       JMP    $    -1, $     0
       DAT    #     0, #     0
       SPL    $     1, $     0
       MOV    $     1, $     0
       SPL    $     1, $     0
       MOV    #     6, $     0
       MOV    <    -1, <     1
       SPL    @     0, $ -3365
       MOV    $     2, <    -1
       JMZ    $    -4, $    -4
       DAT    <  2667, < -2666
       DAT    #     0, #     0
       SPL    $   -15, $     0
       SPL    $   -11, $     0
       SPL    $   -22, $     0
       JMP    $   -28, $     0
       MOV    #   429, $    21
       JMP    $    16, $     0
       MOV    #   532, $    19
       JMP    $    14, $     1
       MOV    #   635, $    17
       JMP    $    12, $     1
       MOV    #   738, $    15
       JMP    $    10, $     1
       MOV    #   841, $    13
       JMP    $     8, $     1
       MOV    #   944, $    11
       JMP    $     6, $     1
       MOV    #  1047, $     9
       JMP    $     4, $     1
       MOV    #  1150, $     7
       JMP    $     2, $     1
       MOV    #   326, $     5
       CMP    @     4, $   -51
       ADD    #  4000, $     3
       ADD    # -3998, $     2
       MOV    <     3, <     1
       MOV    <     2, @   223
       DJN    $    -2, #     7
       JMP    $    35, $    15
       JMP    $   -58, $     1
       SPL    $     0, $    58
       JMP    $    40, $     1
       SPL    $    -1, $   -40
       JMP    $   -43, $     1
       SPL    $    -1, $    43
       JMP    $    29, $     1
       SPL    $    -1, $   -29
       JMP    $   -28, $     1
       SPL    $    -1, $    28
       JMP    $    18, $     1
       SPL    $    -1, $   -18
       JMP    $   -13, $     1
       JMP    $    -1, $    13
START  CMP    $   206, $ -3794
       JMP    $   -22, $     0
       CMP    $   307, $ -3693
       JMP    $   -25, $     0
       CMP    $   408, $ -3592
       JMP    $   -43, $     0
       CMP    $   509, $ -3491
       JMP    $   -43, $     0
       CMP    $   610, $ -3390
       JMP    $   -43, $     0
       CMP    $   711, $ -3289
       JMP    $   -43, $     0
       CMP    $   812, $ -3188
       JMP    $   -43, $     0
       CMP    $   913, $ -3087
       JMP    $   -43, $     0
       CMP    $  1014, $ -2986
       JMP    $   -43, $     0
       CMP    $  1115, $ -2885
       JMP    $   -43, $     0
       MOV    $     8, <     4
       MOV    $     6, <     3
       MOV    $     4, <     2
       MOV    $     2, <     1
       JMP    @     0, $  1388
       MOV    <     2, $  2369
       ADD    #  2365, $    -1
       JMZ    $    -2, $  1594
       JMP    $ -1453, $     0
       END      START
