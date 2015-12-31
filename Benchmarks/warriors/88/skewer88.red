;redcode
;name Skewer '88
;author Ben Ford
;assert 1

       ADD    #   800, $     7
       ADD    #   400, @    -1
       ADD    #   200, @    -2
       CMP    @     4, $    -5
       ADD    # -3900, $     3
       ADD    #  3902, $     2
       MOV    <     3, <     1
       MOV    <     2, @   417
       DJN    $    -2, #     7
       JMP    $    44, $    15
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
START  CMP    $   400, $ -3700
       JMP    $   -22, $     0
       CMP    $   598, $ -3502
       JMP    $   -25, $     0
       CMP    $   796, $ -3304
       JMP    $   -28, <   -27
       CMP    $   994, $ -3106
       JMP    $   -30, $     0
       CMP    $  1392, $ -2708
       JMP    $   -33, <   -32
       CMP    $  1590, $ -2510
       JMP    $   -35, <   -33
       CMP    $  1788, $ -2312
       JMP    $   -37, $     0
       MOV    #  2017, $   -31
       CMP    $  1985, $ -2115
       JMP    $   -37, $     0
       CMP    $  2183, $ -1917
       JMP    $   -40, $     0
       CMP    $  2381, $ -1719
       JMP    $   -43, <   -42
       CMP    $  2579, $ -1521
       JMP    $   -45, $     0
       CMP    $  2977, $ -1123
       JMP    $   -48, <   -47
       CMP    $  3175, $  -925
       JMP    $   -50, <   -48
       CMP    $  3373, $  -727
       JMP    $   -52, $     0
       MOV    $    14, $ -2574
       MOV    @     7, $ -2579
       MOV    <     6, <    -1
       MOV    <     5, <    -2
       MOV    <     4, <    -3
       MOV    <     3, <    -4
       MOV    <     2, <    -5
       JMP    @    -6, <    -6
       SPL    $     0, $     5
       SPL    $     0, < -2721
       MOV    $     7, <  -162
       MOV    $     6, @    -1
       SUB    #    81, $    -2
       DJN    $    -3, < -2725
       DAT    < -2722, < -2722
       END      START
