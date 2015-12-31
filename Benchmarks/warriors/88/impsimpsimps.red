;redcode
;name Imps!  Imps!  Imps!
;author Steven Morrell
;assert 1

       MOV    $     0, $  1143
START  SPL    $     0, $     0
       MOV    $    -2, $    10
       ADD    #   260, $    -1
       SPL    $     1, $     0
       SPL    $     1, $     0
       SPL    $     1, $     0
       SPL    $     1, $     0
       SPL    $     2, $     0
       JMP    @    -7, $     0
       ADD    #  1143, $    -8
       END      START
