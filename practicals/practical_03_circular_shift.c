#include <stdio.h>

void printBin8(unsigned char n) {
    for (int i = 7; i >= 0; i--) printf("%d", (n >> i) & 1);
}

unsigned char rotl(unsigned char n, int k) {
    k %= 8;
    return (unsigned char)((n << k) | (n >> (8 - k)));
}
unsigned char rotr(unsigned char n, int k) {
    k %= 8;
    return (unsigned char)((n >> k) | (n << (8 - k)));
}

int main() {
    unsigned int n; int k;
    printf("Enter 8-bit number (0-255) and shift count: ");
    scanf("%u %d", &n, &k);
    printf("Original      : "); printBin8(n);          printf("\n");
    printf("Circular left : "); printBin8(rotl(n, k)); printf("\n");
    printf("Circular right: "); printBin8(rotr(n, k)); printf("\n");
    return 0;
}
