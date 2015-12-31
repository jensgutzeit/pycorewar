;redcode
;name Griffin 2
;author Anders Ivner
;assert 1

START  MOV    <    11, <    12
       MOV    <    10, <    11
       MOV    <     9, <    10
       MOV    <     8, <     9
       MOV    <     7, <     8
       MOV    <     6, <     7
       MOV    <     5, <     6
       MOV    <     4, <     5
       MOV    <     3, <     4
       MOV    $     5, $  2992
       MOV    $     3, $  2881
       JMP    $  3007, $    13
       DAT    #     0, #  3012
       DAT    <   -21, #     0
       JMP    @  4000, $  3999
       MOV    $   -14, @     3
       MOV    $     6, <     2
       SUB    $   -16, @    -1
       CMP    $  -128, $  3872
       JMN    $    -4, @    -3
       ADD    $     3, @    -4
       DJN    $    -3, #   997
       SPL    $     0, <   -20
       MOV    $  -132, <  -132
       END      START
