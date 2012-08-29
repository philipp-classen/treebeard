#ifndef HAVE_TREEBEARD_LIB_PARSER_H
#define HAVE_TREEBEARD_LIB_PARSER_H

#include <memory>
#include <string>
#include <vector>

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

#include "treebeard/common.h"

namespace treebeard
{
  class option_tree; // forward

  class parser
  {
  public:
    explicit parser(const option_tree* option_root_);

    std::vector<std::string> find_all_matches(const std::string& line);

    struct tokenize_result
    {
      std::string next_token;
      std::string remaining_line;
    };
    static tokenize_result tokenize(const std::string& line);

  private:

    // non-null; must not be freed
    const option_tree *const option_root;

  };

} // end namespace treebeard

#endif // inclusion guard
