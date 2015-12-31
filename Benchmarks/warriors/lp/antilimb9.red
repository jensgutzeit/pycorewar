;redcode-lp
;name Anti-Limb v9
;author Ryan Coleman
;assert 1

       ORG      START
       JMP.A  #    20, <  2465
START  ADD.I  $    -1, $   -60
       MOV.I  $     3, *   -61
       ADD.AB #    -1, $    -1
       JMP.A  $    -3, <    -3
       DAT.X  <  2465, }     6
