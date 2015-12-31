;redcode
;name Gisela 609
;author Andrzej Maciejczak
;assert 1

       DAT    <  2667, #   -13
       SPL    $     0, $   -13
       ADD    #   652, @     3
START  JMZ    $    -1, $   652
       ADD    #     1, @    -1
       JMZ    $    -3, @    -2
       MOV    $    -3, $     6
       SPL    $     0, $     0
       MOV    @     5, <     4
       MOV    @     4, <     3
       DJN    $    -2, <   -11
       DAT    #     0, #     0
       DAT    #     0, #     0
       DAT    #     0, #   -12
       END      START
