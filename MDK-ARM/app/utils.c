#include "utils.h"
float fof_update(float input) {

  input_ = input;
  output_ = output_ * (1 - k_) + input_ * k_;
  return output_;
}
