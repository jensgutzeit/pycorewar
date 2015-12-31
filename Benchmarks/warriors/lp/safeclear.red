;redcode-lp
;name Safe Clear
;author Lukasz Adamowski
;assert 1

       ORG      START
START  SPL.B  #     0, }     1
       MOV.I  *     2, }     0
       DAT.F  $     0, }    -1
