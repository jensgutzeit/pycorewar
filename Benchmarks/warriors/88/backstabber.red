;redcode
;name Backstabber
;author Anders Ivner
;assert 1

START  ADD    $    14, $     1
       CMP    $   -19, $     0
       SLT    #    39, $    -1
       JMP    $    -3, $     0
       MOV    #    38, $     3
       MOV    $    -4, @     1
       MOV    $     9, <     6
       DJN    $    -1, #     0
       DJN    $    -8, #    20
       SPL    $     0, <    -3
       MOV    $     3, <    -2
       JMP    $    -1, <    -5
       DAT    #     0, #     0
       DAT    <    -4, <    -5
       DAT    #  3094, #  3094
       SPL    $    -1, $     1
       END      START
