;redcode
;name Spitfire 3
;author Stefan Strack
;assert 1

       MOV    $    -1, <    -1
       ADD    $    10, @     3
       MOV    $    10, @     4
       MOV    $    -3, <     3
       MOV    $     6, <     2
       SUB    $     8, @    -1
START  CMP    $   768, $   782
       SLT    #    27, @    -3
       DJN    $    -3, <   -14
       JMZ    $    -8, $     0
       SPL    $     0, $     7
       MOV    @   -14, <   -12
       JMP    $    -2, <  2667
       DAT    <   -28, <   -28
       END      START
