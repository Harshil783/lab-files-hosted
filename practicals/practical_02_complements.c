#include <stdio.h>

void printBin8(unsigned char n) {
    for (int i = 7; i >= 0; i--) printf("%d", (n >> i) & 1);
}

int main() {
    unsigned int n;
    printf("Enter 8-bit number (0-255): ");
    scanf("%u", &n);
    unsigned char x = (unsigned char)n;
    unsigned char c1 = ~x;
    unsigned char c2 = c1 + 1;
    printf("Original : "); printBin8(x);  printf("\n");
    printf("1's comp : "); printBin8(c1); printf("\n");
    printf("2's comp : "); printBin8(c2); printf("\n");
    return 0;
}
