//
// Copyright 2021-2022 Patrick Flynn
// This file is part of the Tiny Lang compiler.
// Tiny Lang is licensed under the BSD-3 license. See the COPYING file for more information.
//
#pragma once

#include <string>
#include <vector>

#include <ast/Types.hpp>

class AstStatement;

// Represents a function, external declaration, or global variable
class AstGlobalStatement {
public:
    explicit AstGlobalStatement() {}
    explicit AstGlobalStatement(AstType type) {
        this->type = type;
    }
    
    AstType getType() { return type; }
    
    virtual void print() {}
private:
    AstType type = AstType::EmptyAst;
};

// Represents an extern function
class AstExternFunction : public AstGlobalStatement {
public:
    explicit AstExternFunction(std::string name) : AstGlobalStatement(AstType::ExternFunc) {
        this->name = name;
    }
    
    void addArgument(Var arg) { this->args.push_back(arg); }
    void setArguments(std::vector<Var> args) { this->args = args; }
    
    void setDataType(DataType dataType) {
        this->dataType = dataType;
    }
    
    void setVarArgs() { this->varargs = true; }
    bool isVarArgs() { return this->varargs; }
    
    std::string getName() { return name; }
    DataType getDataType() { return dataType; }
    std::vector<Var> getArguments() { return args; }
    void print() override;
private:
    std::string name = "";
    std::vector<Var> args;
    DataType dataType = DataType::Void;
    bool varargs = false;
};

// Represents a function
class AstFunction : public AstGlobalStatement {
public:
    explicit AstFunction(std::string name) : AstGlobalStatement(AstType::Func) {
        this->name = name;
        block = new AstBlock;
    }
    
    std::string getName() { return name; }
    DataType getDataType() { return dataType; }
    DataType getPtrType() { return ptrType; }
    std::string getDataTypeName() { return dtName; }
    std::vector<Var> getArguments() { return args; }
    AstBlock *getBlock() { return block; }
    
    void setName(std::string name) { this->name = name; }
    
    void setArguments(std::vector<Var> args) { this->args = args; }
    
    void addStatement(AstStatement *statement) {
        block->addStatement(statement);
    }
    
    void setDataType(DataType dataType, DataType ptrType) {
        this->dataType = dataType;
        this->ptrType = ptrType;
    }
    
    void setDataTypeName(std::string name) {
        this->dtName = name;
    }
    
    void print() override;
private:
    std::string name = "";
    std::vector<Var> args;
    AstBlock *block;
    DataType dataType = DataType::Void;
    DataType ptrType = DataType::Void;
    std::string dtName = "";
};
