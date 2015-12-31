;redcode
;name Blurstone '88
;author M. J. Pihlaja
;assert 1

       MOV    $     3, @     1
       MOV    $     5, <     9
       SUB    $     7, $     1
START  MOV    $    68, <    74
       JMZ    $    -3, <    -1
       JMZ    $    -5, $   -12
       SPL    $     0, <   -69
       MOV    $     2, <   -13
       DJN    $    -1, <  2652
       DAT    <   -70, #   -71
       DAT    <   -70, #  3500
       END      START
