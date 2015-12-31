;redcode
;name Miss Careless
;author Derek Ross
;assert 1

       DAT    #    -1, #   -12
       DAT    #     0, #    -4
       SPL    $     0, $    -7
       SPL    $     0, $    -7
       MOV    @     1, <     3
       DJN    $    -1, <    -2
       DAT    #   -39, #   -39
       CMP    <  -224, <  -200
       JMP    $    -5, $     0
START  ADD    $    -3, $    -2
       MOV    $   -10, @    -3
       DJN    $    -4, <   500
       JMP    $    -9, $     0
       END      START
