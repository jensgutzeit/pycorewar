;redcode
;name Beholder's Eye v1.7
;author W. Mintardjo
;assert 1

       ADD    #  1226, @     2
START  JMZ    $    -1, @     2
       MOV    $     5, @     1
       MOV    $     2, <  1224
       JMN    $    -4, @    -4
       SPL    $     0, <     0
       MOV    $     2, <    -6
       JMP    $    -1, $     0
       DAT    <    -2, <   -92
       END      START
