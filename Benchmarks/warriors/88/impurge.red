;redcode
;name Impurge
;author Fredrik Ohrstrom
;assert 1

       MOV    #    26, $     3
       ADD    #    26, $     4
       MOV    $     8, <     3
       DJN    $    -1, #    26
START  SUB    $     7, @    -2
       CMP    $    16, $     0
       DJN    $    -6, <     1
       DJN    $    -3, #   245
       SPL    $     0, <   -31
       MOV    $     2, <   -29
       SPL    $    -1, <   -31
       DAT    <   -32, <   -32
       END      START
