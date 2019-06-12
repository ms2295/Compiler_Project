/*
Name: Manjot Singh
Date: December 5, 2018
Class: CS280
Professor: Gerard Ryan
Assignment: Program 4 Part 1 Parse Tree Header File
*/

#ifndef PARSETREE_H_
#define PARSETREE_H_

#include <vector>
#include <map>
using std::vector;
using std::map;

// NodeType represents all possible types
enum NodeType { ERRTYPE, INTTYPE, STRTYPE, BOOLTYPE };
extern void RunTimeError ( string );

// object holds boolean, integer, or string, and remembers which it holds
class Value {
    bool bval;
    int ival;
    string sval;
    enum VT { isBool, isInt, isString, isTypeError } type;

public:
    Value() : bval(false), ival(0), type(isTypeError) {}
    Value(bool b) : bval(b), ival(0), type(isBool) {}
    Value(int i) : bval(false), ival(i), type(isInt) {}
    Value(string s) : bval(false), ival(0), sval(s), type(isString) {}
    
    // in the case of an error, I use the value to hold the error message
    Value(string sval, bool isError) : bval(false), ival(0), sval(sval), type(isTypeError) {}

    bool isBoolType() const { return type == VT::isBool; }
    bool isIntType() const { return type == VT::isInt; }
    bool isStringType() const { return type == VT::isString; }
    bool isError() const { return type == VT::isTypeError; }
    bool hasMessage() const { return isError() && sval.size() > 0; }

    bool isTrue() const { return isBoolType() && bval; }
    bool getBoolean() const {
        if( !isBoolType() )
        {
            //throw "Not boolean valued";
            return 0;
        }
        return bval;
    }

    int getInteger() const {
        if( !isIntType() )
        {
            //throw "Not integer valued";
            return 0;
        }
        return ival;
    }

    string getString() const {
        if( !isStringType() )
        {
            //throw "Not string valued";
            return 0;
        }
        return sval;
    }

    string getMessage() const {
        if( !hasMessage() )
        {
            //throw "No message";
            return 0;
        }
        return sval;
    }

    friend ostream& operator<<(ostream& out, const Value& v) {
        if( v.type == VT::isBool ) out << (v.bval ? "True" : "False");
        else if( v.type == VT::isInt ) out << v.ival;
        else if( v.type == VT::isString ) out << v.sval;
        else if( v.sval.size() > 0 ) out << "RUNTIME ERROR " << v.sval;
        //else out << "TYPE ERROR";
        return out;
    }

    Value operator+(const Value& v)
    {
        if (type == isInt && v.type == isInt)
        {
            return Value(ival + v.ival);
        }

        if (type == isString && v.type == isString)
        {
            return Value(sval + v.sval);
        }

        RunTimeError("Cannot add these two values");
        return Value();
    }

    Value operator-(const Value& v)
    {
        if (type == isInt && v.type == isInt)
        {
            return Value(ival - v.ival);
        }

        RunTimeError("Cannot subtract these two values");
        return Value();
    }
    
    Value operator*(const Value& v)
    {
        if (type == isInt && v.type == isInt)
        {
            return Value(ival * v.ival);
        }

        if (type == isInt && v.type == isString)
        {
            if(getInteger() >= 0)
            {
                string s;
                for(int i = 0; i < ival; ++i)
                {
                    s = s + v.sval;
                }
                return s;
            }

            RunTimeError("Cannot multiply a string by a negative integer");
        }

        if (type == isString && v.type == isInt)
        {
            if(v.getInteger() >= 0)
            {
                string s;
                for(int i = 0; i < v.ival; ++i)
                {
                    s = s + sval;
                }
                return s;
            }

            RunTimeError("Cannot multiply a string by a negative integer");
        }
        
        if (type == isInt && v.type == isBool)
        {
            if (ival == -1)
            {
                bool ans = !v.bval;
                return (ans);
            }
            
            RunTimeError("Invalid logical not statement");
        }
        
        RunTimeError("Cannot multiply these two values");
        return Value();
    }

    Value operator/(const Value& v)
    {
        if (type == isInt && v.type == isInt)
        {
            if (v.ival != 0)
            {
                return Value(ival / v.ival);
            }
            
            RunTimeError("Cannot divide by 0");
        }

        RunTimeError("Cannot divide these two values");
        return Value();
    }

    Value operator<(const Value& v)
    {
        if (type == isInt && v.type == isInt)
        {
            return Value(ival < v.ival);
        }

        if (type == isString && v.type == isString)
        {
            return Value(sval < v.sval);
        }

        RunTimeError("Type mismatch in <");
        return Value();
    }

