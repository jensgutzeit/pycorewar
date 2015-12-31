;redcode
;name Twilight Pits v6.0
;author W. Mintardjo
;assert 1

START  MOV    $    16, $  3000
       MOV    $    14, $  2995
       MOV    $    12, $  2993
       MOV    $    10, $  2991
       MOV    $     8, $  2989
       MOV    $    15, $  3004
       MOV    $    13, $  3002
       MOV    $    11, $  3000
       MOV    $     9, $  2998
       MOV    $     2, $  3088
       JMP    $  2983, <  1112
       DAT    <   -18, <   -18
       SPL    $     0, <    -5
       MOV    $     6, @     6
       SUB    $    11, $     5
       DJN    $    -2, <  3025
       JMP    $   -80, <    86
       MOV    $    91, <   -91
       SPL    $    -1, $     0
       SPL    $     0, $     0
       SPL    $    -1, $     0
       END      START
