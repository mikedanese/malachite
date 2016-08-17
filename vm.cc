#include <string>
#include <memory>
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <list>
#include <algorithm>
#include <functional>

class VM;
class StackFrame;
class Val;
class FuncVal;
class IntVal;

class Val {
 public:
  virtual ~Val(){};
};

enum VmOp {
  SAVE,
  LOAD,
};

class NativeFuncVal : public Val {
 public:
  std::function<void(VM*)> f_;

  explicit NativeFuncVal(std::function<void(VM*)> f) : f_(f){};
};

class VmOpFuncVal : public Val {
 public:
  VmOp op_;

  explicit VmOpFuncVal(VmOp op) : op_(op){};
};

class IntVal : public Val {
 public:
  long val_;
  explicit IntVal(long val) : val_(val){};
};

class StringVal : public Val {
 public:
  std::string val_;
  explicit StringVal(std::string val) : val_(val){};
};

class ErrorVal : public Val {
 public:
  std::string val_;
  explicit ErrorVal(std::string val) : val_(val){};
};

std::string val_to_string(const Val* v) {
  std::ostringstream out;
  if (auto e = dynamic_cast<const NativeFuncVal*>(v)) {
    return "NATIVE_FUNC";
  } else if (auto e = dynamic_cast<const VmOpFuncVal*>(v)) {
    return "VM_OP_FUNC";
  } else if (auto e = dynamic_cast<const IntVal*>(v)) {
    out << "INT(" << dynamic_cast<const IntVal*>(v)->val_ << ")";
    return out.str();
  } else if (auto e = dynamic_cast<const StringVal*>(v)) {
    out << "STRING(" << dynamic_cast<const StringVal*>(v)->val_ << ")";
    return out.str();
  } else if (auto e = dynamic_cast<const ErrorVal*>(v)) {
    out << "ERROR(" << dynamic_cast<const ErrorVal*>(v)->val_ << ")";
    return out.str();
  } else {
    return "UNKOWN";
  }
};

class StackFrame {
 public:
  const Val* data_;
  std::unordered_map<std::string, const Val*> scope_;

  StackFrame(const Val* data) : data_(data) {
    scope_.reserve(10);
  };
};

class VM {
  std::vector<StackFrame> stack_;
  std::list<const Val*> heap_;
  std::unordered_map<std::string, const Val*> global_scope_;

 public:
  int Push(const Val* e);

  int PushError(std::string error);

  template <typename T>
  const T* Pop();

  template <typename T, typename... Args>
  const T* Alloc(Args...);

  int Call();

  void DumpState();

 private:
  int Load();

  int Save();
};

#define for_each_frame(stack, f) \
  for (auto f = stack.begin(); f != stack_.end(); f++)
#define rfor_each_frame(stack, f) \
  for (auto f = stack.rbegin(); f != stack_.rend(); f++)

int VM::Push(const Val* e) {
  stack_.emplace_back<StackFrame>(e);
  return 0;
};

int VM::PushError(std::string err) {
  stack_.push_back(Alloc<ErrorVal>(err));
  return 0;
};

template <typename T>
const T* VM::Pop() {
  auto f = stack_.back();
  stack_.pop_back();
  if (auto v = dynamic_cast<const T*>(f.data_)) {
    return v;
  }
  exit(1);
};

template <typename T, typename... Args>
const T* VM::Alloc(Args... args) {
  auto p = new T(args...);
  heap_.push_back(p);
  return p;
};

int VM::Call() {
  auto v = Pop<Val>();
  if (auto f = dynamic_cast<const NativeFuncVal*>(v)) {
    std::bind(f->f_, this)();
    return 0;
  } else if (auto f = dynamic_cast<const VmOpFuncVal*>(v)) {
    switch (f->op_) {
      case LOAD:
        return Load();
      case SAVE:
        return Save();
    }
  }
  PushError("tried to call something that wasn't a function");
  return 1;
};

int VM::Load() {
  auto i = Pop<StringVal>();
  std::cerr << "foo:\n";
  rfor_each_frame(stack_, f) {
    if (!f->scope_.count(i->val_))
      continue;
    Push(f->scope_[i->val_]);
    return 0;
  }
  if (global_scope_.count(i->val_)) {
    Push(global_scope_[i->val_]);
    return 0;
  }
  PushError("variable not in scope");
  return 1;
};

int VM::Save() {
  auto v = Pop<Val>();
  auto i = Pop<StringVal>();
  if (stack_.empty()) {
    global_scope_[i->val_] = v;
    return 0;
  }
  rfor_each_frame(stack_, f) {
    printf("here\n");
    if (!f->scope_.count(i->val_))
      continue;
    f->scope_[i->val_] = v;
    return 0;
  }
  stack_.back().scope_[i->val_] = v;
  return 0;
};

void VM::DumpState() {
  int size = stack_.size();
  int i = 0;

  std::cerr << "stack:\n";
  rfor_each_frame(stack_, f) {
    std::cerr << "\t" << size - i << ":" << val_to_string(f->data_) << "\n";
    i++;
  }
}

int main(int argc, char** argv) {
  NativeFuncVal f([](VM* vm) {
    vm->Push(
        vm->Alloc<IntVal>(vm->Pop<IntVal>()->val_ + vm->Pop<IntVal>()->val_));
  });
  VM vm;
  vm.DumpState();
  vm.Push(vm.Alloc<IntVal>(1));
  vm.DumpState();
  vm.Push(vm.Alloc<IntVal>(2));
  vm.DumpState();
  vm.Push(&f);
  vm.DumpState();
  vm.Call();
  vm.DumpState();
  vm.Pop<Val>();

  vm.Push(vm.Alloc<StringVal>("a"));
  vm.Push(vm.Alloc<IntVal>(2));
  vm.Push(vm.Alloc<VmOpFuncVal>(SAVE));
  vm.DumpState();
  vm.Call();
  vm.DumpState();

  vm.Push(vm.Alloc<StringVal>("a"));
  vm.Push(vm.Alloc<VmOpFuncVal>(LOAD));
  vm.DumpState();
  vm.Call();
  vm.DumpState();
  return 0;
};