    Value operator<=(const Value& v)
    {
        if (type == isInt && v.type == isInt)
        {
            return Value(ival <= v.ival);
        }

        if (type == isString && v.type == isString)
        {
            return Value(sval <= v.sval);
        }

        RunTimeError("Type mismatch in <=");
        return Value();
    }

    Value operator>(const Value& v)
    {
        if (type == isInt && v.type == isInt)
        {
            return Value(ival > v.ival);
        }

        if (type == isString && v.type == isString)
        {
            return Value(sval > v.sval);
        }

        RunTimeError("Type mismatch in >");
        return Value();
    }

    Value operator>=(const Value& v)
    {
        if (type == isInt && v.type == isInt)
        {
            return Value(ival >= v.ival);
        }

        if (type == isString && v.type == isString)
        {
            return Value(sval >= v.sval);
        }

        RunTimeError("Type mismatch in >=");
        return Value();
    }

    Value operator==(const Value& v)
    {
        if (type == isInt && v.type == isInt)
        {
            return Value(ival == v.ival);
        }

        if (type == isString && v.type == isString)
        {
            return Value(sval == v.sval);
        }

        if (type == isBool && v.type == isBool)
        {
            return Value(bval == v.bval);
        }

        RunTimeError("Type mismatch in ==");
        return Value();
    }

    Value operator!=(const Value& v) 
    {
        if (type == isInt && v.type == isInt)
        {
            return Value(ival != v.ival);
        }

        if (type == isString && v.type == isString)
        {
            return Value(sval != v.sval);
        }

        if (type == isBool && v.type == isBool)
        {
            return Value(bval != v.bval);
        }

        RunTimeError("Type mismatch in !=");
        return Value();
    }
};

extern map<string, Value> vars;

class ParseTree { //tree nature lives in the class
public:
    int         linenum;
    ParseTree   *left; //pointers of type ParseTree
    ParseTree   *right;

public:
    ParseTree(int linenum, ParseTree *l = 0, ParseTree *r = 0)//constructor
        : linenum(linenum), left(l), right(r) {}

    virtual ~ParseTree() {//THE DESTRUCTOR!!!!!!
        delete left;
        delete right;
    }

    int GetLinenum() const { return linenum; }

    virtual NodeType GetType() const { return ERRTYPE; } //ask any class which is a parsee tree. what its type is. nothing provided. then error
    virtual bool IAmAStringConst() const { return false; }
    virtual bool IAmAnIdent() const { return false; }
    virtual bool IAmABool() const { return false; }
    virtual string getID() const {return ""; }
    virtual bool getBool() const {return false; }
    virtual Value Eval(map<string, Value> &vars) = 0;
};

class StmtList : public ParseTree { // class statement list inherits from class ParseTree. therefore a stmList object is also a Parsetree object

public:
    StmtList(ParseTree *l, ParseTree *r) : ParseTree(0, l, r) {}
    virtual Value Eval(map<string, Value> &vars)
    {
        left->Eval(vars);
        if (right)
        {
            right->Eval(vars);
        }

        return Value();
    }
};

class IfStatement : public ParseTree {
public:
    IfStatement(int line, ParseTree *ex, ParseTree *stmt) : ParseTree(line, ex, stmt) {}
    virtual Value Eval(map<string, Value> &vars)
    {
        Value ans = left->Eval(vars);
        if (ans.isBoolType())
        {
            if (ans.getBoolean())
            {
                return right->Eval(vars); 
            }
        }
        else
        {
            RunTimeError("if expression is not boolean typed");
        }
        return Value();
    }
};

class Assignment : public ParseTree {
public:
    Assignment(int line, ParseTree *lhs, ParseTree *rhs) : ParseTree(line, lhs, rhs) {}
    virtual Value Eval(map<string, Value> &vars)
    {
        if (left->IAmAnIdent())
        {
            if(not vars.empty() and vars.count(left->getID()))
            {
                Value rVal = right->Eval(vars);
                string lVal = left->getID();
                vars[lVal] = rVal;
            }
            
            else
            {
                Value rVal = right->Eval(vars);
                string lVal = left->getID();
                vars.insert (std::pair<string, Value>(lVal, rVal));
            }
        } 
        else
        {
            RunTimeError("Left hand side must be an identifier");
        }
        return Value();
    }
};

