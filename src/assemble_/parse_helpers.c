#include "parse_helpers.h"

char* trim_whitespace(char *s) {

    while( isspace(*s) ) s++;
    if(*s == '\0') return s;

    char *end = s + strlen(s) - 1;
    // Removes whitespace from the end
    while ( (end > s && isspace(*end)) ) end--;
    *(end + 1) = '\0';
    return s;
}

bool is_line_empty(const char *line) {
    if (line == NULL || *line == '\0') return true;
    return false;
}

bool is_label(const char *line, char *label_name_out) {
    // Trim whitespace should already be called

    // No need to check first character is valid label character  
    size_t len = strlen(line);
    if (len - 1 >= MAX_LABEL_LENGTH) {
        fprintf(stderr, "Label too long to fit");
        return false;
    }

    // Check last character is colon
    // Label_name_out will have enough space since it will be initialised with MAX_LABEL_LENGTH
    if( line[len - 1] == ':' ) {
        strncpy( label_name_out, line, len -1);
        label_name_out[len - 1] = '\0';
        return true;
    }

    return false;
}

bool is_directive(char *s) {
    return strncmp(s, ".int", 4) == 0;
}

uint32_t parse_directive(char *line) {
    // PRE: is_directive(line) returns true
    int value;
    // sscanf parses hex and decimal values using %i
    assert(sscanf(line, ".int %i", &value) == 1);
    return (uint32_t) value;
}

uint32_t parse_imm(char *token) {
    // PRE: token is of the form "#imm"
    // trailing chars ignored
    long val = strtol(token + 1, NULL, 0);
    return (uint32_t) val;
}

uint32_t parse_simm(char *token, int num_bits) {
    // PRE: token is of the form "#simm"
    long val = strtol(token + 1, NULL, 0);
    return (uint32_t) val & ((1U << num_bits) - 1);
}

uint32_t parse_shift_type(const char* shift_type_str) {
    if (strcmp(shift_type_str, "lsl") == 0) {
            return 0x0;
        } else if (strcmp(shift_type_str, "lsr") == 0) {
            return 0x1;
        } else if (strcmp(shift_type_str, "asr") == 0) {
            return 0x2;
        } else if (strcmp(shift_type_str, "ror") == 0) {
            return 0x3;
        } else {
            fprintf(stderr, "Error: Invalid shift type '%s'\n", shift_type_str);
            return 0;
        }

}

uint32_t parse_register_token(char* reg_token, uint32_t* sf_bit) {
    
    if (strcmp(reg_token, "sp") == 0 || strcmp(reg_token, "xzr") == 0) {
        if (sf_bit != NULL) {
            *sf_bit = 1; 
        }
        return ZERO_REGISTER; 
    }

    if (strcmp(reg_token, "wsp") == 0 || strcmp(reg_token, "wzr") == 0) {
        if (sf_bit != NULL) {
            *sf_bit = 0; 
        }
        return ZERO_REGISTER; 
    }

    if (strcmp(reg_token, "ZR") == 0) {
        return ZERO_REGISTER; 
    }

    char reg_type = reg_token[0];
    uint32_t reg_num = strtoul(reg_token + 1, NULL, 10); 

    if (sf_bit != NULL) {
        if (reg_type == 'W' || reg_type == 'w') {
            *sf_bit = 0; // 32-bit register
        } else {
            *sf_bit = 1; // 64-bit register
        }
    }
    return reg_num;
}