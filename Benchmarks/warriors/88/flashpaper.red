;redcode
;name Flash Paper3.7
;author Matt Hastings
;assert 1

START  SPL    $    89, < -2050
       SPL    $     1, <   440
       SPL    $     1, <   460
       SPL    $    17, <  2113
       SPL    $    25, <  2140
       SPL    $    33, <  2720
       SPL    $    41, <  4000
       SPL    $    49, <  3980
       SPL    $    57, <  3360
       SPL    $    65, <  -970
       MOV    #     8, $     8
       MOV    <    -1, <     2
       MOV    <    -2, <     1
       SPL    @     0, $ -2340
       MOV    <    -1, $  1020
       JMZ    $    -5, $    -5
       MOV    $     0, $    -1
       DAT    #     0, #     1
       DAT    #     0, #     1
       MOV    #     8, $     8
       MOV    <    -1, <     2
       MOV    <    -2, <     1
       SPL    @     0, $ -2177
       MOV    <    -1, $  -740
       JMZ    $    -5, $    -5
       MOV    $     0, $    -1
       DAT    #     0, #     1
       DAT    #     0, #     1
       MOV    #     8, $     8
       MOV    <    -1, <     2
       MOV    <    -2, <     1
       SPL    @     0, $  1000
       MOV    <    -1, $ -3690
       JMZ    $    -5, $    -5
       MOV    $     0, $    -1
       DAT    #     0, #     1
       DAT    #     0, #     1
       MOV    #     8, $     8
       MOV    <    -1, <     2
       MOV    <    -2, <     1
       SPL    @     0, $ -1891
       MOV    <    -1, $  1873
       JMZ    $    -5, $    -5
       MOV    $     0, $    -1
       DAT    #     0, #     1
       DAT    #     0, #     1
       MOV    #     8, $     8
       MOV    <    -1, <     2
       MOV    <    -2, <     1
       SPL    @     0, $  3009
       MOV    <    -1, $  -200
       JMZ    $    -5, $    -5
       MOV    $     0, $    -1
       DAT    #     0, #     1
       DAT    #     0, #     1
       MOV    #     8, $     8
       MOV    <    -1, <     2
       MOV    <    -2, <     1
       SPL    @     0, $ -3168
       MOV    <    -1, $ -1830
       JMZ    $    -5, $    -5
       MOV    $     0, $    -1
       DAT    #     0, #     1
       DAT    #     0, #     1
       MOV    #     8, $     8
       MOV    <    -1, <     2
       MOV    <    -2, <     1
       SPL    @     0, $  1080
       MOV    <    -1, $ -3904
       JMZ    $    -5, $    -5
       MOV    $     0, $    -1
       DAT    #     0, #     1
       DAT    #     0, #     1
       MOV    #     8, $     8
       MOV    <    -1, <     2
       MOV    <    -2, <     1
       SPL    @     0, $ -3040
       MOV    <    -1, $  -195
       JMZ    $    -5, $    -5
       MOV    $     0, $    -1
       DAT    #     0, #     1
       DAT    #     0, #     1
       MOV    #     8, $     8
       MOV    <    -1, <     2
       MOV    <    -2, <     1
       SPL    @     0, $ -3040
       MOV    <    -1, $  -195
       JMZ    $    -5, $    -5
       MOV    $     0, $    -1
       SPL    $     6, < -2030
       SPL    $     5, <   750
       SPL    $     4, < -2630
       SPL    $     3, <  3467
       SPL    $     2, < -1153
       SPL    $     1, <  -453
       SPL    $     0, $     0
       MOV    <  2860, $ -2858
       ADD    $     2, $    -1
       DJN    $    -2, <  2920
       DAT    #   948, #  -948
       END      START
