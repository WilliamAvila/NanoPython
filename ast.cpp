#include <sstream>
#include "ast.h"

map<string,int> mapper;
int current_temp_offset = 4;
int current_label = 0;

string NewTemp(){
	string new_temp = "dword[ebp - " + to_string(current_temp_offset) + "]";
	current_temp_offset += 4;
	return new_temp;
}


string NewLabel(){
	string label = "label" + to_string(current_label);
	current_label++;
	return label;
}

void ResetOffset(){
	current_temp_offset = 4;
}

string ResolveId(string id){
	if(mapper.find(id) != mapper.end()){
	  int offset = mapper[id];
	  string dir = "dword[ebp - " + to_string(offset) + "]";
	  return dir;
	}

	mapper[id] = current_temp_offset;
	string dir = "dword[ebp - " + to_string(current_temp_offset) + "]";
	current_temp_offset += 4;
	return dir;
}

void AddExpression::GenerateCode(string& code, string &place ){

	string code1 , place1;
	string code2 , place2;

	place = NewTemp();

	e1->GenerateCode(code1, place1);
	e2->GenerateCode(code2, place2);

	code = code1 + code2 +
		"mov eax, " + place1 + "\n" +
		"add eax, " + place2 + "\n" +
		"mov " +place + ", eax \n";
}

void SubExpression::GenerateCode(string& code, string &place ){
	string code1 , place1;
	string code2 , place2;

	place = NewTemp();

	e1->GenerateCode(code1, place1);
	e2->GenerateCode(code2, place2);

	code = code1 + code2 +
		"mov eax, " + place1 + "\n" +
		"sub eax, " + place2 + "\n" +
		"mov " +place + ", eax \n";
}

void MultExpression::GenerateCode(string& code, string &place ){

	string code1 , place1;
	string code2 , place2;

	place = NewTemp();

	e1->GenerateCode(code1, place1);
	e2->GenerateCode(code2, place2);

	code = code1 + code2 +
		"mov eax, " + place1 + "\n" +
		"imul eax, " + place2 + "\n" +
		"mov " +place + ", eax \n";
}

void DivExpression::GenerateCode(string& code, string &place ){

	string code1 , place1;
	string code2 , place2;

	place = NewTemp();

	e1->GenerateCode(code1, place1);
	e2->GenerateCode(code2, place2);

	code = code1 + code2 +
		"mov eax, " + place1 + "\n" +
		"xor edx, edx\n" +
		"idiv " + place2 + "\n" +
		"mov " +place + ", eax \n";
}

void GreaterExpression::GenerateCode(string& code, string &place){
	string code1 , place1;
	string code2 , place2;

	place = NewTemp();

	e1->GenerateCode(code1, place1);
	e2->GenerateCode(code2, place2);

	code = code1 + code2 +
		"mov eax, " + place1 + "\n" +
		"cmp eax, " + place2 + "\n" +
		"setg al \n" +
		"movzx " +place + ", al \n";
}

void LesserExpression::GenerateCode(string& code, string &place){
	string code1 , place1;
	string code2 , place2;

	place = NewTemp();

	e1->GenerateCode(code1, place1);
	e2->GenerateCode(code2, place2);

	code = code1 + code2 +
		"mov eax, " + place1 + "\n" +
		"cmp eax, " + place2 + "\n" +
		"setl al \n" +
		"movzx " +place + ", al \n";
}

void GreaterOrEqualExpression::GenerateCode(string& code, string &place){
	string code1 , place1;
	string code2 , place2;

	place = NewTemp();

	e1->GenerateCode(code1, place1);
	e2->GenerateCode(code2, place2);

	code = code1 + code2 +
		"mov eax, " + place1 + "\n" +
		"cmp eax, " + place2 + "\n" +
		"setge al \n" +
		"movzx " +place + ", al \n";
}

void LesserOrEqualExpression::GenerateCode(string& code, string &place){
	string code1 , place1;
	string code2 , place2;

	place = NewTemp();

	e1->GenerateCode(code1, place1);
	e2->GenerateCode(code2, place2);

	code = code1 + code2 +
		"mov eax, " + place1 + "\n" +
		"cmp eax, " + place2 + "\n" +
		"setle al \n" +
		"movzx " +place + ", al \n";
}

void EqualityExpression::GenerateCode(string& code, string &place){
	string code1 , place1;
	string code2 , place2;

	place = NewTemp();

	e1->GenerateCode(code1, place1);
	e2->GenerateCode(code2, place2);

	code = code1 + code2 +
		"mov eax, " + place1 + "\n" +
		"cmp eax, " + place2 + "\n" +
		"sete al \n" +
		"movzx " +place + ", al \n";
}

void InequalityExpression::GenerateCode(string& code, string &place){
	string code1 , place1;
	string code2 , place2;

	place = NewTemp();

	e1->GenerateCode(code1, place1);
	e2->GenerateCode(code2, place2);

	code = code1 + code2 +
		"mov eax, " + place1 + "\n" +
		"cmp eax, " + place2 + "\n" +
		"setne al \n" +
		"movzx " +place + ", al \n";
}

void NumExpression::GenerateCode(string& code, string &place){
	code = "";
	place = to_string(value);
}

void IdExpression::GenerateCode(string& code, string &place){
	code = "";
	place = ResolveId(Id);
}

string AssignStatement::GenerateCode(){
    string code, place;
      expr->GenerateCode(code,place);
      stringstream ss;
      string address = ResolveId(this->id);
      ss << code << endl
      << "mov eax, " << place << endl
      << "mov " << address << ", eax" << endl;
      return ss.str();
}

string PrintStatement::GenerateCode(){
      string code, place;
      expr->GenerateCode(code,place);
      stringstream ss;
      ss << code << endl
      << "push " << place << endl
      << "push paddr(0x10000000)" << endl
      << "call @libc.printf" << endl
      << "add esp, 8" << endl;
      return ss.str();
}


string BlockStatement::GenerateCode(){

	return "";
}

string IfStatement::GenerateCode(){
	string code, place;
	string label_else = NewLabel();
	string end_if = NewLabel();

	cond->GenerateCode(code,place);
	string blockCode = this->trueBlock->GenerateCode();
	string elseBlock = this->falseBlock->GenerateCode();
	stringstream ss;

	ss << code << endl
	<< "cmp " << place << ", 0"<< endl
	<< "je " << label_else << endl
	<< blockCode << endl
	<< "jmp " << end_if << endl
	<< label_else << ":" << endl
  << falseBlock << endl
	<< end_if << ":";

	return ss.str();
}

string WhileStatement::GenerateCode(){
	string code, place;
	string label_while = NewLabel();
	string end_while = NewLabel();

	cond->GenerateCode(code,place);
	string blockCode = this->block->GenerateCode();
	stringstream ss;
	ss << code << endl
	<< label_while << ":" << endl
	<< "cmp " << place << ", 0"<< endl
	<< "je " << end_while << endl
	<< blockCode << endl
	<< "jmp " << label_while << endl
	<< end_while << ":";

	return ss.str();

}


string ForStatement::GenerateCode(){
	string code, place, rangeCode, rangePlace;
	string label_for = NewLabel();
	string end_for = NewLabel();

	init->GenerateCode(code,place);
	range->GenerateCode(rangeCode, rangePlace);

	string blockCode = this->block->GenerateCode();

	stringstream ss;
	ss << code << endl
	<< rangeCode << endl
	<< label_for << ":" << endl
	<< "cmp " << place << ", " << rangePlace << endl
	<< "jg " << end_for << endl
	<< blockCode << endl
	<< "jmp " << label_for << endl
	<< end_for << ":";

	return ss.str();

}
