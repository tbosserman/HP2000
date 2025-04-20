#define MAX_TOKENS	250

extern void parse_line();
extern void store_token(symbol_t *tokens, int num_tokens, symbol_t *sym);

typedef struct {
    int		linenum;
    int		num_tokens;
    symbol_t	*tokens[];
} progline_t;

typedef struct {
    int		num_lines;
    progline_t	*lines[];
} program_t;
