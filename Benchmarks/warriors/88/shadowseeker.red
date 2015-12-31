;redcode
;name Shadow Seeker
;author John Metcalf
;assert 1

       ADD    #     1, @     2
       MOV    $     6, <    10
       SUB    $     7, $     1
START  SUB    $   207, <   213
       JMZ    @    -4, <    -1
       MOV    $    -2, @    -4
       JMZ    <    -6, #     0
       SPL    $     0, <   -70
       MOV    $     1, <    -5
       DAT    <   -70, <   -71
       END      START
