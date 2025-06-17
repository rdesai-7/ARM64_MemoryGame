#include "pass_two.h"

instruction_entry_t instruction_table[] = {
    {"add", assemble_add_sub_instruction},
    {"adds", assemble_add_sub_instruction},
    {"sub", assemble_add_sub_instruction},
    {"subs", assemble_add_sub_instruction},
    {"cmp", cmp_assembly},
    {"cmn", cmn_assembly},
    {"neg", neg_assembly},
    {"negs", negs_assembly},
    {"and", bit_logic_assembly},
    {"ands", bit_logic_assembly},
    {"bic", bit_logic_assembly},
    {"bics", bit_logic_assembly},
    {"eor", bit_logic_assembly},
    {"orr", bit_logic_assembly},
    {"eon", bit_logic_assembly},
    {"orn", bit_logic_assembly},
    {"tst", tst_assembly},
    {"movk", mov_wide_assembly},
    {"movn", mov_wide_assembly},
    {"movz", mov_wide_assembly},
    {"mov", mov_assembly},
    {"mvn", mvn_assembly},
    {"madd", multiply_assembly},
    {"msub", multiply_assembly},
    {"mul", mul_assembly},
    {"mneg", mneg_assembly},
    {"b", parse_b_unconditional},
    {"b.cond", parse_b_conditional},
    {"br", parse_br_register},
    {"str", parse_loadstore},
    {"ldr", parse_loadstore},
    {NULL, NULL},
};

static void tokenize(char *str, char *delim, char *tokens[], int *num_tokens) {
    char *token;
    char *save_ptr = NULL;
    int curr_token_count = 0;

    token = strtok_r(str, delim, &save_ptr);
    while (token != NULL) {
        tokens[curr_token_count] = token;
        curr_token_count++;
        token = strtok_r(NULL, delim, &save_ptr);
    }
    *num_tokens = curr_token_count;
}

bool run_pass_two( const char *filename, ARM_STATE *state) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error Opening File");
        return false;
    }

    char line_buffer[LINE_BUFFER];

    while( fgets(line_buffer, sizeof(line_buffer), file) != NULL ) {
        char *line = trim_whitespace(line_buffer);
        char label_name[MAX_LABEL_LENGTH];

        if( !is_line_empty(line) && !is_label(line, label_name)) {

            //deal with .int directives
            if (is_directive(line)) {
                uint32_t N = parse_directive(line);
                state->binaryInstructions[state->currAddress / ADDR_INCREMENT] = N;
            } else {

                //Tokenise the line
                char *tokens[MAX_TOKENS];
                int num_tokens = 0;
                tokenize(line, ", ", tokens, &num_tokens);

                char *current_mnemonic = (strncmp(tokens[0], "b.", 2) == 0) ? "b.cond" : tokens[0]; //Generalises b.cond mnemonic

                //Select correct parse function based on mnemonic and parse instruction
                uint32_t curr_instruction;
                bool instr_assigned = false;
                for (int i = 0; instruction_table[i].mnemonic != NULL; i++) {
                    if (strcmp(current_mnemonic, instruction_table[i].mnemonic) == 0) {
                        curr_instruction = instruction_table[i].parser(tokens, num_tokens, state);
                        instr_assigned = true;
                        break;
                    }
                }
                if (!instr_assigned) {
                    fprintf(stderr, "Mnemonic \"%s\" doesn't match to any entries in the lookup table",tokens[0]);
                    return false;
                } else {
                    //append current instruction to array
                    state->binaryInstructions[state->currAddress / ADDR_INCREMENT] = curr_instruction;
                } 
            }

            state->currAddress += ADDR_INCREMENT;
        }
    }

    // Standard error checking
    if (ferror(file)) {
        fprintf(stderr, "Error reading from file");
        fclose(file);
        return false;
    }

    fclose(file);
    return true;
}