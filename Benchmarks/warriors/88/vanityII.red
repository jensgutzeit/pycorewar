;redcode
;name Vanity II
;author Stefan Strack
;assert 1

       ADD    #   754, @     2
START  JMZ    $    -1, @     2
       MOV    $     5, @     1
       MOV    $     2, <   756
       JMN    $    -4, @    -4
       SPL    $     0, <    -7
       MOV    $     2, <   -10
       JMP    $    -1, $     0
       DAT    < -2676, <    -8
       END      START
