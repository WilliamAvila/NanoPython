#include <iostream>
#include <string>
#include <map>
#include <list>
class Statement;
using namespace std;
typedef list<Statement *> StatementList;

class Expression{

	public: 
	  
	  virtual void GenerateCode(string& code, string &place ) = 0;
};

class AddExpression : public Expression{
	public:
		Expression *e1, *e2;
		AddExpression(Expression *e1, Expression *e2){
			this->e1 = e1;
			this->e2 = e2;
		} 

		void GenerateCode(string& code, string &place );
};

class SubExpression : public Expression{
	public:
		Expression *e1, *e2;
		SubExpression(Expression *e1, Expression *e2){
			this->e1 = e1;
			this->e2 = e2;
		} 
		void GenerateCode(string& code, string &place );
};

class MultExpression : public Expression{
	public:
		Expression *e1, *e2;
		MultExpression(Expression *e1, Expression *e2){
			this->e1 = e1;
			this->e2 = e2;
		} 
		void GenerateCode(string& code, string &place );
};

class DivExpression : public Expression{
	public:
		Expression *e1, *e2;
		DivExpression(Expression *e1, Expression *e2){
			this->e1 = e1;
			this->e2 = e2;
		} 
		void GenerateCode(string& code, string &place );
};

class GreaterExpression : public Expression{
	public:
		Expression *e1, *e2;
		GreaterExpression(Expression *e1, Expression *e2){
			this->e1 = e1;
			this->e2 = e2;
		} 

		void GenerateCode(string& code, string &place);
};

class LesserExpression : public Expression{
	public:
		Expression *e1, *e2;
		LesserExpression(Expression *e1, Expression *e2){
			this->e1 = e1;
			this->e2 = e2;
		} 

		void GenerateCode(string& code, string &place);
};

class GreaterOrEqualExpression : public Expression{
	public:
		Expression *e1, *e2;
		GreaterOrEqualExpression(Expression *e1, Expression *e2){
			this->e1 = e1;
			this->e2 = e2;
		} 

		void GenerateCode(string& code, string &place);
};

class LesserOrEqualExpression : public Expression{
	public:
		Expression *e1, *e2;
		LesserOrEqualExpression(Expression *e1, Expression *e2){
			this->e1 = e1;
			this->e2 = e2;
		} 

		void GenerateCode(string& code, string &place);
};

class EqualityExpression : public Expression{
	public:
		Expression *e1, *e2;
		EqualityExpression(Expression *e1, Expression *e2){
			this->e1 = e1;
			this->e2 = e2;
		} 

		void GenerateCode(string& code, string &place);
};

class InequalityExpression : public Expression{
	public:
		Expression *e1, *e2;
		InequalityExpression(Expression *e1, Expression *e2){
			this->e1 = e1;
			this->e2 = e2;
		} 

		void GenerateCode(string& code, string &place);
};

class NumExpression : public Expression{
	public:
		int value;

		NumExpression(int value){
			this->value = value;
		}


		void GenerateCode(string& code, string &place);
};

class IdExpression : public Expression{
	public:
		string Id;

		IdExpression(string Id){
			this->Id = Id;
		}

		void GenerateCode(string& code, string &place);
};


enum StatementKind {
    BLOCK_STATEMENT,
    PRINT_STATEMENT,
    ASSIGN_STATEMENT,
    IF_STATEMENT,
    WHILE_STATEMENT,
    FOR_STATEMENT
};

class Statement {
public:
    virtual string GenerateCode() = 0;
    virtual StatementKind getKind() = 0;
};

class BlockStatement: public Statement {
public:
    BlockStatement(StatementList *stList) { this->stList = stList; }
    string GenerateCode();
    StatementKind getKind() { return BLOCK_STATEMENT; }

    StatementList * stList;
};

class PrintStatement: public Statement {
public:
    PrintStatement(Expression *expr) { this->expr = expr; }
    string GenerateCode();
    StatementKind getKind() { return PRINT_STATEMENT; }

    Expression *expr;
};

class AssignStatement: public Statement {
public:
    AssignStatement(string id, Expression *expr) {
        this->id = id;
        this->expr = expr;
    }
    string GenerateCode();
    StatementKind getKind() { return ASSIGN_STATEMENT; }

    string id;
    Expression *expr;
};

class IfStatement: public Statement {
public:
    IfStatement(Expression *cond, Statement *trueBlock, Statement *falseBlock) {
        this->cond = cond;
        this->trueBlock = trueBlock;
        this->falseBlock = falseBlock;
    }
    string GenerateCode();
    StatementKind getKind() { return IF_STATEMENT; }

    Expression *cond;
    Statement *trueBlock;
    Statement *falseBlock;
};

class WhileStatement: public Statement {
public:
    WhileStatement(Expression *cond, Statement *block){
        this->cond = cond;
        this->block = block;
    }
    string GenerateCode();
    StatementKind getKind() { return WHILE_STATEMENT; }

    Expression *cond;
    Statement *block;
};

class ForStatement: public Statement{
public:
      ForStatement( Expression *init, Expression *range, Statement *block){
          this->init = init;
          this->range = range;
          this->block = block;
      }

      string GenerateCode();
      StatementKind getKind() { return FOR_STATEMENT; }


      Expression *init;
      Expression *range;
      Statement *block;
};
