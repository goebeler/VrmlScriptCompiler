#ifndef VRML_SCANNER_H
#define VRML_SCANNER_H

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include <iostream>
#include "location.hh"
#include "vrml_parser.hpp"

//#include <iostream>

class VrmlScanner : public yyFlexLexer {
public:
  VrmlScanner(std::istream *in = nullptr, std::ostream *out = nullptr)
      : yyFlexLexer(in, out) {}

  virtual ~VrmlScanner() {}

  // Dies ist die Methode, die Bison aufruft
  yy::parser::symbol_type yylex(yy::location* yylloc);

  // Wrapper für YYText() und YYLeng() um die korrekten Methoden aufzurufen
  const char* YYText() const { return yyFlexLexer::YYText(); }
  int YYLeng() const { return yyFlexLexer::YYLeng(); }

  void LexerError(const char *msg) { std::cerr << msg << std::endl; }
};

#endif // VRML_SCANNER_H