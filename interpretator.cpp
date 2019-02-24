/*
Model language

H	>   O {EOL O}
O	>  	N | I | W | P
N	>	Name [ = | += | -= | *= | /= | %= ] [ gets | E | '[ E {, E}'] ]
I	>	if E then O {EOL O} [else O {EOL O}] end
W	>	while E do O {EOL O} end | for Name in '[ Name | Number ']..'[ Name | Number '] do O {EOL O} end
P	>	puts(E {, E})
E	>  	E1 [ == | != | < | > | <= | >= ] E1 | E1
E1 	>  	T {[ + | - | || ] T}
T	>  	F {[ * | / | && | % ] F}
F	>  	Name | Number | String | true | false | !E | (E)
*/

#include <iostream>
#include <cstring>
#include <cstdio>
#include <ctype.h>
#include <cstdlib>

using namespace std;

enum type_of_lex {
	LEX_NULL,
	LEX_DO, LEX_ELSE, LEX_END, LEX_IF, LEX_FALSE, LEX_TRUE, LEX_INT, LEX_BOOL,
	LEX_THEN, LEX_WHILE, LEX_FOR, LEX_IN, LEX_PUTS, LEX_GETS, LEX_CHAR, LEX_DOUBLE, 
	LEX_FIN,
	LEX_COMMA, LEX_DOT, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS, LEX_GTR,
	LEX_PLUS, LEX_MINUS, LEX_MUL, LEX_DIV, LEX_MOD, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_QUOT,
	LEX_PLUSEQ, LEX_MINUSEQ, LEX_MULEQ, LEX_DIVEQ, LEX_MODEQ, LEX_EOL, LEX_LSQBR, LEX_RSQBR,
	LEX_AND, LEX_OR, LEX_NOT,
	LEX_NUM,
	LEX_ID,
	LEX_STR,
	POLIZ_LABEL,
	POLIZ_ADDRESS,
	POLIZ_GO,
	POLIZ_FGO
};

const char *lex_names[] = {
	"LEX_NULL",
	"LEX_DO", "LEX_ELSE", "LEX_END", "LEX_IF", "LEX_FALSE", "LEX_TRUE", "LEX_INT", "LEX_BOOL",
	"LEX_THEN", "LEX_WHILE", "LEX_FOR", "LEX_IN", "LEX_PUTS", "LEX_GETS", "LEX_CHAR", "LEX_DOUBLE",
	"LEX_FIN",
	"LEX_COMMA", "LEX_DOT", "LEX_ASSIGN", "LEX_LPAREN", "LEX_RPAREN", "LEX_EQ", "LEX_LSS", "LEX_GTR",
	"LEX_PLUS", "LEX_MINUS", "LEX_MUL", "LEX_DIV", "LEX_MOD", "LEX_LEQ", "LEX_NEQ", "LEX_GEQ", "LEX_QUOT",
	"LEX_PLUSEQ", "LEX_MINUSEQ", "LEX_MULEQ", "LEX_DIVEQ", "LEX_MODEQ", "LEX_EOL", "LEX_LSQBR", "LEX_RSQBR",
	"LEX_AND", "LEX_OR", "LEX_NOT",
	"LEX_NUM",
	"LEX_ID",
	"LEX_STR",
	"POLIZ_LABEL",
	"POLIZ_ADDRESS",
	"POLIZ_GO",
	"POLIZ_FGO"
};

///////////////////////// Класс Lex //////////////////////////

class Lex {
	type_of_lex t_lex;
	int v_lex;
	double d_lex;
	char *s_lex;
public:
	Lex(type_of_lex t = LEX_NULL, int v = 0, double d = 0, const char *s = NULL): t_lex(t),v_lex(v), d_lex(d) {
		if (s) {
			s_lex = new char [strlen(s)+1];
			strcpy(s_lex,s);
		}
		else s_lex = NULL;
	}
	type_of_lex get_type() { return t_lex; }
	int get_value() { return v_lex; }
	double get_dvalue() { return d_lex; }
	char *get_str() { return s_lex; }
	/*friend ostream &operator<<(ostream &s, Lex l) {
		s <<'(' << l.t_lex <<',' << l.v_lex <<");" ;
		return s;
	}*/
	friend ostream &operator<<(ostream &s, Lex l) {
		//s << lex_names[l.t_lex] << ',' << l.v_lex << "\n" ;
		s << lex_names[l.t_lex] << " ";
		l.s_lex ? (s << l.s_lex) : (s << l.v_lex);
		s << endl;
		return s;
	}
};

///////////////////// Класс Ident ////////////////////////////

class Ident {
	char *name;
	bool declare;
	type_of_lex type;
	bool assign;
	int value;
	double dval;
	char *st;
public:
	Ident() { declare = false; assign = false; st = NULL; dval = 0; value = 0;}
	char *get_name() { return name; }
	void put_name(const char *n) {
		name = new char [strlen(n)+1];
		strcpy(name,n);
	}
	bool get_declare() { return declare; }
	void put_declare() { declare = true; }
	
