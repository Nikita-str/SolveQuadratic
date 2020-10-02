#include "cpp_ReadDouble.h"

int read_line_to_doubles(double *ds, int max_amount, int *amount)
{
    assert(amount);
    assert(ds);
    
    //*amount = 0;

    std::string str;
    if (!std::getline(std::cin, str, '\n')) {
        return ERR_STREAM;
    }
    size_t len = str.length();

    //IEEE 754
    //precision starts to be lost after 52 bits:
    constexpr long long unsigned BAD_BIT_MASK = (0xFFFLL) << 52;//0xFFF0_0000_0000_0000LL; 
    long long unsigned fraction = 1;
    long long unsigned now_bits = 0;
    bool sign = 0;
    int stage = 0;

    for (size_t i = 0; i < len;  i++) {
        char c = str[i];
        if (isspace(c)) {
            if (stage == FIRST_STAGE) {
                continue;
            }
            ds[(*amount)++] = (sign ? -1.0 : 1.0) * ((double)now_bits / (double)fraction);
            fraction = 1;
            now_bits = 0;
            sign = 0;
            stage = 0;
            continue;
        }
        bool c_check = 0;
        if (c == '-') {
            if (stage != FIRST_STAGE) {
                return ERR_BAD_INPUT;
            }
            sign = 1;
            stage = SIGN_STAGE;
            c_check = 1;
        }
        if ('0' <= c && c <= '9') {
            if (stage < PRE_DOT_STAGE) {
                stage = PRE_DOT_STAGE;
            }
            now_bits = (now_bits * 10) + c - '0';
            if ((now_bits & BAD_BIT_MASK) != 0) {
                return ERR_LOST_PRECISION;
            }
            if (stage == AFTER_DOT_STAGE) {
                fraction *= 10;
            }
            c_check = 1;
        }
        if (c == '.') {
            if (stage != PRE_DOT_STAGE) {
                return ERR_BAD_INPUT;
            }
            stage = AFTER_DOT_STAGE;
            c_check = 1;
        }
        if (*amount == max_amount) {
            return ERR_TOO_MANY_READ;
        }
        if (!c_check) {
            return ERR_BAD_INPUT;
        }
    }

    if (stage == SIGN_STAGE) {
        return ERR_BAD_INPUT;
    }
    if (stage != FIRST_STAGE) {
        ds[(*amount)++] = (sign ? -1.0 : 1.0) * ((double)now_bits / (double)fraction);
    }

    return NO_ERR;
}