#ifndef VM_H_
#define VM_H_
namespace serpentine {

class Frame {
 public:
  Frame(){};
};

class Stack {
  unsigned calls_;
  unsigned limit_;
  std::vector<Frame> stack_;

 public:
  Stack(unsigned limit) : calls_(0), limit_(limit){};
};

class VM {
 public:
  VM(){};
};
};
#endif
