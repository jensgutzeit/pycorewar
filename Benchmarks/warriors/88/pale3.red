;redcode
;name Pale Shears3
;author Matt Hastings
;assert 1

START  MOV    $     6, $   159
       MOV    $     3, <    -1
       ADD    #   156, $    -2
       DJN    $    -3, #  1599
       SPL    $     0, <     0
       MOV    $     2, <    -4
       JMP    $    -1, $    -1
       DAT    <   -92, <    -2
       END      START