	type_of_lex get_type() { return type; }
	void put_type(type_of_lex t) { type = t; }
	
	bool get_assign() { return assign; }
	void put_assign() { assign = true; }
	
	int get_value() { return value; }
	void put_value(int v) { value = v; }
	
	double get_dvalue() { return dval; }
	void put_dvalue(double d) { dval = d; }
	
	char *get_str() { return st; }
	void put_str(char *s) {
	    if (s) {
			st = new char [strlen(s)+1];
			strcpy(st,s);
		}
		else st = NULL;
	}
};

////////////////////// Класс Tabl_ident ///////////////////////

class Tabl_ident {
	Ident *p;
	int size;
	int top;
public:
	Tabl_ident(int max_size) {
		p = new Ident[size = max_size];
		top = 1;
	}
	~Tabl_ident() { delete [] p; }
	Ident &operator[](int k) { return p[k]; }
	int put(const char *buf);
};

int Tabl_ident::put(const char *buf)
{
	for(int j = 1; j <top; j++)
		if (!strcmp(buf, p[j].get_name()))
			return j;
	p[top].put_name(buf);
	++top;
	return top-1;
}

/////////////////////////////////////////////////////////////////

template <class T, int max_size> class Stack {
	T s[max_size];
	int top;
public:
	Stack() { top = 0; }
	void reset() { top = 0; }
	void push(T i);
	T pop();
	bool is_empty() { return top == 0; }
	bool is_full() { return top == max_size; }
};

template <class T, int max_size> void Stack <T, max_size> :: push(T i) {
	if (!is_full())
		s [top++] = i;
	else
		throw "Stack_is_full";
}

template <class T, int max_size> T Stack <T, max_size> :: pop() {
	if (!is_empty())
		return s[--top];
	else
		throw "stack_is_empty";
}

class MyElem {
	int v;
	double d;
	char *s;
	type_of_lex t;
public:
	MyElem() { s = NULL; d = 0; v = 0;}	
	type_of_lex get_type() { return t; }
	void put_type(type_of_lex type) { t = type; }

	int get_value() { return v; }
	void put_value(int val) { v = val; }
	
	double get_dvalue() { return d; }
	void put_dvalue(double dval) { d = dval; }
	
	char *get_str() { return s; }
	void put_str(char *st) {
	    if (st) {
			s = new char [strlen(st)+1];
			strcpy(s,st);
		}
		else st = NULL;
	}
	
};
///////////////////////// Класс Poliz /////////////////////////////

class Poliz {
	Lex *p;
	int size;
	int free;
public:
	Poliz(int max_size) {
		p = new Lex [size = max_size];
		free = 0;
	}
	~Poliz() { delete [] p; }
	void put_lex(Lex l) {
		p [free] = l;
		free++;
	}
	void put_lex(Lex l, int place) { p [place] = l; }
	void blank() { free++; }
	int get_free() { return free; }
	Lex &operator[](int index) {
		if (index > size)
			throw "POLIZ:out of array";
		else if (index > free)
			throw "POLIZ:indefinite element of array";
		else
			return p[index];
	}
	void print() {
		for (int i = 0; i < free; ++i)
		cout << i << " " << p[i];
	}
};

////////////////////////////////////////////////////////////////////

Tabl_ident TID(100);

///////////////////// Класс Scanner //////////////////////////////

class Scanner {
	enum state { H, IDENT, NUMB, COM, NLG, AND, OR, STR, DELIM };
	static const char *TW [];
	static type_of_lex words [];
	static const char *TD [];
	static type_of_lex dlms [];
	state CS;
	FILE *fp;
	char c;
	char buf[80];
	int buf_top;
	void clear() {
		buf_top = 0;
		for (int j = 0; j < 80; j++)
		buf[j] = '\0';
	}
	void add() { buf[buf_top++] = c; }
	int look(const char *buf, const char **list) {
		int i = 0;
		while(list[i]) {
			if (!strcmp(buf, list[i]))
				return i;
			++i;
		}
		return 0;
	}
	void gc() { c = fgetc(fp); }
public:
	Scanner(const char *program) {
		fp = fopen(program, "r");
		CS = H;
		clear();
		gc();
	}
	Lex get_lex();
};

const char *Scanner::TW [] = {
	"", "do", "else", "end", "if", "false", "true", "int", "bool",
	"then", "while", "for", "in", "puts", "gets", "char", "double", NULL
};

type_of_lex Scanner::words [] = {
	LEX_NULL, LEX_DO, LEX_ELSE, LEX_END, LEX_IF, LEX_FALSE, LEX_TRUE, LEX_INT, LEX_BOOL,
	LEX_THEN, LEX_WHILE, LEX_FOR, LEX_IN, LEX_PUTS, LEX_GETS, LEX_CHAR, LEX_DOUBLE, LEX_NULL
};

const char *Scanner::TD [] = {
	"", "@", "\n", ",", ".", "=",
	"(", ")", "==", "<", ">",
	"+", "-", "*", "/", "%",
	"+=", "-=", "*=", "/=", "%=",
	"<=", "!=", ">=", "\"",
	"&&", "||", "!", "[", "]", NULL
};

