;redcode
;name Blur '88
;author Anton Marsden
;assert 1

       MOV    $     5, <     9
       ADD    $     7, $     1
START  CMP    $  -205, $  -210
       MOV    $    -1, @    -3
       JMN    $    -4, @    -3
       SPL    $     0, <   -69
       MOV    $     2, <    -3
       DJN    $    -1, @   -71
       DAT    <   -70, <   -70
       DAT    #     0, #  -140
       END      START
