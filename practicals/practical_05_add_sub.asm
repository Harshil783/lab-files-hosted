; Addition and subtraction of two 8-bit numbers (GNUSim8085)
MVI B,25H
MVI C,12H
MOV A,B
ADD C
STA 2000H      ; sum
MOV A,B
SUB C
STA 2001H      ; difference (B - C)
HLT
