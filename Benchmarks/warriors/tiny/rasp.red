;redcode-tiny
;name Rasp
;author Leonardo H. Liporati
;assert 1

       ORG      START
START  SNE.I  $  -127, $   -53
       SEQ.I  $   361, <    13
       JMP.B  $     6, >    12
       SNE.I  $  -290, $  -216
       SEQ.I  $   198, <     9
       JMP.B  $     3, {     3
       SNE.I  $    45, $   119
       JMZ.B  $     6, $   121
       MUL.BA $     6, $     1
       SNE.I  $   311, >   310
       ADD.A  #    74, $    -1
       MOV.I  $     9, }    -2
       JMN.F  $    -1, }    -3
       SPL.B  $     1, }   264
       SPL.B  $     1, }  -377
       SPL.B  @     0, >   137
       MOV.I  }    -1, >    -1
       MOV.I  {    -2, <     1
       SPL.B  @     0, >    42
       MOV.I  #   274, }  -137
