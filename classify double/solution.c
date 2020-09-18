#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>>
#include <math.h>

/**
 * Library-level functions.
 * You should use them in the main sections.
 */

uint64_t convertToUint64 (double number) {
    return *((uint64_t *)(&number));
}

bool getBit (const uint64_t number, const uint8_t index) {
    uint64_t mask = 1;
    mask <<= index;
    uint64_t temp = mask & number;
    temp >>= index;
    return temp;
}


/**
 * Checkers here:
 */

bool checkForPlusZero (uint64_t number) {
    for (uint8_t i = 0; i < 64; ++i) {
        if (getBit(number, 63 - i) != 0)
            return 0;
    }
    return 1;
}

bool checkForMinusZero (uint64_t number) {
    if (getBit(number, 63) == 1) {
        for (uint8_t i = 0; i < 63; ++i)
            if (getBit(number, 62 - i) != 0)
                return 0;
        return 1;
    } else
        return 0;
}

bool checkForPlusInf (uint64_t number) {
    if (getBit(number, 63) == 0) {
        for (uint8_t i = 52; i < 63; i++)
            if (getBit(number, i) != 1)
                return 0;
        for (uint8_t i = 0; i < 52; i++) {
            if (getBit(number, i) != 0)
                return 0;
        }
        return 1;
    } else
        return 0;
}

bool checkForMinusInf (uint64_t number) {
    if (getBit(number, 63) == 1) {
        for (uint8_t i = 52; i < 63; i++)
            if (getBit(number, i) != 1)
                return 0;
        for (uint8_t i = 0; i < 52; i++) {
            if (getBit(number, i) != 0)
                return 0;
        }
        return 1;
    } else
        return 0;
}

bool checkForPlusNormal (uint64_t number) {
    uint8_t unique_bit = 0;
    if (getBit(number, 63) == 0) {
        for (uint8_t i = 52; i < 63; i++) {
            if (getBit(number, i) == 0)
                unique_bit++;
        }

        if (unique_bit > 0 && unique_bit < 11)
            return 1;
        else
            return 0;
    } else
        return 0;
}

bool checkForMinusNormal (uint64_t number) {
    uint8_t unique_bit = 0;
    if (getBit(number, 63) == 1) {
        for (uint8_t i = 52; i < 63; i++) {
            if (getBit(number, i) == 0)
                unique_bit++;
        }

        if (unique_bit > 0 && unique_bit < 11)
            return 1;
        else
            return 0;
    } else
        return 0;
}

bool checkForPlusDenormal (uint64_t number) {
    if (getBit(number, 63) == 0) {
        for (uint8_t i = 52; i < 63; i++) {
            if (getBit(number, i) != 0)
                return 0;
        }
        uint8_t unique_bit = 0;
        for (uint8_t i = 0; i < 52; i++) {
            if (getBit(number, i) == 0)
                unique_bit++;
        }

        if (unique_bit > 0 && unique_bit < 11)
            return 0;
        else
            return 1;

    } else
        return 0;
}

bool checkForMinusDenormal (uint64_t number) {
    if (getBit(number, 63) == 1) {
        for (uint8_t i = 52; i < 63; i++) {
            if (getBit(number, i) != 0)
                return 0;
        }
        uint8_t unique_bit = 0;
        for (uint8_t i = 0; i < 52; i++) {
            if (getBit(number, i) == 0)
                unique_bit++;
        }

        if (unique_bit > 0 && unique_bit < 11)
            return 0;
        else
            return 1;

    } else
        return 0;
}

bool checkForSignalingNan (uint64_t number) {
    for (uint8_t i = 52; i < 63; i++) {
        if (getBit(number, i) != 1)
            return 0;
    }
    uint8_t unique_bit = 0;
    for (uint8_t i = 0; i < 52; i++) {
        if (getBit(number, i) == 0)
            unique_bit++;
    }

    if (unique_bit > 0 && unique_bit < 11)
        return 1;
    else
        return 0;
}

bool checkForQuietNan (uint64_t number) {
    for (uint8_t i = 52; i < 63; i++) {
        if (getBit(number, i) != 1)
            return 0;
    }

    if (getBit(number, 51) == 1)
        return 1;
    else
        return 0;
}


void classify (double number) {
    if (checkForPlusZero(convertToUint64(number))) {
        printf("Plus zero\n");
    }

    else if (checkForMinusZero(convertToUint64(number))) {
        printf("Minus zero\n");
    }

    else if (checkForPlusInf(convertToUint64(number))) {
        printf("Plus inf\n");
    }

    else if (checkForMinusInf(convertToUint64(number))) {
        printf("Minus inf\n");
    }

    else if (checkForPlusNormal(convertToUint64(number))) {
        printf("Plus normal\n");
    }

    else if (checkForMinusNormal(convertToUint64(number))) {
        printf("Minus normal\n");
    }

    else if (checkForPlusDenormal(convertToUint64(number))) {
        printf("Plus Denormal\n");
    }

    else if (checkForMinusDenormal(convertToUint64(number))) {
        printf("Minus Denormal\n");
    }

    else if (checkForSignalingNan(convertToUint64(number))) {
        printf("Signailing NaN\n");
    }

    else if (checkForQuietNan(convertToUint64(number))) {
        printf("Quiet NaN\n");
    }

    else {
        printf("Error.\n");
    }
}
