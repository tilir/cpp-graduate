#pragma once

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#include <iostream>

class NumLexer : public yyFlexLexer {
  std::string current_lexem;
  std::string current_value;
  
  int process_plus() {
    current_lexem = "operator";
    current_value = "+";
    return 1;
  }
  
  int process_minus() {
    current_lexem = "operator";
    current_value = "-";
    return 1;
  }
  
  int process_eq() {
    current_lexem = "operator";
    current_value = "=";
    return 1;
  }
  
  int process_digit() {
    current_lexem = "number";
    current_value = yytext;
    return 1;
  }
  
  int process_unknown() {
    current_lexem = "unknown";
    current_value = "?";
    return 1;
  }

public:
  int yylex() override;
  void print_current() const {
    std::cout << current_lexem << " <" << current_value << ">" << std::endl;
  }
};
