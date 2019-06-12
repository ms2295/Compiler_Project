/*
Name: Manjot Singh
Date: December 5, 2018
Class: CS280
Professor: Gerard Ryan
Assignment: Program 4 Part 1 Parse File
*/

#include "parse.h"
#include "Program_4_Part_1_Parse_Tree.h"

map<string, int> counters;

// WRAPPER FOR PUSHBACK
namespace Parser {
	bool pushed_back = false;
	Token	pushed_token;

	static Token GetNextToken(istream *in, int *line) {
		if (pushed_back) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);//call get next token from lexer
	}

	static void PushBackToken(Token& t) {
		if (pushed_back) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;
	}

}

static int error_count = 0;

void
ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

ParseTree *Prog(istream *in, int *line)
{
	ParseTree *sl = Slist(in, line);

	if (sl == 0)
		ParseError(*line, "No statements in program");

	if (error_count) //error_count > 0
		return 0;

	return sl;
}

// Slist is a Statement followed by a Statement List
ParseTree *Slist(istream *in, int *line) {
	ParseTree *s = Stmt(in, line);
	if (s == 0)
		return 0;

	if (Parser::GetNextToken(in, line) != SC) {
		ParseError(*line, "Missing semicolon");
		return 0;
	}

	return new StmtList(s, Slist(in, line));//left child is statement, right child is recursive call to statement list
}

ParseTree *Stmt(istream *in, int *line) {
	ParseTree *s;

	Token t = Parser::GetNextToken(in, line);//create local token
	switch (t.GetTokenType()) {
	case IF:
		s = IfStmt(in, line);
		break;

	case PRINT:
		s = PrintStmt(in, line);
		break;

	case DONE:
		return 0;

	case ERR:
		ParseError(*line, "Invalid token");
		return 0;

	default:
		// put back the token and then see if it's an Expr
		Parser::PushBackToken(t);
		s = Expr(in, line);
		if (s == 0) {
			ParseError(*line, "Invalid statement");
			return 0;
		}
		break;
	}
	return s;//return statement
}
//see IF token
ParseTree *IfStmt(istream *in, int *line) {
	// ADD HANDLER
	ParseTree *ex = Expr( in, line );

	Token t = Parser::GetNextToken( in, line );

	if ( ex )
	{
		if ( t == THEN )
		{
			ParseTree *stmt = Stmt( in, line );
			if (stmt)
			{
				return new IfStatement(t.GetLinenum(), ex, stmt);
			}
		}
	}
	ParseError(*line, "Invalid IF statement");
	return 0;
}

ParseTree *PrintStmt(istream *in, int *line) {

	ParseTree *ex = Expr(in, line);
	int l = *line;

	if (ex == 0){
		ParseError(*line, "Invalid Expression");
		return 0;
	}

	return new PrintStatement(l, ex);
}

ParseTree *Expr(istream *in, int *line) {
	ParseTree *t1 = LogicExpr(in, line);
	if (t1 == 0) {
		return 0;
	}

	Token t = Parser::GetNextToken(in, line);

	if (t != ASSIGN) {
		Parser::PushBackToken(t);
		return t1;
	}

	ParseTree *t2 = Expr(in, line); // right assoc
	if (t2 == 0) {
		ParseError(*line, "Missing expression after operator (Expr)");
		return 0;
	}

	return new Assignment(t.GetLinenum(), t1, t2);
}

ParseTree *LogicExpr(istream *in, int *line) {
	ParseTree *t1 = CompareExpr(in, line);
	if (t1 == 0) {
		return 0;
	}

	// HANDLE OP
    while (true) {
		Token t = Parser::GetNextToken(in, line);

		if (t != LOGICAND && t != LOGICOR) {
			Parser::PushBackToken(t);
			return t1;
		}

		ParseTree *t2 = CompareExpr(in, line);
		if (t2 == 0) {
			ParseError(*line, "Missing expression after operator (Logic)");
			return 0;
		}

		if (t == LOGICAND)
			t1 = new LogicAndExpr(t.GetLinenum(), t1, t2);
		else
			t1 = new LogicOrExpr(t.GetLinenum(), t1, t2);
	}
}