type_of_lex Scanner::dlms [] = {
	LEX_NULL, LEX_FIN, LEX_EOL, LEX_COMMA, LEX_DOT, LEX_ASSIGN,
	LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS, LEX_GTR,
	LEX_PLUS, LEX_MINUS, LEX_MUL, LEX_DIV, LEX_MOD,
	LEX_PLUSEQ, LEX_MINUSEQ, LEX_MULEQ, LEX_DIVEQ, LEX_MODEQ,
	LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_QUOT,
	LEX_AND, LEX_OR, LEX_NOT, LEX_LSQBR, LEX_RSQBR, LEX_NULL
};

Lex Scanner::get_lex() {
	int d, j, fl = 0;
	double l, t;

	CS = H;
	do {
		switch(CS) {
			case H:
				if (c ==' ' || c== '\r' || c == '\t')
					gc();
				else if (isalpha(c) || c == '_') {
					clear();
					add();
					gc();
					CS = IDENT;
				}
				else if (isdigit(c)) {
					d = c - '0';
					gc();
					CS = NUMB;
				}
				else if (c == '#') {
					gc();
					CS = COM;
				}
				else if (c == '!' || c == '<' || c == '>' || c == '+' ||
						c == '-' || c == '*' || c == '/' || c == '%' || c == '=') {
					clear();
					add();
					gc();
					CS = NLG;
				}
				else if (c == '&') {
					clear();
					add();
					gc();
					CS = AND;
				}
				else if (c == '|') {
					clear();
					add();
					gc();
					CS = OR;
				}
				else if ( c == '"') {
					clear();
					gc();
					CS = STR;
				}
				else if (c == EOF)
					return Lex(LEX_FIN);
				else
					CS = DELIM;
				break;

			case IDENT:
				if (isalpha(c) || isdigit(c) || c == '_') {
					add();
					gc();
				}
				else if (j = look(buf, TW))
					return Lex(words[j], j);
				else {
					j = TID.put(buf);
					return Lex(LEX_ID, j);
				}
				break;

			case NUMB:
				if (isdigit(c) && !fl) {
					d = d * 10 + (c - '0');
					gc();
				}
				else if (isdigit(c) && fl) {
					l = (c - '0');
					for ( int j = 0; j < fl; j++)
						l /= 10;
					t += l;
					fl++;
					gc();
				}
				else if (c == '.') {
				    fl = 1;
				    t = d;
				    gc();
				}
				else {
                    if (!fl) return Lex(LEX_NUM, d);
                    else
                    	return Lex(LEX_DOUBLE, 0, t);
				}
				break;

			case COM:
				if (c == '\n' || c == EOF) {
					gc();
					CS = H;
				}
				else
					gc();
				break;

			case NLG:
				if (c == '=') {
					add();
					gc();
					j = look(buf, TD);
					return Lex(dlms[j], j);
				}
				else {
					j = look(buf, TD);
					return Lex(dlms[j], j);
				}
				break;

			case AND:
				if (c == '&') {
					add();
					gc();
					j = look(buf, TD);
					return Lex(LEX_AND, j);
				}
				else
					throw "!";
				break;

			case OR:
				if (c == '=') {
					add();
					gc();
					j = look(buf, TD);
					return Lex(LEX_OR, j);
				}
				else
					throw "!";
				break;

			case STR:
				if ( c != '"') {
					add();
					gc();
				}
				else {
					gc();
					CS = H;
					return Lex(LEX_STR, 0, 0, buf);
				}
				break;

			case DELIM:
				clear();
				add();
				if ((j = look(buf, TD))) {
					gc();
					return Lex(dlms[j], j);
				}
				else
					throw c;
				break;
		} //end switch
	} while (true);
}

/////////////////////////// Класс Parser /////////////////////////////////

class Parser {
	Lex curr_lex;
	type_of_lex c_type;
	int c_val;
	char *c_str;
	Scanner scan;

	void H();
	void O();
	void N();
	void I();
	void W();
	void P();
	void E();
	void E1();
	void T();
	void F();

	void gl() {
		curr_lex = scan.get_lex();
		c_type = curr_lex.get_type();
		c_val = curr_lex.get_value();
		c_str = curr_lex.get_str();
		//cout << c_type << " ";
		//cout << lex_names[c_type] << endl;
	}

public:
	Poliz prog;
	Parser(const char *program): scan(program), prog(1000) {}
	void analyze();
};

void Parser::analyze() {
	gl();
	/*while ( c_type != LEX_FIN ) {
		gl();
	}*/
	H();
	//prog.print(); cout << endl;
	cout << "Compilation ended!!! <br />" << endl;
}

void Parser::H() {
	O();
	while (c_type == LEX_EOL) {
		gl();
		O();
	}
	if (c_type != LEX_FIN)
		throw curr_lex;
}

