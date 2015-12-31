;redcode
;name Oneshot '88
;author John Metcalf
;assert 1

       DAT    <  2667, <    -7
       SPL    $     0, <   -15
       MOV    @     4, <     5
       MOV    @     3, <     4
       DJN    $    -2, <    -9
       DAT    <   -22, <   -22
       ADD    $    -1, @    -4
START  CMP    $  -121, $  -110
       JMP    <    -2, $     0
       JMP    $    -3, $     0
       END      START
