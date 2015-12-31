;redcode-tiny
;name Tiny Freeze
;author Lukasz Grabun
;assert 1

       ORG      START
       JMP.B  $     7, >  -354
       MOV.I  $     5, >    -1
START  SEQ.I  }   210, }   215
       MOV.AB $    -1, @    -2
       ADD.F  $     2, $    -2
       JMN.A  $    -4, $    -3
       SPL.B  #   210, #   210
       MOV.I  $     2, >   -11
       DJN.F  $    -1, >   -12
       DAT.F  >     5, $    15