void Parser::O() {
	if (c_type == LEX_ID) {
		prog.put_lex(Lex(POLIZ_ADDRESS, c_val));
		N();
	}
	else if (c_type == LEX_IF) {
		gl();
		I();
	}
	else if (c_type == LEX_WHILE || c_type == LEX_FOR) {
		W();
	}
	else if (c_type == LEX_PUTS) {
		gl();
		P();
	}
	else if (c_type == LEX_EOL) {
		gl();
		O();
	}
}

void Parser::N()  {
	type_of_lex c;
	int v = c_val;

		/*if (c_type == LEX_GETS) gl();
		else if (c_type == LEX_LSQBR) {
			gl();
			E();
			while(c_type == LEX_COMMA) {
				gl();
				E();
			}
			if (c_type != LEX_RSQBR)
				throw curr_lex;
			gl();
		}
		else */

    gl();
	if (c_type == LEX_PLUSEQ || c_type == LEX_MINUSEQ || c_type == LEX_MULEQ ||
        c_type == LEX_DIVEQ || c_type == LEX_MODEQ) {
        c = c_type;
        prog.put_lex(Lex(LEX_ID, v));
		gl();
		E();
		switch(c) {
		    case LEX_PLUSEQ: prog.put_lex(Lex(LEX_PLUS)); break;
		    case LEX_MINUSEQ: prog.put_lex(Lex(LEX_MINUS)); break;
		    case LEX_MULEQ: prog.put_lex(Lex(LEX_MUL)); break;
		    case LEX_DIVEQ: prog.put_lex(Lex(LEX_DIV)); break;
		    case LEX_MODEQ: prog.put_lex(Lex(LEX_MOD)); break;
		}
	}
	else if (c_type == LEX_ASSIGN) {
	    gl();
	    E();
	}
	else
		throw curr_lex;

    prog.put_lex(Lex(LEX_ASSIGN));
}

void Parser::I() {
	int pl0, pl1, pl2, pl3;

	E();

	pl2 = prog.get_free();
	prog.blank();
	prog.put_lex(Lex(POLIZ_FGO));

	if (c_type == LEX_THEN) {
		gl();
		O();
		while (c_type == LEX_EOL) {
			gl();
			O();
		}

		pl3 = prog.get_free();
		prog.blank();
		prog.put_lex(Lex(POLIZ_GO));
		prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl2);

		if (c_type == LEX_ELSE) {
			gl();
			O();
			while (c_type == LEX_EOL) {
				gl();
				O();
			}

			if (c_type != LEX_END)
				throw curr_lex;

			prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl3);
		}
		else if (c_type == LEX_END)
			prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl3);
		else throw curr_lex;
	}
	else
		throw curr_lex;
	gl();
}

void Parser::W() {
	int pl0, pl1, pl2, pl3;

	if (c_type == LEX_WHILE) {

		pl0=prog.get_free();

		gl();
		E();

		pl1=prog.get_free();
		prog.blank();
		prog.put_lex(Lex(POLIZ_FGO));

		if (c_type == LEX_DO) {
			gl();
			O();
			while (c_type == LEX_EOL) {
				gl();
				O();
			}

			prog.put_lex(Lex(POLIZ_LABEL, pl0));
			prog.put_lex(Lex(POLIZ_GO));
			prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl1);
		}
		else if (c_type != LEX_END)
			throw curr_lex;
	}
	else {
		type_of_lex id;
		int val;
		gl();
		if (c_type == LEX_ID) {
			val = c_val;
			prog.put_lex(Lex(POLIZ_ADDRESS, val));
			gl();

			if (c_type == LEX_IN) {
				gl();

				if (c_type == LEX_ID || c_type == LEX_NUM) {
					if (c_type == LEX_ID) prog.put_lex(Lex(LEX_ID, c_val));
					else prog.put_lex(curr_lex);
					prog.put_lex(Lex(LEX_ASSIGN));
					gl();

					if (c_type == LEX_DOT) {
						gl();
						if (c_type == LEX_DOT) {
							gl();
							if (c_type == LEX_ID || c_type == LEX_NUM) {
								pl0=prog.get_free();

								prog.put_lex(Lex(LEX_ID, val));
								if (c_type == LEX_ID) prog.put_lex(Lex(LEX_ID, c_val));
								else prog.put_lex(curr_lex);
								prog.put_lex(Lex(LEX_LEQ));
								gl();

								pl1=prog.get_free();
								prog.blank();
								prog.put_lex(Lex(POLIZ_FGO));

								if (c_type == LEX_DO) {
									gl();
									O();
									while (c_type == LEX_EOL) {
										gl();
										O();
									}

									prog.put_lex(Lex(POLIZ_ADDRESS, val));
									prog.put_lex(Lex(LEX_ID, val));
									prog.put_lex(Lex(LEX_NUM, 1));
									prog.put_lex(Lex(LEX_PLUS));
									prog.put_lex(Lex(LEX_ASSIGN));

									prog.put_lex(Lex(POLIZ_LABEL, pl0));
									prog.put_lex(Lex(POLIZ_GO));
									prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl1);

									if (c_type != LEX_END)
										throw curr_lex;
								}
								else
									throw curr_lex;
							}
							else
								throw curr_lex;
						}
						else
							throw curr_lex;
					}
					else
						throw curr_lex;
				}
				else
					throw curr_lex;
			}
			else
				throw curr_lex;

		}
		else
			throw curr_lex;
	}
	gl();
}

