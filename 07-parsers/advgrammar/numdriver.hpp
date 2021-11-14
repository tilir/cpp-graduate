//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Object-oriented parser with some state
//
//----------------------------------------------------------------------------

#pragma once

#include <iostream>
#include <numeric>

#include "numgrammar.tab.hh"
#include <FlexLexer.h>

namespace yy {

class NumDriver {
  FlexLexer *plex_;
  std::vector<std::pair<std::vector<int>, std::vector<int>>> vvv_;

public:
  NumDriver(FlexLexer *plex) : plex_(plex) {}

  parser::token_type yylex(parser::semantic_type *yylval) {
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

  void insert(std::vector<std::pair<std::vector<int>, std::vector<int>>> v) {
    vvv_.assign(v.rbegin(), v.rend());
  }

  void printout() const {
    for (auto vv : vvv_) {
      int sumleft = std::accumulate(vv.first.begin(), vv.first.end(), 0);
      int sumright = std::accumulate(vv.second.begin(), vv.second.end(), 0);
      int res = (sumleft == sumright);
      std::cout << "Checking: " << sumleft << " vs " << sumright
                << "; Result: " << res << std::endl;
    }
  }
};

} // namespace yy