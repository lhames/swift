//===--- ASTNode.cpp - Swift Language ASTs --------------------------------===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2017 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://swift.org/LICENSE.txt for license information
// See https://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//
//
// This file implements the ASTNode, which is a union of Stmt, Expr, and Decl.
//
//===----------------------------------------------------------------------===//

#include "swift/AST/ASTNode.h"
#include "swift/AST/Decl.h"
#include "swift/AST/Expr.h"
#include "swift/AST/Stmt.h"
#include "swift/AST/Pattern.h"
#include "swift/AST/TypeLoc.h"
#include "swift/Basic/SourceLoc.h"

using namespace swift;

SourceRange ASTNode::getSourceRange() const {
  if (const auto *E = this->dyn_cast<Expr*>())
    return E->getSourceRange();
  if (const auto *S = this->dyn_cast<Stmt*>())
    return S->getSourceRange();
  if (const auto *D = this->dyn_cast<Decl*>())
    return D->getSourceRange();
  if (const auto *P = this->dyn_cast<Pattern*>())
    return P->getSourceRange();
  if (const auto *L = this->dyn_cast<TypeLoc *>())
    return L->getSourceRange();
  llvm_unreachable("unsupported AST node");
}

/// Return the location of the start of the statement.
SourceLoc ASTNode::getStartLoc() const {
  return getSourceRange().Start;
}

/// Return the location of the end of the statement.
SourceLoc ASTNode::getEndLoc() const {
  return getSourceRange().End;
}

DeclContext *ASTNode::getAsDeclContext() const {
  if (auto *E = this->dyn_cast<Expr*>()) {
    if (isa<AbstractClosureExpr>(E))
      return static_cast<AbstractClosureExpr*>(E);
  } else if (is<Stmt*>()) {
    return nullptr;
  } else if (auto *D = this->dyn_cast<Decl*>()) {
    if (isa<DeclContext>(D))
      return cast<DeclContext>(D);
  } else if (getOpaqueValue())
    llvm_unreachable("unsupported AST node");
  return nullptr;
}

bool ASTNode::isImplicit() const {
  if (const auto *E = this->dyn_cast<Expr*>())
    return E->isImplicit();
  if (const auto *S = this->dyn_cast<Stmt*>())
    return S->isImplicit();
  if (const auto *D = this->dyn_cast<Decl*>())
    return D->isImplicit();
  if (const auto *P = this->dyn_cast<Pattern*>())
    return P->isImplicit();
  if (const auto *L = this->dyn_cast<TypeLoc*>())
    return false;
  llvm_unreachable("unsupported AST node");
}

void ASTNode::walk(ASTWalker &Walker) {
  if (auto *E = this->dyn_cast<Expr*>())
    E->walk(Walker);
  else if (auto *S = this->dyn_cast<Stmt*>())
    S->walk(Walker);
  else if (auto *D = this->dyn_cast<Decl*>())
    D->walk(Walker);
  else if (auto *P = this->dyn_cast<Pattern*>())
    P->walk(Walker);
  else
    llvm_unreachable("unsupported AST node");
}

void ASTNode::dump(raw_ostream &OS, unsigned Indent) const {
  if (auto S = dyn_cast<Stmt*>())
    S->dump(OS, /*context=*/nullptr, Indent);
  else if (auto E = dyn_cast<Expr*>())
    E->dump(OS, Indent);
  else if (auto D = dyn_cast<Decl*>())
    D->dump(OS, Indent);
  else if (auto P = dyn_cast<Pattern*>())
    P->dump(OS, Indent);
  else
    OS << "<null>";
}

void ASTNode::dump() const {
  dump(llvm::errs());
}

#define FUNC(T)                                                               \
bool ASTNode::is##T(T##Kind Kind) const {                                     \
  if (!is<T*>())                                                              \
    return false;                                                             \
  return get<T*>()->getKind() == Kind;                                        \
}
FUNC(Stmt)
FUNC(Expr)
FUNC(Decl)
FUNC(Pattern)
#undef FUNC