void Parser::P() {
	if (c_type == LEX_LPAREN) {
		gl();
		E();
		while (c_type == LEX_COMMA) {
			gl();
			prog.put_lex(Lex(LEX_PUTS));
			E();
		}
		if (c_type == LEX_RPAREN) {
			gl();
			prog.put_lex(Lex(LEX_PUTS));
		}
		else
			throw curr_lex;
	}
	else
		throw curr_lex;
}
            
void Parser::E() {
	type_of_lex c;
	E1();
	if (c_type == LEX_EQ || c_type == LEX_LSS || c_type == LEX_GTR ||
	c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ) {
		c = c_type;
		gl();
		E1();
		prog.put_lex(Lex(c));
	}
}

void Parser::E1() {
	type_of_lex c;
	T();
	while (c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR) {
		c = c_type;
		gl();
		T();
		prog.put_lex(Lex(c));
	}
}

void Parser::T() {
	type_of_lex c;
	F();
	while (c_type == LEX_MUL || c_type == LEX_DIV || c_type == LEX_AND || c_type == LEX_MOD) {
		c = c_type;
		gl();
		F();
		prog.put_lex(Lex(c));
	}
}

void Parser::F() {
	if (c_type == LEX_ID) {
		prog.put_lex(Lex(LEX_ID, c_val));
		gl();
	}
	else if (c_type == LEX_NUM) {
		prog.put_lex(curr_lex);
		gl();
	}
	else if (c_type == LEX_DOUBLE) {
		prog.put_lex(curr_lex);
		gl();
	}
	else if (c_type == LEX_TRUE) {
		prog.put_lex(Lex(LEX_TRUE, 1));
		gl();
	}
	else if (c_type == LEX_FALSE) {
		prog.put_lex(Lex(LEX_FALSE, 0));
		gl();
	}
	else if (c_type == LEX_NOT) {
		gl();
		E();
		prog.put_lex(Lex(LEX_NOT));
	}
	else if (c_type == LEX_STR) {
		prog.put_lex(curr_lex);
		gl();
	}
	else if (c_type == LEX_LPAREN) {
		gl();
		E();
		if (c_type == LEX_RPAREN)
			gl();
		else
			throw curr_lex;
	}
	else
		throw curr_lex;
}

////////////////////////////////////////////////////////////////

class Executer {
	Lex pc_el;
public:
	void execute(Poliz & prog);
};

