;redcode
;name dead end
;author nandor sieben
;assert 1

START  MOV    $    43, $   931
       MOV    $    41, <    -1
       MOV    $    39, <    -2
       MOV    $    40, $  1816
       MOV    $    38, <    -1
       MOV    $    36, <    -2
       MOV    $    37, $  2701
       MOV    $    35, <    -1
       MOV    $    33, <    -2
       MOV    $    34, $  3586
       MOV    $    32, <    -1
       MOV    $    30, <    -2
       MOV    $    31, $ -3529
       MOV    $    29, <    -1
       MOV    $    27, <    -2
       MOV    $    28, $ -2644
       MOV    $    26, <    -1
       MOV    $    24, <    -2
       MOV    $    25, $ -1759
       MOV    $    23, <    -1
       MOV    $    21, <    -2
       MOV    $    22, $  -874
       MOV    $    20, <    -1
       MOV    $    18, <    -2
       SPL    $     4, $     0
       SPL    $     6, $     0
       SPL    $     9, $     0
       JMP    @   -27, $     0
       SPL    $     5, $     0
       SPL    $    10, $     0
       JMP    @   -27, $     0
       SPL    $     7, $     0
       JMP    @   -26, $     0
       SPL    $     7, $     0
       JMP    @   -25, $     0
       JMP    @   -23, $     0
       DAT    #     0, #     0
       DAT    #     0, #     0
       JMP    @   -23, $     0
       JMP    @   -21, $     0
       SPL    @   -19, $     0
       SPL    $     0, <    -5
       MOV    $    -6, <    -6
       DJN    $    -2, <    -7
       END      START