ParseTree *CompareExpr(istream *in, int *line) {
	ParseTree *t1 = AddExpr(in, line);
	if (t1 == 0) {
		return 0;
	}

	// HANDLE OP
    while (true) {
		Token t = Parser::GetNextToken(in, line);

		if (t != EQ && t != NEQ && t != GT && t != GEQ && t != LT && t != LEQ) {
			Parser::PushBackToken(t);
			return t1;
		}

		ParseTree *t2 = AddExpr(in, line);
		if (t2 == 0) {
			ParseError(*line, "Missing expression after operator (Compare)");
			return 0;
		}

		if (t == EQ)
			t1 = new EqExpr(t.GetLinenum(), t1, t2);
        else if (t == NEQ)
            t1 = new NEqExpr(t.GetLinenum(), t1, t2);
        else if (t == GT)
            t1 = new GtExpr(t.GetLinenum(), t1, t2);
        else if (t == GEQ)
            t1 = new GEqExpr(t.GetLinenum(), t1, t2);
        else if (t == LT)
            t1 = new LtExpr(t.GetLinenum(), t1, t2);
		else
			t1 = new LEqExpr(t.GetLinenum(), t1, t2);
	}
}

ParseTree *AddExpr(istream *in, int *line) {
	ParseTree *t1 = MulExpr(in, line);
	if (t1 == 0) {
		return 0;
	}

	while (true) {
		Token t = Parser::GetNextToken(in, line);

		if (t != PLUS && t != MINUS) {
			Parser::PushBackToken(t);
			return t1;
		}

		ParseTree *t2 = MulExpr(in, line);
		if (t2 == 0) {
			ParseError(*line, "Missing expression after operator (Add)");
			return 0;
		}

		if (t == PLUS)
			t1 = new PlusExpr(t.GetLinenum(), t1, t2);
		else
			t1 = new MinusExpr(t.GetLinenum(), t1, t2);
	}
}

ParseTree *MulExpr(istream *in, int *line) {
	ParseTree *t1 = Factor(in, line);
	if (t1 == 0) {
		return 0;
	}

	// HANDLE Op
    while (true) {
		Token t = Parser::GetNextToken(in, line);

		if (t != STAR && t != SLASH) {
			Parser::PushBackToken(t);
			return t1;
		}

		ParseTree *t2 = Factor(in, line);
		if (t2 == 0) {
			ParseError(*line, "Missing expression after operator (Mul)");
			return 0;
		}

		if (t == STAR)
			t1 = new TimesExpr(t.GetLinenum(), t1, t2);
		else
			t1 = new DivideExpr(t.GetLinenum(), t1, t2);
	}
}

ParseTree *Factor(istream *in, int *line) {
	bool neg = false;
	Token t = Parser::GetNextToken(in, line);

	if (t == MINUS) {
		neg = true;
	}
	else {
		Parser::PushBackToken(t);
	}

	ParseTree *p1 = Primary(in, line);
	if (p1 == 0) {
		ParseError(*line, "Missing primary");
		return 0;
	}

	if (neg) {
		// handle as -1 * Primary
		return new TimesExpr(t.GetLinenum(), new IConst(t.GetLinenum(), -1), p1);
	}
	else
		return p1;
}

ParseTree *Primary(istream *in, int *line) {
	bool tf;
	Token t1 = Parser::GetNextToken(in, line);

	// PROCESS TOKEN, IDENTIFY PRIMARY, RETURN SOMETHING
	if (t1 == IDENT)
	{
		counters[t1.GetLexeme()]++;
		return new Ident(t1);
	}

	if (t1 == ICONST)
	{
		return new IConst(t1);
	}

	if (t1 == SCONST)
	{
		return new SConst(t1);
	}

	if (t1 == TRUE)
	{
		tf = true;
		return new BoolConst(t1, tf);
	}

	if (t1 == FALSE)
	{
		tf = false;
		return new BoolConst(t1, tf);
	}

	if (t1 == LPAREN)
	{
		ParseTree *expr = Expr(in, line);
		Token t2 = Parser::GetNextToken(in, line);

		if (t2 == RPAREN)
		{
			return expr;
		}
	}

	ParseError(t1.GetLinenum(), "Invalid primary");

	return 0;
}