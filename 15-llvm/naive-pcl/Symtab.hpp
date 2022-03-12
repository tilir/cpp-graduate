//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Symtab.hpp -- symbol table
//
//------------------------------------------------------------------------------

#pragma once

#include <string>
#include <unordered_map>

#include "INode.hpp"

class Symtab_local final {
  std::unordered_map<std::string, Node::INode *> Symbols_;

public:
  void add(std::string Symbol, Node::INode *Node) { Symbols_[Symbol] = Node; }

  Node::INode *find(std::string Symbol) const {
    auto It = Symbols_.find(Symbol);
    if (It == Symbols_.end())
      return nullptr;
    return It->second;
  }

  void free() {
    for (auto &Elt : Symbols_)
      delete Elt.second;
    Symbols_.clear();
  }
};

class Symtab final {
  std::unordered_map<const Node::IScope *, Symtab_local> Tables_;

public:
  void add(const Node::IScope *Scope, std::string Symbol, Node::INode *N) {
    Tables_[Scope].add(Symbol, N);
  }

  Node::INode *exists(const Node::IScope *Scope, std::string Symbol) const {
    auto It = Tables_.find(Scope);
    if (It == Tables_.end())
      return nullptr;
    return It->second.find(Symbol);
  }

  void free(const Node::IScope *Scope) {
    auto It = Tables_.find(Scope);
    if (It != Tables_.end())
      It->second.free();
  }
};

extern Symtab GlobalTable;
