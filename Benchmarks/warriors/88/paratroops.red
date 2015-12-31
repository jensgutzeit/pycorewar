;redcode
;name Paratroops v1.2
;author W. Mintardjo
;assert 1

       DAT    #    98, #    98
       ADD    $    -1, $     1
START  CMP    $    97, $   144
       SLT    #    62, $    -1
       JMP    $    -3, $     0
       MOV    $    -5, <    -3
       DJN    $    -1, #    50
       ADD    #  1600, $     4
       MOV    $     9, @     3
       MOV    $     7, <     2
       MOV    $     5, <     1
       SPL    @     0, $   504
       MOV    $     2, <    -1
       DJN    $    -6, #     5
       DAT    <    -4, <    -5
       SPL    $     0, <    -3
       MOV    $    -2, <    -4
       JMP    $    -1, <    -5
       END      START
