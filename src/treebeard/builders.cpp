#include "treebeard/builders.h"

namespace treebeard
{
  std::unique_ptr<option_tree> one_of(
    const std::vector<std::string>& fixed_strings)
  {
    std::vector<std::unique_ptr<option_tree>> choices;
    for(const std::string& next_option : fixed_strings)
    {
      choices.push_back(std::unique_ptr<option_tree>(
	new fixed_string_option_leaf(next_option)));
    }

    return std::unique_ptr<option_tree>(
      new multiple_option_tree(std::move(choices)));
  }

  std::unique_ptr<option_tree> make_option_tree(const char* value)
  {
    return std::unique_ptr<option_tree>(new fixed_string_option_leaf(value));
  }

  std::unique_ptr<option_tree> make_option_tree(const std::string& value)
  {
    return std::unique_ptr<option_tree>(new fixed_string_option_leaf(value));
  }

  // ----------------------------------------------------------------------

  std::unique_ptr<option_leaf> make_option_leaf(const char* value)
  {
    return std::unique_ptr<option_leaf>(new fixed_string_option_leaf(value));
  }

  std::unique_ptr<option_leaf> make_option_leaf(const std::string& value)
  {
    return std::unique_ptr<option_leaf>(new fixed_string_option_leaf(value));
  }


} // end namespace treebeard
