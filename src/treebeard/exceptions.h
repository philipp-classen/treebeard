#ifndef HAVE_TREEBEARD_LIB_EXCEPTIONS_H
#define HAVE_TREEBEARD_LIB_EXCEPTIONS_H

#include <exception>
#include <string>

#include "treebeard/common.h"

namespace treebeard
{
  class treebeard_exception : public std::exception
  {
  };

  class end_of_line : public treebeard_exception
  {
  };

  class illegal_state_error : public treebeard_exception
  {
  public:
    illegal_state_error(std::string msg_) : msg(std::move(msg_)) {}
      
  private:
    std::string msg;
  };

} // end namespace treebeard

#endif // inclusion guard
