;redcode
;name rdrc: Gymnosperm Trickery
;author Dave Hillis
;assert 1

START  SPL    $     0, <    -2
       SPL    $     0, <    -3
       MOV    $     1, <     1
       MOV    <     0, <  3432
       DJN    $    -2, <    -6
       END      START
