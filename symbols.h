#define	NONE		-1
#define	EOL		0
#define	INTEGER		1
#define	FLOAT		2
#define	STRING		3
#define	LPAREN		4
#define RPAREN		5
#define	PLUS		6
#define	MINUS		7
#define	TIMES		8
#define	DIVIDE		9
#define EQUAL		10
#define	VARIABLE	11
#define LET		12
#define IF		13
#define PRINT		14
#define GOTO		15
#define GOSUB		16
#define	END_OF_FILE	9999

#define MAXLINE		1024
#define	MAXSTRING	256

typedef struct {
    char	line[MAXLINE];
    char	*curp;
    int		curch;
    int		nextch;
} line_t;

typedef union {
    int		intval;
    double	floatval;
    char	strval[MAXSTRING];
} value_u;

typedef struct {
    int		symtype;
    value_u	value;
} symbol_t;	

extern int get_line(FILE *fp);
extern symbol_t *getsym();
extern int get_character();
