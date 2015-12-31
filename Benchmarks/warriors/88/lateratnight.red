;redcode
;name LaterAtNight
;author Robert Macrae
;assert 1

       ADD    #  -480, $     7
       ADD    #  -240, @    -1
       ADD    #  -120, @    -2
       CMP    @     4, $    -5
       ADD    # -3900, $     3
       ADD    #  3900, $     2
       MOV    $     5, @     1
       MOV    $     4, @  -235
       SUB    #     7, $    -1
       DJN    $    -3, #     7
       JMP    $    31, $    15
       DAT    <     1, <    46
START  CMP    $  -240, $  3660
       JMP    $   -10, $     0
       CMP    $  -362, $  3538
       JMP    $   -13, $     0
       CMP    $  -484, $  3416
       JMP    $   -16, <   -15
       CMP    $  -606, $  3294
       JMP    $   -18, $     0
       CMP    $  -848, $  3052
       JMP    $   -21, <   -20
       CMP    $  -970, $  2930
       JMP    $   -23, <   -21
       CMP    $ -1092, $  2808
       JMP    $   -25, $     0
       MOV    # -1195, $   -19
       CMP    $ -1215, $  2685
       JMP    $   -25, $     0
       CMP    $ -1337, $  2563
       JMP    $   -28, $     0
       CMP    $ -1459, $  2441
       JMP    $   -31, <   -30
       CMP    $ -1581, $  2319
       JMP    $   -33, $     0
       CMP    $ -1823, $  2077
       JMP    $   -36, <   -35
       CMP    $ -1945, $  1955
       JMP    $   -38, <   -36
       CMP    $ -2067, $  1833
       JMP    $   -40, $     0
       SPL    $     2, $     0
       SPL    $     2, $  2930
       SPL    $     1, $    12
       MOV    $    -1, $     0
       MOV    <    -2, <    -3
       SPL    $  2917, $ -2157
       MOV    <  2925, <    -1
       MOV    <     9, <     1
       SPL    @     0, $  2030
       MOV    <     9, <     1
       SPL    @     0, $ -1158
       MOV    $     3, <    -4
       MOV    #     0, $     6
       JMP    $    -4, <    -3
       END      START
