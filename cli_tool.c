/*
 2025-07-17
 Created by Jonhengs <https://github.com/Jonhengs>

 compile with
 gcc cli_tool.c -o wfrp_calc -lm
*/


/*
  Supported Languages are
  - LANG_DE for German
  - LANG_EN for English
*/
#define LANG_EN

#if defined(LANG_DE)
    #define INP_G "gk"
    #define INP_S 's'
    #define INP_C 'g'

    #define OUT_G "GK"
    #define OUT_S "S"
    #define OUT_C "G"

    #define DIV_0 "Fehler: Division durch 0."
    #define INVALID_OP "Ungültige Operation."
    #define GOOD_BYE "Auf Wiedersehen!"

    #define TEXT1 "\n-- Warhammer Fantasy Rollenspiel Währungsrechner --\n\n"
    #define TEXT2 "Dieses Programm hilft beim Rechnen mit WFRSP 4e Währungen.\n"
    #define TEXT3 "In WFRSP, 1 GoldKrone(GK) = 20 Silverschlling(S) = 240 kupferGroschen(G).\n\n"
    #define TEXT4 "Werte werden in dem Format: \"2GK 5S 12G\" eingegeben.\n"
    #define TEXT5 "Bei Addition/Subtraktion wird der 2. Wert im selben Format übergeben,\nbei Multiplikation/Division wird der Faktor im Format: \"2.5\" eigegeben.\n"
    #define TEXT6 "Folgende Operatoren sind valide: \"+ - * :\".\nUm den aktuellen wert zu löschen 'c' um zu beenden 'q'\n"
#else // for any undefined lang use english
    #define INP_G "gc"
    #define INP_S 's'
    #define INP_C 'd'

    #define OUT_G "GC"
    #define OUT_S "S"
    #define OUT_C "D"

    #define DIV_0 "Error: Division by 0."
    #define INVALID_OP "Invalid operation."
    #define GOOD_BYE "Good bye!"

    #define TEXT1 "\n-- Warhammer Fantasy Roleplay Currency calculator --\n\n"
    #define TEXT2 "This program helps calculating with WFRP 4e currencys.\n"
    #define TEXT3 "In WFRP 1 gold crown(GC) = 20 silver shillings(S) = 240 brass pennies/dimes(D).\n\n"
    #define TEXT4 "Values can be entered in the format: \"2GC 5S 12D\".\n"
    #define TEXT5 "For addition/subtraction the 2. value will be entered in the same format.\nFor multiplication/divison the factor will be entered in the format: \"2.5\".\n"
    #define TEXT6 "The following operations are allowed: \"+ - * :\".\"\n'c' to start with zero again. 'q' to quit the program."
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define C_PER_S 12
#define S_PER_G 20
#define C_PER_G (C_PER_S * S_PER_G)

typedef struct {
    int g; // gold
    int s; // silver
    int c; // copper
} Currency;

int to_copper(Currency v) {
    return v.g * C_PER_G + v.s * C_PER_S + v.c;
}

Currency from_copper(int total) {
    Currency v;
    v.g = total / C_PER_G;
    total %= C_PER_G;
    v.s = total / C_PER_S;
    v.c = total % C_PER_S;
    return v;
}

void print_currency(Currency v) {
    printf("%d" OUT_G " %d" OUT_S " %d" OUT_C "\n", v.g, v.s, v.c);
}

void normalize(char *str) {
    char *src = str, *dst = str;
    while (*src) {
        if (!isspace((unsigned char)*src))
            *dst++ = tolower((unsigned char)*src);
        src++;
    }
    *dst = '\0';
}

Currency parse_currency(char *input) {
    normalize(input);
    Currency v = {0, 0, 0};
    char *p = input;
    while (*p) {
        int val = atoi(p);
        while (*p && isdigit((unsigned char)*p)) p++;
        if (strncmp(p, INP_G, 2) == 0) {
            v.g = val; p += 2;
        } else if (*p == INP_S) {
            v.s = val; p++;
        } else if (*p == INP_C) {
            v.c = val; p++;
        } else {
            p++;
        }
    }
    return v;
}

Currency add(Currency a, Currency b) {
    return from_copper(to_copper(a) + to_copper(b));
}

Currency subtract(Currency a, Currency b) {
    return from_copper(to_copper(a) - to_copper(b));
}

Currency multiply(Currency a, double factor) {
    return from_copper(round(to_copper(a) * factor));
}

Currency divide(Currency a, double divisor) {
    if (divisor == 0) {
        printf(DIV_0 "\n");
        return a;
    }
    return from_copper(round(to_copper(a) / divisor));
}

void set_zero(Currency v) {
    v.g = v.s = v.c = 0;
}

int main() {
    char line[200];
    char op;
    char *rest;
    Currency result = {0, 0, 0};
    int continueWithResult = 0;

    printf(TEXT1);
    printf(TEXT2);
    printf(TEXT3);
    printf(TEXT4);
    printf(TEXT5);
    printf(TEXT6);

    while (1) {
        Currency a, b;
        double factor;
        char *p;

        printf("\n");

        if (!continueWithResult) {
            printf("   ");
            fgets(line, sizeof(line), stdin);
            p = line;

            while (isspace((unsigned char)*p)) p++;
            
            if (tolower(*p) == 'q') break;
            if (tolower(*p) == 'c') {
                set_zero(result);
                continueWithResult = 0;
                printf(" * * * * * * \n");
                continue;
            }

            a = parse_currency(line);
        } else {
            a = result;
            printf("   ");
            print_currency(a);
        }

        printf(" ");
        fgets(line, sizeof(line), stdin);

        // remove leading spaces
        p = line;
        while (isspace((unsigned char)*p)) p++;

        // special commands
        if (tolower(*p) == 'q') break;
        if (tolower(*p) == 'c') {
            set_zero(result);
            continueWithResult = 0;
            printf(" * * * * * * \n");
            continue;
        }

        // set operator
        op = *p;
        p++;

        if (op == '+' || op == '-') {
            b = parse_currency(p);
            result = (op == '+') ? add(a, b) : subtract(a, b);
        } else if (op == '*' || op == '/' || op == ':') {
            factor = strtod(p, &rest);
            result = (op == '*') ? multiply(a, factor) : divide(a, factor);
        } else {
            printf(INVALID_OP "\n");
            continue;
        }

        printf(" ----------\n = ");
        print_currency(result);
        continueWithResult = 1;
    }

    printf(GOOD_BYE "\n");
    return 0;
}

