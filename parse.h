extern void parse_line();

typedef struct {
    int		linenum;
    int		num_tokens;
    symbol_t	*tokens[];
} progline_t;

typedef struct {
    int		num_lines;
    progline_t	*lines[];
} program_t;
