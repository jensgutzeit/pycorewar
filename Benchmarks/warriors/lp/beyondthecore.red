;redcode-lp
;name From Beyond the Core
;author John Metcalf
;assert 1

       ORG      START
       DJN.F  #     1, $     1
       ADD.AB #  1671, #  1672
START  JMZ.F  $    -1, @    -1
       SLT.B  $    -2, #     5
       MOV.I  >     1, @    -3
       DJN.B  $    -4, #    -5
