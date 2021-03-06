//===- MILexer.h - Lexer for machine instructions -------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the function that lexes the machine instruction source
// string.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_CODEGEN_MIRPARSER_MILEXER_H
#define LLVM_LIB_CODEGEN_MIRPARSER_MILEXER_H

#include "llvm/ADT/APSInt.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/STLExtras.h"
#include <functional>

namespace llvm {

class Twine;

/// A token produced by the machine instruction lexer.
struct MIToken {
  enum TokenKind {
    // Markers
    Eof,
    Error,

    // Tokens with no info.
    comma,
    equal,

    // Identifier tokens
    Identifier,
    NamedRegister,

    // Other tokens
    IntegerLiteral
  };

private:
  TokenKind Kind;
  StringRef Range;
  APSInt IntVal;

public:
  MIToken(TokenKind Kind, StringRef Range) : Kind(Kind), Range(Range) {}

  MIToken(TokenKind Kind, StringRef Range, const APSInt &IntVal)
      : Kind(Kind), Range(Range), IntVal(IntVal) {}

  TokenKind kind() const { return Kind; }

  bool isError() const { return Kind == Error; }

  bool isRegister() const { return Kind == NamedRegister; }

  bool is(TokenKind K) const { return Kind == K; }

  bool isNot(TokenKind K) const { return Kind != K; }

  StringRef::iterator location() const { return Range.begin(); }

  StringRef stringValue() const { return Range; }

  const APSInt &integerValue() const { return IntVal; }
};

/// Consume a single machine instruction token in the given source and return
/// the remaining source string.
StringRef lexMIToken(
    StringRef Source, MIToken &Token,
    function_ref<void(StringRef::iterator, const Twine &)> ErrorCallback);

} // end namespace llvm

#endif
