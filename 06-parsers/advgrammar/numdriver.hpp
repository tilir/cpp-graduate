#pragma once

#include <iostream>
#include <FlexLexer.h>
#include "numgrammar.tab.hh"

namespace yy {
  
class NumDriver {
  FlexLexer *plex_;
  std::vector<std::vector<std::vector<int>>> vvv_;
public:
  NumDriver (FlexLexer *plex) : plex_(plex) {}

  parser::token_type yylex(parser::semantic_type* yylval) {
    parser::token_type tt = static_cast<parser::token_type>(plex_->yylex());
    if (tt == yy::parser::token_type::NUMBER)
      yylval->as<int>() = std::stoi(plex_->YYText());
    return tt;
  }
  
  bool parse() {
    parser parser(this);
    bool res = parser.parse();
    return !res;
  }
  
  void insert(std::vector<std::vector<std::vector<int>>> v) {
    vvv_.assign(v.begin(), v.end());
  }
  
  void printout() const {
    for (auto vv : vvv_) {
      int fst = 0;
      for (auto v : vv) {
        int sum = 0;
        for (auto x : v)
          sum += x;
        std::cout << sum;
        if (fst == 0)
          std::cout << " == ";
        fst = 1;
      }
      std::cout << std::endl;
    }
  }
};  

}