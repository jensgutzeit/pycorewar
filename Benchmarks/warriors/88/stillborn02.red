;redcode
;name Stillborn Bomber v0.2
;author mjp
;assert 1

START  CMP    $   580, $   868
       JMP    $    35, $    61
       CMP    $  1154, $  1442
       JMP    $    32, $    24
       CMP    $  1728, $  2016
       JMP    $    30, <    -2
       CMP    $  2302, $  2590
       JMP    $    27, <    28
       CMP    $  2876, $  3164
       JMP    $    25, $    20
       CMP    $  3450, $  3738
       JMP    $    23, <    -8
       CMP    $ -3976, $ -3688
       JMP    $    21, <    -4
       CMP    $ -3402, $ -3114
       JMP    $    18, <    19
       CMP    $ -2828, $ -2540
       JMP    $    16, <     4
       CMP    $ -2254, $ -1966
       JMP    $    14, <    16
       CMP    $ -1680, $ -1392
       JMP    $    12, $    10
       CMP    $ -1106, $  -818
       JMP    $    10, <   -20
       CMP    $  -532, $  -244
       JMP    $     8, <   -16
       JMP    $    17, <  1152
       SPL    $     0, $   576
       SPL    $     0, $  2880
       SPL    $     0, $  1728
       SPL    $     0, $  2304
       SPL    $     0, $  3456
       DAT    <     1, <   121
       ADD    @   -12, $     7
       ADD    @   -25, @    -1
       ADD    @   -32, @    -2
       CMP    $   264, @     4
       SUB    #   288, @    -1
       ADD    #   299, @    -2
       MOV    $    -7, @     1
       MOV    $  3000, @   540
       SUB    #    11, $    -1
       DJN    $    -3, #     8
       MOV    <   -42, <     9
       MOV    <   -43, <     8
       MOV    <   -44, <     7
       MOV    <   -45, <     6
       MOV    <   -46, <     5
       MOV    <   -47, <     4
       MOV    <   -48, <     3
       MOV    <   -49, <     2
       MOV    <   -50, <     1
       JMP    @     0, $ -2852
       SPL    $     0, <   -93
       ADD    #  2231, $     1
       MOV    $    -2, @     2
       MOV    $     5, @    -1
       JMZ    $    -3, @     0
       MOV    $     2, <    -2
       DJN    $    -1, < -3710
       DAT    < -2762, <   -94
       MOV    $   100, <   100
       END      START
