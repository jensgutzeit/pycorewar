;redcode
;name Trivial v0.2
;author John Metcalf
;assert 1

       DAT    <  3428, <   -11
       DAT    <   -13, <    -2
       MOV    $   -24, $   -23
       ADD    $    -1, $     1
START  ADD    $  -122, <  -117
       JMZ    $    -2, <    -1
       MOV    $    -2, $     5
       SPL    $     0, <   -12
       MOV    @     4, <     3
       DJN    $    -1, <   -14
       JMP    $    -2, #     0
       DAT    #     0, #     0
       DAT    #     0, #    -5
       END      START
