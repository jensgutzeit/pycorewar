;redcode-lp
;name Paper 'LP' II
;author Philip Kendall
;assert 1

       ORG      START
START  SPL.A  $    18, >  1000
       SPL.A  $    40, >  2000
       SPL.A  $    62, >  3000
       SPL.A  $    84, >  4000
       SPL.A  $   106, > -3000
       SPL.A  $   128, > -2000
       SPL.A  $   150, > -1000
       JMP.A  $   172, >  -500
       DAT.A  $     1, $     1
       DAT.B  $     1, $     1
       DAT.AB $     1, $     1
       DAT.BA $     1, $     1
       DAT.F  $     1, $     1
       DAT.X  $     1, $     1
       DAT.I  $     1, $     1
       DAT.A  $     1, #     1
       MOV.A  #     1, $     6
       MOV.AB #     7, $     2
       MOV.I  {     4, <     4
       DJN.B  $    -1, #     7
       SPL.A  *     2, > -2667
       JMZ.B  @     1, $    -2
       DAT.F  $     1, $   883
       DAT.A  $     1, $     1
       DAT.B  $     1, $     1
       DAT.AB $     1, $     1
       DAT.BA $     1, $     1
       DAT.F  $     1, $     1
       DAT.X  $     1, $     1
       DAT.I  $     1, $     1
       DAT.A  #     1, $     1
       DAT.B  #     1, $     1
       DAT.AB #     1, $     1
       DAT.BA #     1, $     1
       DAT.F  #     1, $     1
       DAT.X  #     1, $     1
       DAT.I  #     1, $     1
       DAT.A  $     1, #     1
       DAT.B  $     1, #     1
       MOV.A  #     1, $     6
       MOV.AB #     7, $     2
       MOV.I  {     4, <     4
       DJN.B  $    -1, #     7
       SPL.A  *     2, > -1143
       JMZ.B  @     1, $    -2
       DAT.F  $     1, $  1257
       DAT.A  $     1, $     1
       DAT.B  $     1, $     1
       DAT.AB $     1, $     1
       DAT.BA $     1, $     1
       DAT.F  $     1, $     1
       DAT.X  $     1, $     1
       DAT.I  $     1, $     1
       DAT.A  #     1, $     1
       DAT.B  #     1, $     1
       DAT.AB #     1, $     1
       DAT.BA #     1, $     1
       DAT.F  #     1, $     1
       DAT.X  #     1, $     1
       DAT.I  #     1, $     1
       DAT.A  $     1, #     1
       DAT.B  $     1, #     1
       MOV.A  #     1, $     6
       MOV.AB #     7, $     2
       MOV.I  {     4, <     4
       DJN.B  $    -1, #     7
       SPL.A  *     2, >  -889
       JMZ.B  @     1, $    -2
       DAT.F  $     1, $  -573
       DAT.A  $     1, $     1
       DAT.B  $     1, $     1
       DAT.AB $     1, $     1
       DAT.BA $     1, $     1
       DAT.F  $     1, $     1
       DAT.X  $     1, $     1
       DAT.I  $     1, $     1
       DAT.A  #     1, $     1
       DAT.B  #     1, $     1
       DAT.AB #     1, $     1
       DAT.BA #     1, $     1
       DAT.F  #     1, $     1
       DAT.X  #     1, $     1
       DAT.I  #     1, $     1
       DAT.A  $     1, #     1
       DAT.B  $     1, #     1
       MOV.A  #     1, $     6
       MOV.AB #     7, $     2
       MOV.I  {     4, <     4
       DJN.B  $    -1, #     7
       SPL.A  *     2, >  2909
       JMZ.B  @     1, $    -2
       DAT.F  $     1, $  3139
       DAT.A  $     1, $     1
       DAT.B  $     1, $     1
       DAT.AB $     1, $     1
       DAT.BA $     1, $     1
       DAT.F  $     1, $     1
       DAT.X  $     1, $     1
       DAT.I  $     1, $     1
       DAT.A  #     1, $     1
       DAT.B  #     1, $     1
       DAT.AB #     1, $     1
       DAT.BA #     1, $     1
       DAT.F  #     1, $     1
       DAT.X  #     1, $     1
       DAT.I  #     1, $     1
       DAT.A  $     1, #     1
       DAT.B  $     1, #     1
       MOV.A  #     1, $     6
       MOV.AB #     7, $     2
       MOV.I  {     4, <     4
       DJN.B  $    -1, #     7
       SPL.A  *     2, > -3077
       JMZ.B  @     1, $    -2
       DAT.F  $     1, $ -2047
       DAT.A  $     1, $     1
       DAT.B  $     1, $     1
       DAT.AB $     1, $     1
       DAT.BA $     1, $     1
       DAT.F  $     1, $     1
       DAT.X  $     1, $     1
       DAT.I  $     1, $     1
       DAT.A  #     1, $     1
       DAT.B  #     1, $     1
       DAT.AB #     1, $     1
       DAT.BA #     1, $     1
       DAT.F  #     1, $     1
       DAT.X  #     1, $     1
       DAT.I  #     1, $     1
       DAT.A  $     1, #     1
       DAT.B  $     1, #     1
       MOV.A  #     1, $     6
       MOV.AB #     7, $     2
       MOV.I  {     4, <     4
       DJN.B  $    -1, #     7
       SPL.A  *     2, > -2353
       JMZ.B  @     1, $    -2
       DAT.F  $     1, $  2155
       DAT.A  $     1, $     1
       DAT.B  $     1, $     1
       DAT.AB $     1, $     1
       DAT.BA $     1, $     1
       DAT.F  $     1, $     1
       DAT.X  $     1, $     1
       DAT.I  $     1, $     1
       DAT.A  #     1, $     1
       DAT.B  #     1, $     1
       DAT.AB #     1, $     1
       DAT.BA #     1, $     1
       DAT.F  #     1, $     1
       DAT.X  #     1, $     1
       DAT.I  #     1, $     1
       DAT.A  $     1, #     1
       DAT.B  $     1, #     1
       MOV.A  #     1, $     6
       MOV.AB #     7, $     2
       MOV.I  {     4, <     4
       DJN.B  $    -1, #     7
       SPL.A  *     2, >   421
       JMZ.B  @     1, $    -2
       DAT.F  $     1, $  3803
       DAT.A  $     1, $     1
       DAT.B  $     1, $     1
       DAT.AB $     1, $     1
       DAT.BA $     1, $     1
       DAT.F  $     1, $     1
       DAT.X  $     1, $     1
       DAT.I  $     1, $     1
       DAT.A  #     1, $     1
       DAT.B  #     1, $     1
       DAT.AB #     1, $     1
       DAT.BA #     1, $     1
       DAT.F  #     1, $     1
       DAT.X  #     1, $     1
       DAT.I  #     1, $     1
       DAT.A  $     1, #     1
       DAT.B  $     1, #     1
       MOV.A  #     1, $     6
       MOV.AB #     7, $     2
       MOV.I  {     4, <     4
       DJN.B  $    -1, #     7
       SPL.A  *     2, >  -381
       JMZ.B  @     1, $    -2
       DAT.F  $     1, $  -921
       DAT.A  $     1, $     1
       DAT.B  $     1, $     1
       DAT.AB $     1, $     1
       DAT.BA $     1, $     1
       DAT.F  $     1, $     1
       DAT.X  $     1, $     1
       DAT.I  $     1, $     1
       DAT.A  #     1, $     1
       DAT.B  #     1, $     1
       DAT.AB #     1, $     1
       DAT.BA #     1, $     1
       DAT.F  #     1, $     1
       DAT.X  #     1, $     1
       DAT.I  #     1, $     1
       DAT.A  $     1, #     1
       DAT.B  $     1, #     1