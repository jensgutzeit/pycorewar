;redcode
;name Stasis
;author David Moore
;assert 1

       DAT    <   -15, <   -15
       JMN    $    -1, #     0
       MOV    $     5, <   657
       ADD    #   653, @     2
START  JMZ    $    -1, <    -2
       MOV    $    -4, @    -3
       JMZ    $    -4, @    -4
       SPL    $     0, <   -14
       MOV    $    -8, <    -2
       DJN    $    -1, <  3970
       END      START
