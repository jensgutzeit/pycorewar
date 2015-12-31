;redcode
;name Nimbus 1.2
;author Alex MacAulay
;assert 1

START  SPL    $    12, $     0
       SPL    $     1, $     0
       SPL    $     1, $     0
       SPL    $     1, $     0
       SPL    $     1, $     0
       SPL    $     1, $     0
       MOV    $    -1, $     0
       SPL    $     1, $     0
       SPL    $     2, $     0
       JMP    @     0, #     7
       ADD    #   127, $    -1
       DAT    <    -5, <    -6
       DJN    $     0, #    92
       MOV    $    -2, <     2
       SPL    $    -1, <    -2
       JMP    $    -2, <    -2
       MOV    $     0, $   127
       END      START
