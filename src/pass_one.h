#define LINE_BUFFER 256
#define ADDR_INCREMENT 4
#define MAX_LABEL_LENGTH 128 
// is max_label_length a good size? idk

extern char* trim_whitespace(char *s);
extern bool is_line_empty(const char *line);
extern bool is_label(const char *line, char *label_name_out);
extern bool run_pass_one( const char *filename, SymbolTable_t st, uint32_t *program_size );