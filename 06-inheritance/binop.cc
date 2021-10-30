//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Different nodes in the same vector
//
//----------------------------------------------------------------------------

#include <vector>

enum Node_t {
  SCOPE,
  UNOP,
  BINOP,
};

enum UnOp_t { INC, DEC };

enum BinOp_t { ADD, SUB, MULT, DIV };

struct Node {
  Node *parent;
  Node_t type_;
};

struct BinOp : public Node {
  BinOp_t op_;
  Node *lhs_, *rhs_;
  BinOp(Node *parent, BinOp_t opcode)
      : Node{parent, Node_t::BINOP}, op_(opcode) {}
};

struct UnOp : public Node {
  UnOp_t op_;
  Node *rhs_;
  UnOp(Node *parent, UnOp_t opcode) : Node{parent, Node_t::UNOP}, op_(opcode) {}
};

int main() {
  // type-erased container
  std::vector<Node *> nodes;
  nodes.push_back(new BinOp(nullptr, ADD));
  nodes.push_back(new UnOp(nullptr, INC));
  using ItTy = typename std::vector<Node *>::iterator;

  for (ItTy it = nodes.begin(), ite = nodes.end(); it != ite; ++it) {
    Node *elt = *it;
    if (elt->type_ == Node_t::BINOP) {
      BinOp *p = static_cast<BinOp *>(elt);
      delete p;
    } else if (elt->type_ == Node_t::UNOP) {
      UnOp *p = static_cast<UnOp *>(elt);
      delete p;
    }
  }
}