void Executer::execute(Poliz & prog)
{
	Stack <MyElem, 100> args;
	int index = 0, size = prog.get_free();
	MyElem i, j;
	while (index < size) {
		MyElem el;
		int q;
		pc_el = prog[index];
		switch (pc_el.get_type()) {
			
			case LEX_TRUE: case LEX_FALSE: case LEX_NUM:
				el.put_value(pc_el.get_value());
				//cout << pc_el.get_value() << endl;
				el.put_type(LEX_INT);
				args.push(el);
				break;
				
			case LEX_DOUBLE:
				el.put_dvalue(pc_el.get_dvalue());
				//cout << pc_el.get_dvalue() << endl;
				el.put_type(LEX_DOUBLE);
				args.push(el);
				break;
					
			case LEX_STR:
				el.put_str(pc_el.get_str());
				//cout << pc_el.get_str() << endl;
				el.put_type(LEX_CHAR);
				args.push(el);
				break;
				
			case POLIZ_ADDRESS: case POLIZ_LABEL:
				el.put_value(pc_el.get_value());
				el.put_type(pc_el.get_type());
				args.push(el);
				break;	
					
			case LEX_ID:
				q = pc_el.get_value();
				if (TID[q].get_assign()) {
					if (TID[q].get_str() != NULL) {
						el.put_str(TID[q].get_str());
						el.put_type(LEX_CHAR);
					}
					else if (TID[q].get_dvalue() != 0 ) {
						el.put_dvalue(TID[q].get_dvalue());
						el.put_type(LEX_DOUBLE);
					}
					else {
						el.put_value(TID[q].get_value());
						el.put_type(LEX_INT);
					}
					args.push(el);
					break;
				} else
					throw "POLIZ: indefinite identifier";
				break;

			case LEX_NOT:
				i = args.pop();
				if (i.get_type() == LEX_INT) {
					i.put_value(!i.get_value());
					args.push(i);
				}
				else 
					throw "Impossible operation";
				break;

			case LEX_OR:
				i = args.pop();
				j = args.pop();
				if (i.get_type() == LEX_INT && j.get_type() == LEX_INT) {
					i.put_value(i.get_value() || j.get_value());
					args.push(i);
				}
				else 
					throw "Impossible operation";
				break;


			case LEX_AND:
				i = args.pop();
				j = args.pop();
				if (i.get_type() == LEX_INT && j.get_type() == LEX_INT) {
					i.put_value(i.get_value() && j.get_value());
					args.push(i);
				}
				else 
					throw "Impossible operation";
				break;

			case POLIZ_GO:
				index = args.pop().get_value() - 1;
				break;

			case POLIZ_FGO:
				i = args.pop();
				if (!args.pop().get_value()) index = i.get_value()-1;
				break;

			case LEX_PUTS:
				i = args.pop();
				switch (i.get_type()) {
					case LEX_CHAR: cout << i.get_str() << endl; break;
					case LEX_DOUBLE: cout << i.get_dvalue() << endl; break;
					case LEX_INT: cout << i.get_value() << endl; break;
				}
				break;

			case LEX_PLUS:
				i = args.pop();
				j = args.pop();
				switch (j.get_type()) {
					case LEX_CHAR: 
						if (i.get_type() == LEX_CHAR) {
							char *st;
							st = new char [strlen(j.get_str()) + strlen(i.get_str())+1];
							strcpy(st,j.get_str());
							strcat(st,i.get_str());
							j.put_str(st);
							args.push(j);
						}
						else 
							throw "Impossible operation";
						break;
					case LEX_DOUBLE:
						if (i.get_type() == LEX_DOUBLE) {
							j.put_dvalue(j.get_dvalue() + i.get_dvalue());
							args.push(j);
						}
						else if (i.get_type() == LEX_INT) {
							j.put_dvalue(j.get_dvalue() + (double) i.get_value());
							args.push(j);
						}
						else 
							throw "Impossible operation";
						break;
					case LEX_INT:
						if (i.get_type() == LEX_DOUBLE) {
							i.put_dvalue((double) j.get_value() + i.get_dvalue());
							args.push(i);
						}
						else if (i.get_type() == LEX_INT) {
							j.put_value(j.get_value() + i.get_value());
							args.push(j);
						}
						else 
							throw "Impossible operation";
						break;
				}
				break;

			case LEX_MUL:
				i = args.pop();
				j = args.pop();
				switch (j.get_type()) {
					case LEX_CHAR:
						if (i.get_type() == LEX_INT) {
							char *st;
							st = new char [i.get_value()*strlen(j.get_str())+1];
							strcpy(st,j.get_str());
							for ( q = 0; q < i.get_value() - 1; q++)
								strcat(st,j.get_str());
							j.put_str(st);
							args.push(j);
						}
						else 
							throw "Impossible operation";
						break;
					case LEX_DOUBLE:
						if (i.get_type() == LEX_DOUBLE) {
							//cout << "DD " << j.get_dvalue() << " " << i.get_dvalue() << endl;
							j.put_dvalue(j.get_dvalue() * i.get_dvalue());
							args.push(j);
						}
						else if (i.get_type() == LEX_INT) {
							//cout << "DI " << j.get_dvalue() << " " << (double)i.get_value() << endl;
							j.put_dvalue(j.get_dvalue() * (double)i.get_value());
							args.push(j);
						}
						else 
							throw "Impossible operation";
						break;
						
					case LEX_INT:
						if (i.get_type() == LEX_DOUBLE) {
							//cout << "ID " << (double) j.get_value() << " " << i.get_dvalue() << endl;
							i.put_dvalue((double)j.get_value() * i.get_dvalue());
							args.push(i);
						}
						else if (i.get_type() == LEX_INT) {
							//cout << "II " << j.get_value() << " " << i.get_value() << endl;
							j.put_value(j.get_value() * i.get_value());
							args.push(j);
						}
						else 
							throw "Impossible operation";
						break;
				}
				break;

			case LEX_MINUS:
				i = args.pop();
				j = args.pop();
				switch (j.get_type()) {
					case LEX_CHAR:  
						throw "Impossible operation";
					case LEX_DOUBLE:
						if (i.get_type() == LEX_DOUBLE) {
							j.put_dvalue(j.get_dvalue() - i.get_dvalue());
							args.push(j);
						}
						else if (i.get_type() == LEX_INT) {
							j.put_dvalue(j.get_dvalue() - (double) i.get_value());
							args.push(j);
						}
						else 
							throw "Impossible operation";
						break;
						
					case LEX_INT:
						if (i.get_type() == LEX_DOUBLE) {
							i.put_dvalue((double)j.get_value() - i.get_dvalue());
							args.push(i);
						}
						else if (i.get_type() == LEX_INT) {
							j.put_value(j.get_value() - i.get_value());
							args.push(j);
						}
						else 
							throw "Impossible operation";
						break;
				}
				break;

			case LEX_DIV:
				i = args.pop();
				j = args.pop();
				switch (j.get_type()) {
					case LEX_CHAR:  
						throw "Impossible operation";
					case LEX_DOUBLE:
						if (i.get_type() == LEX_DOUBLE && i.get_dvalue()) {
							j.put_dvalue(j.get_dvalue() / i.get_dvalue());
							args.push(j);
						}
						else if (i.get_type() == LEX_INT && i.get_value()) {
							j.put_dvalue(j.get_dvalue() / (double) i.get_value());
							args.push(j);
						}
						else if (!i.get_value() || !i.get_dvalue())
							throw "POLIZ:divide by zero";
						else 
							throw "Impossible operation";
						break;
						
					case LEX_INT:
						if (i.get_type() == LEX_DOUBLE  && i.get_dvalue()) {
							i.put_dvalue((double) j.get_value() / i.get_dvalue());
							args.push(i);
						}
						else if (i.get_type() == LEX_INT && i.get_value()) {
							j.put_value(j.get_value() / i.get_value());
							args.push(j);
						}
						else if (!i.get_value() || !i.get_dvalue())
							throw "POLIZ:divide by zero";
						else 
							throw "Impossible operation";
						break;
				}
				break;	

			case LEX_MOD:
				i = args.pop();
				j = args.pop();
				switch (j.get_type()) {
					case LEX_CHAR:  
						throw "Impossible operation";
					case LEX_DOUBLE:
						if (i.get_type() == LEX_DOUBLE && i.get_dvalue()) {
							throw "Impossible operation";
							//j.put_dvalue(j.get_dvalue() % i.get_dvalue());
							//args.push(j);
						}
						else if (i.get_type() == LEX_INT && i.get_value()) {
							throw "Impossible operation";
							//j.put_dvalue(j.get_dvalue() % (double) i.get_value());
							//args.push(j);
						}
						else if (!i.get_value() || !i.get_dvalue())
							throw "POLIZ:divide by zero";
						else 
							throw "Impossible operation";
						break;
						
					case LEX_INT:
						if (i.get_type() == LEX_DOUBLE  && i.get_dvalue()) {
							throw "Impossible operation";
							//i.put_dvalue((double) j.get_value() % i.get_dvalue());
							//args.push(i);
						}
						else if (i.get_type() == LEX_INT && i.get_value()) {
							j.put_value(j.get_value() % i.get_value());
							args.push(j);
						}
						else if (!i.get_value() || !i.get_dvalue())
							throw "POLIZ:divide by zero";
						else 
							throw "Impossible operation";
						break;
				}
				break;
				
			case LEX_EQ:
				i = args.pop();
				j = args.pop();
				switch (j.get_type()) {
					case LEX_CHAR:  
						if (i.get_type() == LEX_CHAR) {
							q = strcmp(j.get_str(), i.get_str());
							if ( q == 0) el.put_value(1);
							else el.put_value(0);
						}
						else 
							throw "Impossible operation";
						break;
					case LEX_DOUBLE:
						if (i.get_type() == LEX_DOUBLE) {
							el.put_value(j.get_dvalue() == i.get_dvalue());
						}
						else if (i.get_type() == LEX_INT) {
							el.put_value(j.get_dvalue() == i.get_value());
						}
						else 
							throw "Impossible operation";
						break;
						
					case LEX_INT:
						if (i.get_type() == LEX_DOUBLE) {
							el.put_value(j.get_value() == i.get_dvalue());
						}
						else if (i.get_type() == LEX_INT) {
							el.put_value(j.get_value() == i.get_value());
						}
						else 
							throw "Impossible operation";
						break;
				}
				el.put_type(LEX_INT);
				args.push(el);
				break;

			case LEX_LSS:
				i = args.pop();
				j = args.pop();
				switch (j.get_type()) {
					case LEX_CHAR:  
						if (i.get_type() == LEX_CHAR) {
							q = strcmp(j.get_str(), i.get_str());
							if ( q < 0) el.put_value(1);
							else el.put_value(0);
						}
						else 
							throw "Impossible operation";
						break;
					case LEX_DOUBLE:
						if (i.get_type() == LEX_DOUBLE) {
							el.put_value(j.get_dvalue() < i.get_dvalue());
						}
						else if (i.get_type() == LEX_INT) {
							el.put_value(j.get_dvalue() < i.get_value());
						}
						else 
							throw "Impossible operation";
						break;
						
					case LEX_INT:
						if (i.get_type() == LEX_DOUBLE) {
							el.put_value(j.get_value() < i.get_dvalue());
						}
						else if (i.get_type() == LEX_INT) {
							el.put_value(j.get_value() < i.get_value());
						}
						else 
							throw "Impossible operation";
						break;
				}
				el.put_type(LEX_INT);
				args.push(el);
				break;

			case LEX_GTR:
				i = args.pop();
				j = args.pop();
				switch (j.get_type()) {
					case LEX_CHAR:  
						if (i.get_type() == LEX_CHAR) {
							q = strcmp(j.get_str(), i.get_str());
							if ( q > 0) el.put_value(1);
							else el.put_value(0);
						}
						else 
							throw "Impossible operation";
						break;
					case LEX_DOUBLE:
						if (i.get_type() == LEX_DOUBLE) {
							el.put_value(j.get_dvalue() > i.get_dvalue());
						}
						else if (i.get_type() == LEX_INT) {
							el.put_value(j.get_dvalue() > i.get_value());
						}
						else 
							throw "Impossible operation";
						break;
						
					case LEX_INT:
						if (i.get_type() == LEX_DOUBLE) {
							el.put_value(j.get_value() > i.get_dvalue());
						}
						else if (i.get_type() == LEX_INT) {
							el.put_value(j.get_value() > i.get_value());
						}
						else 
							throw "Impossible operation";
						break;
				}
				el.put_type(LEX_INT);
				args.push(el);
				break;

			case LEX_LEQ:
				i = args.pop();
				j = args.pop();
				switch (j.get_type()) {
					case LEX_CHAR:  
						if (i.get_type() == LEX_CHAR) {
							q = strcmp(j.get_str(), i.get_str());
							if ( q <= 0) el.put_value(1);
							else el.put_value(0);
						}
						else 
							throw "Impossible operation";
						break;
					case LEX_DOUBLE:
						if (i.get_type() == LEX_DOUBLE) {
							el.put_value(j.get_dvalue() <= i.get_dvalue());
						}
						else if (i.get_type() == LEX_INT) {
							el.put_value(j.get_dvalue() <= i.get_value());
						}
						else 
							throw "Impossible operation";
						break;
						
					case LEX_INT:
						if (i.get_type() == LEX_DOUBLE) {
							el.put_value(j.get_value() <= i.get_dvalue());
						}
						else if (i.get_type() == LEX_INT) {
							el.put_value(j.get_value() <= i.get_value());
						}
						else 
							throw "Impossible operation";
						break;
				}
				el.put_type(LEX_INT);
				args.push(el);
				break;

			case LEX_GEQ:
				i = args.pop();
				j = args.pop();
				switch (j.get_type()) {
					case LEX_CHAR:  
						if (i.get_type() == LEX_CHAR) {
							q = strcmp(j.get_str(), i.get_str());
							if ( q >= 0) el.put_value(1);
							else el.put_value(0);
						}
						else 
							throw "Impossible operation";
						break;
					case LEX_DOUBLE:
						if (i.get_type() == LEX_DOUBLE) {
							el.put_value(j.get_dvalue() >= i.get_dvalue());
						}
						else if (i.get_type() == LEX_INT) {
							el.put_value(j.get_dvalue() >= i.get_value());
						}
						else 
							throw "Impossible operation";
						break;
						
					case LEX_INT:
						if (i.get_type() == LEX_DOUBLE) {
							el.put_value(j.get_value() >= i.get_dvalue());
						}
						else if (i.get_type() == LEX_INT) {
							el.put_value(j.get_value() >= i.get_value());
						}
						else 
							throw "Impossible operation";
						break;
				}
				el.put_type(LEX_INT);
				args.push(el);
				break;

			case LEX_NEQ:
				i = args.pop();
				j = args.pop();
				switch (j.get_type()) {
					case LEX_CHAR:  
						if (i.get_type() == LEX_CHAR) {
							q = strcmp(j.get_str(), i.get_str());
							if ( q != 0) el.put_value(1);
							else el.put_value(0);
						}
						else 
							throw "Impossible operation";
						break;
					case LEX_DOUBLE:
						if (i.get_type() == LEX_DOUBLE) {
							el.put_value(j.get_dvalue() != i.get_dvalue());
						}
						else if (i.get_type() == LEX_INT) {
							el.put_value(j.get_dvalue() != i.get_value());
						}
						else 
							throw "Impossible operation";
						break;
						
					case LEX_INT:
						if (i.get_type() == LEX_DOUBLE) {
							el.put_value(j.get_value() != i.get_dvalue());
						}
						else if (i.get_type() == LEX_INT) {
							el.put_value(j.get_value() != i.get_value());
						}
						else 
							throw "Impossible operation";
						break;
				}
				el.put_type(LEX_INT);
				args.push(el);
				break;

			case LEX_ASSIGN:
				i = args.pop();
				j = args.pop();
				//cout << i.get_value() << " " << i.get_dvalue() << endl;
				TID[j.get_value()].put_str(i.get_str());
				TID[j.get_value()].put_value(i.get_value());
				TID[j.get_value()].put_dvalue(i.get_dvalue());
				TID[j.get_value()].put_assign();
				break;

			default:
				cout << lex_names[pc_el.get_type()] << endl;
				throw "POLIZ: unexpected elem";
		}//end of switch
		++index;
	};//end of while
	cout << "<br /> Finish of executing!!!" << endl;
}

class Interpretator {
	Parser pars;
	Executer E;
public:
	Interpretator(const char*program):pars(program) {}
	void interpretation();
};

void Interpretator::interpretation() {
	pars.analyze();
	E.execute(pars.prog);
}

int main(int argc,char *argv[])
{
	try {
		Interpretator I("hello.txt");
		I.interpretation();
		return 0;
	}
	catch(char c) {
		cout << "unexpected symbol " << c << endl;
		return 1;
	}
	catch(Lex l) {
		cout << "unexpected lexeme ";
		cout << l;
		return 1;
	}
	catch(const char *source) {
		cout << source << endl;
		return 1;
	}
}
