#ifndef AST_H_
#define AST_H_

namespace serpentine {
namespace ast {

enum Type {
  ATOM,
  APPLY,
  ARGS,
  INDEX,
  ARRAY,
  INFIX,
  CONDITIONAL,
  IMPORT,
  LITERAL_BOOLEAN,
  LITERAL_NULL,
  LITERAL_NUMBER,
  LITERAL_STRING,
  OBJECT,
};

enum InfixOp {
  MULT,
  DIV,
  PERCENT,
  PLUS,
  MINUS,
  SHIFT_L,
  SHIFT_R,
  GREATER,
  GREATER_EQ,
  LESS,
  LESS_EQ,
  EQUAL,
  UNEQUAL,
  BITWISE_AND,
  BITWISE_XOR,
  BITWISE_OR,
  AND,
  OR,
};

struct Node {
 public:
  Type type_;
  std::string fodder;

  Node(Type type, std::string fodder);
};

struct Atom : public Node {
 public:
  Atom(std::string fodder) : Node(Type::ATOM, fodder){};
};

struct Apply : public Node {
 public:
  Apply(std::string fodder) : Node(Type::APPLY, fodder){};
};

struct Args : public Node {
 public:
  Args(std::string fodder) : Node(Type::ARGS, fodder){};
};

struct Index : public Node {
 public:
  Index(std::string fodder) : Node(Type::INDEX, fodder){};
};

struct Array : public Node {
 public:
  Array(std::string fodder) : Node(Type::ARRAY, fodder){};
};

struct Infix : public Node {
 public:
  Infix(std::string fodder) : Node(Type::INFIX, fodder){};
};

struct Conditional : public Node {
 public:
  Conditional(std::string fodder) : Node(Type::CONDITIONAL, fodder){};
};

struct Import : public Node {
 public:
  Import(std::string fodder) : Node(Type::IMPORT, fodder){};
};

struct LiteralBoolean : public Node {
 public:
  LiteralBoolean(std::string fodder) : Node(Type::LITERAL_BOOLEAN, fodder){};
};

struct LiteralNull : public Node {
 public:
  LiteralNull(std::string fodder) : Node(Type::LITERAL_NULL, fodder){};
};

struct LiteralNumber : public Node {
 public:
  LiteralNumber(std::string fodder) : Node(Type::LITERAL_NUMBER, fodder){};
};

struct LiteralString : public Node {
 public:
  LiteralString(std::string fodder) : Node(Type::LITERAL_STRING, fodder){};
};

struct Object : public Node {
 public:
  Object(std::string fodder) : Node(Type::OBJECT, fodder){};
};
};
};
#endif
