//===--- SILFunction.cpp - Defines the SILFunction data structure ---------===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2015 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See http://swift.org/LICENSE.txt for license information
// See http://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//

#include "swift/SIL/SILFunction.h"
#include "swift/SIL/SILModule.h"
using namespace swift;

SILFunction::SILFunction(SILModule &Module, SILLinkage Linkage,
                         StringRef Name, SILType LoweredType,
                         Optional<SILLocation> Loc,
                         IsTransparent_t isTrans)
  : ModuleAndLinkage(&Module, Linkage),
    Name(Name),
    LoweredType(LoweredType),
    Location(Loc),
    DeclCtx(nullptr),
    DebugScope(nullptr),
    Transparent(isTrans) {
  Module.functions.push_back(this);
}

SILFunction::~SILFunction() {
}

void SILFunction::setDeclContext(Decl *D) {
  if (!D) return;
  switch (D->getKind()) {
  case DeclKind::Func:        DeclCtx = cast<FuncDecl>(D)->getFuncExpr(); break;
  // These three dual-inherit from DeclContext.
  case DeclKind::Constructor: DeclCtx = cast<ConstructorDecl>(D); break;
  case DeclKind::Extension:   DeclCtx = cast<ExtensionDecl>(D);   break;
  case DeclKind::Destructor:  DeclCtx = cast<DestructorDecl>(D);  break;
  default:
    DeclCtx = D->getDeclContext();
  }
  assert(DeclCtx);
}

void SILFunction::setDeclContext(Expr *E) {
  if (!E) return;
  DeclCtx = dyn_cast<CapturingExpr>(E);
}

ASTContext &SILFunction::getASTContext() const {
  return getModule().getASTContext();
}
