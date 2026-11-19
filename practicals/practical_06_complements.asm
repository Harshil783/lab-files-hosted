; 1's and 2's complement of an 8-bit number (GNUSim8085)
MVI A,55H
CMA
STA 2000H      ; 1's complement
INR A
STA 2001H      ; 2's complement
HLT