class PrintStatement : public ParseTree {
public:
    PrintStatement(int line, ParseTree *e) : ParseTree(line, e) {}
    virtual Value Eval(map<string, Value> &vars)
    {
        cout << left->Eval(vars) << '\n';
        return Value();
    }
};

class PlusExpr : public ParseTree {//TYPE DETERMINED BY OPERANDS. SO INTEGER ADDITION RETURNS AN INT. 
public:
    PlusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &vars)
    {
        return left->Eval(vars) + right->Eval(vars);
    }
};

class MinusExpr : public ParseTree {
public:
    MinusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &vars)
    {
        return left->Eval(vars) - right->Eval(vars);
    }
};

class TimesExpr : public ParseTree {
public:
    TimesExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &vars)
    {
        return left->Eval(vars) * right->Eval(vars);
    }
};

class DivideExpr : public ParseTree {
public:
    DivideExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &vars)
    {
        return left->Eval(vars) / right->Eval(vars);
    }
};

class LogicAndExpr : public ParseTree {//BOOLEAN TYPE RETURN
public:
    LogicAndExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &vars)
    {
        Value lVal = left->Eval(vars);
        Value rVal = right->Eval(vars);
        if (lVal.isBoolType() && rVal.isBoolType())
        {
            return lVal.isTrue() && rVal.isTrue();
        }
        else
        {
            RunTimeError("first operand of && is not boolean typed");
        }
        return Value();
    }
};

class LogicOrExpr : public ParseTree {
public:
    LogicOrExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &vars)
    {
        Value lVal = left->Eval(vars);
        Value rVal = right->Eval(vars);
        if (lVal.isBoolType() || rVal.isBoolType())
        {
            return lVal.isTrue() || rVal.isTrue();
        }
        else
        {
            RunTimeError("first operand of || is not boolean typed");
        }
        return Value();
    }
};

class EqExpr : public ParseTree {
public:
    EqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &vars)
    {
        return left->Eval(vars) == right->Eval(vars);
    }
};

class NEqExpr : public ParseTree {
public:
    NEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &vars)
    {
        return left->Eval(vars) != right->Eval(vars);
    }
};

class LtExpr : public ParseTree {
public:
    LtExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &vars)
    {
        return left->Eval(vars) < right->Eval(vars);
    }
};

class LEqExpr : public ParseTree {
public:
    LEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &vars)
    {
        return left->Eval(vars) <= right->Eval(vars);
    }
};

class GtExpr : public ParseTree {
public:
    GtExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &vars)
    {
        return left->Eval(vars) > right->Eval(vars);
    }
};

class GEqExpr : public ParseTree {
public:
    GEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    virtual Value Eval(map<string, Value> &vars)
    {
        return left->Eval(vars) >= right->Eval(vars);
    }
};

class IConst : public ParseTree {
    int val;

public:
    IConst(int l, int i) : ParseTree(l), val(i) {}//nothing passed to the parsetree constructor cuz integer constant is a leaf
    IConst(Token& t) : ParseTree(t.GetLinenum()) {
        val = stoi(t.GetLexeme());
    }

    NodeType GetType() const { return INTTYPE; }
    virtual Value Eval(map<string, Value> &vars)
    {
        return Value(val);
    }
};

class BoolConst : public ParseTree {
    bool val;

public:
    BoolConst(Token& t, bool val) : ParseTree(t.GetLinenum()), val(val) {}

    NodeType GetType() const { return BOOLTYPE; }
    
    bool IAmABool() const { return true; }
    bool getBool() const {return val; }
    virtual Value Eval(map<string, Value> &vars)
    {
        return Value(val);
    }
};

class SConst : public ParseTree {
    string val;


public:
    SConst(Token& t) : ParseTree(t.GetLinenum()) {
        val = t.GetLexeme();
    }

    NodeType GetType() const { return STRTYPE; }
    
    bool IAmAStringConst() const { return true; }
    virtual Value Eval(map<string, Value> &vars)
    {
        return Value(val);
    }
};

class Ident : public ParseTree {
    string id;

public:
    Ident(Token& t) : ParseTree(t.GetLinenum()), id(t.GetLexeme()) {}
    bool IAmAnIdent() const { return true; }
    string getID() const {return id; }
        
    virtual Value Eval(map<string, Value> &vars)
    {
        if (not vars.empty() and vars.count(id))
        {
            return vars[id];
        }
        else
        {
            RunTimeError("Variable is used before being set");
        }
        return Value();
    }
};

#endif /* PARSETREE_H_ */
