;redcode-tiny
;name Mad pixel
;author Neogryzor
;assert 1

       ORG      START
START  MOV.I  {    63, $   339
       MOV.I  {    85, $   361
       MOV.I  {   107, $   383
       MOV.I  {   129, $  -395
       MOV.I  {   151, $  -373
       MOV.I  {   173, $  -351
       MOV.I  {   195, $  -329
       MOV.I  {   217, $  -307
       MOV.I  {   239, $  -285
       MOV.I  {   261, $  -263
       MOV.I  {   283, $  -241
       MOV.I  {   305, $  -219
       SPL.B  $     1, {     0
       SPL.B  $     1, {  -244
       MOV.I  <     1, {     1
       SPL.B  $  -395, $     5
       SPL.B  @     0, $  -316
       MOV.I  }    -1, >    -1
       MOV.I  {    -2, {     1
       DJN.F  $   290, <   110
