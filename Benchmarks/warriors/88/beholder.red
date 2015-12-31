;redcode
;name Beholder's Eye v1.1
;author W. Mintardjo
;assert 1

START  ADD    #  2234, $     3
       JMZ    $    -1, @     2
       MOV    $     5, @     1
       MOV    $     2, <    -2
       JMN    $    -4, $    -4
       SPL    $     0, <     0
       MOV    $     2, <    -6
       JMP    $    -1, $     0
       DAT    <   -92, <    -2
       END      START
