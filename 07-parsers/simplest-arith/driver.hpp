//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Driver for additions and multiplications grammar
//
//----------------------------------------------------------------------------

#pragma once

#include "simplest.tab.hh"
#include <FlexLexer.h>

namespace yy {

class Driver {
  FlexLexer *plex_;

public:
  Driver(FlexLexer *plex) : plex_(plex) {}

  parser::token_type yylex(parser::semantic_type *yylval) {
    parser::token_type tt = static_cast<parser::token_type>(plex_->yylex());
    if (tt == yy::parser::token_type::NUMBER)
      yylval->as<int>() = std::stoi(plex_->YYText());
    if (tt == yy::parser::token_type::ERR) {
      std::cout << "Lexical error, unrecoginzed lexem: '" << plex_->YYText()
                << "' at line #" << plex_->lineno() << std::endl;
      std::terminate();
    }
    return tt;
  }

  bool parse() {
    parser parser(this);
    bool res = parser.parse();
    return !res;
  }
};

} // namespace yy