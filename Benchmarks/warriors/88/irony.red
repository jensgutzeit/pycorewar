;redcode
;name Irony v1.0
;author Brant D. Thomsen
;assert 1

       ADD    #    12, $     3
       MOV    #    12, $     6
       ADD    $     9, $     1
START  CMP    $   -80, $   -70
       SLT    #    22, $    -1
       DJN    $    -3, < -3503
       MOV    $     3, <    -3
       DJN    $    -1, #    12
       JMZ    $    -8, $    -9
       SPL    $     0, <   -77
       MOV    $     1, <    -3
       DAT    <   -78, <   -78
       END      START
