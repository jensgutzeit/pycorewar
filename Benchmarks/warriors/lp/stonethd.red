;redcode-lp
;name Stone Throwing Devils
;author Ben Ford
;assert 1

       ORG      START
       MOV.I  $    14, {     8
       MOV.I  $    12, {     7
       MOV.I  $    10, {     6
       SPL.B  $     1, $     1
       SPL.B  $     1, $     1
       SPL.B  $     2, <  4000
       JMP.B  *     2, }     0
       JMP.B  @     1, }     0
       DIV.F  #  2694, #  2693
       DIV.F  #   190, #  2857
       DIV.F  # -2477, #   190
       DIV.F  #  2856, # -2477
       MOV.I  >   -32, $  2727
       ADD.F  $     1, $    -1
       JMP.B  $    -2, <  2726
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  <    15, <    10
       DAT.F  <     7, <     4
       DAT.F  <    13, <    11
       DAT.F  <    76, <     1
START  SEQ.I  $  3959, $   -41
       JMP.B  $    55, {  3967
       SEQ.I  $  3257, $  -743
       JMP.B  $    52, }    -6
       SEQ.I  $  2755, $ -1245
       JMP.B  $    50, }    50
       SEQ.I  $  3453, $  -547
       JMP.B  $    48, {   -10
       SEQ.I  $  2651, $ -1349
       DJN.A  $    46, {    46
       SEQ.I  $  2549, $ -1451
       JMP.B  $    44, {    44
       SEQ.I  $  3347, $  -653
       JMP.B  $    42, {  3355
       SEQ.I  $  3645, $  -355
       JMP.B  >    40, {  3653
       SEQ.I  $ -3657, $   343
       JMP.B  $    37, }    37
       SEQ.I  $ -2959, $  1041
       JMP.B  $    35, {    35
       SEQ.I  $  3539, $  -461
       JMP.B  >    34, >   -24
       SEQ.I  $ -2263, $  1737
       DJN.B  $    31, {    31
       SEQ.I  $  3735, $  -265
       JMP.B  >    30, <   -28
       SEQ.I  $ -1567, $  2433
       DJN.A  $    27, {    28
       SEQ.I  $   131, $ -3869
       DJN.B  $    25, }    26
       SEQ.I  $  1629, $ -2371
       JMP.B  $    23, {   -34
       SEQ.I  $   527, $ -3473
       JMP.B  $    21, >   -36
       SEQ.I  $ -1975, $  2025
       JMP.B  $    19, {    20
       SEQ.I  $  1923, $ -2077
       JMP.B  $    17, <   -40
       SEQ.I  $  2921, $ -1079
       JMP.B  >    16, }    15
       SEQ.I  $   819, $ -3181
       JMP.B  $    13, }   -44
       SEQ.I  $  3017, $  -983
       JMP.B  >    12, {    11
       SEQ.I  $ -2585, $  1415
       DJN.A  $     9, }     9
       SEQ.I  $ -1187, $  2813
       JMP.B  $     7, }     8
       SEQ.I  $  2211, $ -1789
       DJN.F  $     5, $   -52
       SEQ.I  $  3109, $  -891
       DJN.B  >     4, {     3
       SNE.I  $  1207, $ -2793
       JMZ.B  *     9, $  1196
       MUL.B  $   -57, $     5
       MUL.AB $   -58, @    -1
       SNE.I  >  3456, @     3
       ADD.AB #  4000, $     2
       MOV.I  $   -59, @     1
       MOV.I  $   -60, *  -100
       SUB.AB #     9, $    -1
       DJN.B  $    -3, #    12
       JMP.B  $  -131, <  1234
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       DAT.F  $     0, $     0
       MOV.I  #  2667, *     0
