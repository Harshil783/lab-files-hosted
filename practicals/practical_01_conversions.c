#include <stdio.h>

void decToBin(unsigned int n) {
    int b[32], i = 0;
    if (n == 0) { printf("0"); return; }
    while (n > 0) { b[i++] = n % 2; n /= 2; }
    for (i--; i >= 0; i--) printf("%d", b[i]);
}

void decToHex(unsigned int n) {
    char h[16]; int i = 0;
    if (n == 0) { printf("0"); return; }
    while (n > 0) {
        int r = n % 16;
        h[i++] = (r < 10) ? '0' + r : 'A' + (r - 10);
        n /= 16;
    }
    for (i--; i >= 0; i--) putchar(h[i]);
}

unsigned int binToDec(long long b) {
    unsigned int d = 0, p = 1;
    while (b > 0) { d += (b % 10) * p; p *= 2; b /= 10; }
    return d;
}

int main() {
    int ch; unsigned int n; long long b;
    printf("1.Dec->Bin 2.Dec->Hex 3.Bin->Dec: ");
    scanf("%d", &ch);
    if (ch == 1) { printf("Decimal: "); scanf("%u", &n); decToBin(n); }
    else if (ch == 2) { printf("Decimal: "); scanf("%u", &n); decToHex(n); }
    else if (ch == 3) { printf("Binary: "); scanf("%lld", &b); printf("%u", binToDec(b)); }
    printf("\n");
    return 0;
}
