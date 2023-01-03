#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include<sys/time.h>

long long tseed(void) {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}

int check(const char *id) {
    int len = strlen(id);
    if (len > 9) return 0;
    char help[10];
    strcpy(help, id);
    if (len < 9)
        for (int i = 0; i < 9 - len; i++)
            help[i] = '0';
        help[9] = '\0';

    int sum = 0;
    for (int i = 0; i < 9; i++) {
        if (!isdigit(help[i]))
            return 0;
        int digit = help[i] - '0';
        int enums = digit * ((i % 2) + 1);
        sum += enums > 9 ? enums - 9 : enums;
    }

    return sum % 10 == 0;
}

char* gen() {
    static char val[12];
    srand(tseed());
    sprintf(val, "%03u", rand() % 1000); // mov --> dword ptr [esp+4]
    // generate national 3 numbers
    for (int i = 0; i < 5; i++)
        sprintf(val + 3 + i, "%u", rand() % 10);

    int sum = 0; // DWORD PTR
    for (int i = 0; i < 8; i++) {
        int digit = val[i] - '0'; // movzx --> eax, BYTE PTR val[rax]
        if (digit < 0 || digit > 9) return NULL;
        int result = digit * (i % 2 == 0 ? 1 : 2);
        while (result > 0) {
            sum += result % 10;
            result /= 10;
        }
    }
    sprintf(val + 8, "%u", (10 - (sum % 10)) % 10); // mov --> dword ptr [esp+4]
    return val;
}



int main(void) {
    int choice;
    printf("\r\n1 - Check Valid ID\n2 - Generate ID\r\nEnter choice: \n");
    scanf("%d", &choice);
    switch (choice) {
        case 1: {
            char id[11];
            printf("Enter ID: ");
            while(scanf("%9s", id) == 1) {
                if(id[0] == 'q')
                    break;
                int result = check(id);
                printf("The ID number is %s\n", result ? "valid" : "invalid");
                printf("Enter ID: ");
            }
        }
        case 2: {
            printf("Generating...\r\n");
            int count = 0;
            char* genid = gen();
            printf("The ID number is %9s\n", genid);
        }
        default:
            return 0;
    }
}
