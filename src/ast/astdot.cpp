#include <fstream>
#include <string>
#include <cstdlib>
#include <iostream>

#include <ast/ast.hpp>

int idx = 0;

void AstTree::dot() {
    std::string output = "digraph AST {\n";
    output += "tree[shape=box, label=\"" + getFile() + "\"]\n";
    
    for (auto s : getStructs()) {
        output += s->dot("tree");
    }
    
    for (auto global : getGlobalStatements()) {
        output += global->dot("tree");
    }
    
    output += "}\n";
    
    // Write the file
    std::cout << output << std::endl;
    
    std::ofstream writer("ast.dot");
    writer << output << std::endl;
    writer.close();
    
    system("dot -Tpng ast.dot > ast.png");
}

//
// Structures
//
std::string AstStruct::dot(std::string parent) {
    std::string name = "struct" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[shape=rect, label=\"struct " + getName() + "\"];\n";
    output += parent + " -> " + name + ";\n";
    
    for (auto item : items) {
        std::string item_name = "item" + std::to_string(idx);
        ++idx;
        
        output += item_name + "[label=\"" + item.name + "\"];\n";
        output += name + " -> " + item_name + ";\n";
        
        output += defaultExpressions[item.name]->dot(item_name);
    }
    
    return output;
}

//
// Global statements (functions)
//
std::string AstExternFunction::dot(std::string parent) {
    return parent + " -> " + getName() + "[shape=rect];\n";
}

std::string AstFunction::dot(std::string parent) {
    std::string output = getName() + "[shape=box];\n";
    output += parent + " -> " + getName() + ";\n";
    output += getBlock()->dot(getName());
    
    return output;
}

//
// Blocks
//
std::string AstBlock::dot(std::string parent) {
    std::string name = "block" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[shape=box, label=\"Block\"];\n";
    output += parent + " -> " + name + ";\n";

    for (auto stmt : block) {
        output += stmt->dot(name);
    }
    
    return output;
}

//
// Statements
//
std::string AstExprStatement::dot(std::string parent) {
    std::string name = "expr" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"expression\"];\n";
    output += parent + " -> " + name + ";\n";
    output += getExpression()->dot(name);
    
    return output;
}

std::string AstFuncCallStmt::dot(std::string parent) {
    std::string name = "fc" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"" + getName() + "\"];\n";
    output += parent + " -> " + name + ";\n";
    
    output += getExpression()->dot(name);
    
    return output;
}

std::string AstReturnStmt::dot(std::string parent) {
    std::string name = "return" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"return\"];\n";
    output += parent + " -> " + name + ";\n";
    
    if (hasExpression()) output += getExpression()->dot(name);
    
    return output;
}

std::string AstVarDec::dot(std::string parent) {
    std::string name = "var" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"var " + this->name + "\"];\n";
    output += parent + " -> " + name + ";\n";
    
    return output;
}

std::string AstStructDec::dot(std::string parent) {
    std::string name = "struct" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"struct " + this->varName + " : " + this->structName + "\"];\n";
    output += parent + " -> " + name + ";\n";
    
    return output;
}

std::string AstIfStmt::dot(std::string parent) {
    std::string name = "cond" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"if\"];\n";
    output += parent + " -> " + name + ";\n";
    
    output += getExpression()->dot(name);
    output += block->dot(name);
    for (auto br : branches) output += br->dot(name);
    
    return output;
}

std::string AstElifStmt::dot(std::string parent) {
    std::string name = "cond" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"elif\"];\n";
    output += parent + " -> " + name + ";\n";
    
    output += getExpression()->dot(name);
    output += block->dot(name);
    
    return output;
}

std::string AstElseStmt::dot(std::string parent) {
    std::string name = "cond" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"else\"];\n";
    output += parent + " -> " + name + ";\n";
    output += block->dot(name);
    
    return output;
}

std::string AstWhileStmt::dot(std::string parent) {
    std::string name = "while" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"while\"];\n";
    output += parent + " -> " + name + ";\n";
    
    output += getExpression()->dot(name);
    output += block->dot(name);
    
    return output;
}

std::string AstBreak::dot(std::string parent) {
    std::string name = "break" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"break\"];\n";
    output += parent + " -> " + name + ";\n";
    
    return output;
}

std::string AstContinue::dot(std::string parent) {
    std::string name = "continue" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"continue\"];\n";
    output += parent + " -> " + name + ";\n";
    
    return output;
}

//
// Expressions
//
std::string AstExprList::dot(std::string parent) {
    std::string name = "list" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"list\"];\n";
    output += parent + " -> " + name + ";\n";
    
    for (auto expr : list) output += expr->dot(name);
    
    return output;
}

std::string AstNegOp::dot(std::string parent) {
    std::string name = "neg" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"-\"];\n";
    output += parent + " -> " + name + ";\n";
    output += getVal()->dot(name);
    return output;
}

std::string AstAssignOp::dot(std::string parent) {
    std::string name = "assign" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\":=\"];\n";
    output += parent + " -> " + name + ";\n";
    output += getLVal()->dot(name);
    output += getRVal()->dot(name);
    return output;
}

std::string AstAddOp::dot(std::string parent) {
    std::string name = "add" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"+\"];\n";
    output += parent + " -> " + name + ";\n";
    output += getLVal()->dot(name);
    output += getRVal()->dot(name);
    return output;
}

std::string AstSubOp::dot(std::string parent) {
    std::string name = "sub" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"-\"];\n";
    output += parent + " -> " + name + ";\n";
    output += getLVal()->dot(name);
    output += getRVal()->dot(name);
    return output;
}

std::string AstMulOp::dot(std::string parent) {
    std::string name = "mul" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"*\"];\n";
    output += parent + " -> " + name + ";\n";
    output += getLVal()->dot(name);
    output += getRVal()->dot(name);
    return output;
}

std::string AstDivOp::dot(std::string parent) {
    std::string name = "div" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"/\"];\n";
    output += parent + " -> " + name + ";\n";
    output += getLVal()->dot(name);
    output += getRVal()->dot(name);
    return output;
}

std::string AstAndOp::dot(std::string parent) {
    std::string name = "and" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"&\"];\n";
    output += parent + " -> " + name + ";\n";
    output += getLVal()->dot(name);
    output += getRVal()->dot(name);
    return output;
}

std::string AstOrOp::dot(std::string parent) {
    std::string name = "or" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"|\"];\n";
    output += parent + " -> " + name + ";\n";
    output += getLVal()->dot(name);
    output += getRVal()->dot(name);
    return output;
}

std::string AstXorOp::dot(std::string parent) {
    std::string name = "xor" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"^\"];\n";
    output += parent + " -> " + name + ";\n";
    output += getLVal()->dot(name);
    output += getRVal()->dot(name);
    return output;
}

std::string AstEQOp::dot(std::string parent) {
    std::string name = "eq" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"=\"];\n";
    output += parent + " -> " + name + ";\n";
    output += getLVal()->dot(name);
    output += getRVal()->dot(name);
    return output;
}

std::string AstNEQOp::dot(std::string parent) {
    std::string name = "neq" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"!=\"];\n";
    output += parent + " -> " + name + ";\n";
    output += getLVal()->dot(name);
    output += getRVal()->dot(name);
    return output;
}

std::string AstGTOp::dot(std::string parent) {
    std::string name = "gt" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\">\"];\n";
    output += parent + " -> " + name + ";\n";
    output += getLVal()->dot(name);
    output += getRVal()->dot(name);
    return output;
}

std::string AstLTOp::dot(std::string parent) {
    std::string name = "lt" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"<\"];\n";
    output += parent + " -> " + name + ";\n";
    output += getLVal()->dot(name);
    output += getRVal()->dot(name);
    return output;
}

std::string AstGTEOp::dot(std::string parent) {
    std::string name = "gte" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\">=\"];\n";
    output += parent + " -> " + name + ";\n";
    output += getLVal()->dot(name);
    output += getRVal()->dot(name);
    return output;
}

std::string AstLTEOp::dot(std::string parent) {
    std::string name = "lte" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"<=\"];\n";
    output += parent + " -> " + name + ";\n";
    output += getLVal()->dot(name);
    output += getRVal()->dot(name);
    return output;
}

std::string AstLogicalAndOp::dot(std::string parent) {
    std::string name = "and" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"and\"];\n";
    output += parent + " -> " + name + ";\n";
    output += getLVal()->dot(name);
    output += getRVal()->dot(name);
    return output;
}

std::string AstLogicalOrOp::dot(std::string parent) {
    std::string name = "or" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"or\"];\n";
    output += parent + " -> " + name + ";\n";
    output += getLVal()->dot(name);
    output += getRVal()->dot(name);
    return output;
}

std::string AstChar::dot(std::string parent) {
    std::string name = "char" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"\'" + getValue() + "\'\"];\n";
    output += parent + " -> " + name + ";\n";
    return output;
}

std::string AstString::dot(std::string parent) {
    std::string name = "string" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"\\\"" + getValue() + "\\\"\"];\n";
    output += parent + " -> " + name + ";\n";
    return output;
}

std::string AstI32::dot(std::string parent) {
    std::string name = "int" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"" + std::to_string(getValue()) + "\"];\n";
    output += parent + " -> " + name + ";\n";
    return output;
}

std::string AstID::dot(std::string parent) {
    std::string name = "id" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"" + getValue() + "\"];\n";
    output += parent + " -> " + name + ";\n";
    return output;
}

std::string AstArrayAccess::dot(std::string parent) {
    std::string name = "array_acc" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"" + getValue() + "\"];\n";
    output += parent + " -> " + name + ";\n";
    output += getIndex()->dot(name);
    return output;
}

std::string AstStructAccess::dot(std::string parent) {
    std::string name = "struct_acc" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"" + getName() + "." + getMember() + "\"];\n";
    output += parent + " -> " + name + ";\n";
    return output;
}

std::string AstFuncCallExpr::dot(std::string parent) {
    std::string name = "func_call_expr" + std::to_string(idx);
    ++idx;
    
    std::string output = name + "[label=\"" + getName() + "\"];\n";
    output += parent + " -> " + name + ";\n";
    output += getArgExpression()->dot(name);
    return output;
}

