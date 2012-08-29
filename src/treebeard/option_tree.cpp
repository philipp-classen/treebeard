#include "parser.h"
#include "option_tree.h"

#include <boost/algorithm/string/predicate.hpp>

namespace
{
  std::string indent(int current_level)
  {
    TREEBEARD_ASSERT(current_level >= 0);
    std::string result;
    return result.assign(2 * current_level, ' ');
  }
}

namespace treebeard
{

  expanded_completions::expanded_completions(
    std::vector<std::string>&& all_completions_,
    bool is_complete_, const option_tree* tree_) :
      all_completions(std::move(all_completions_)),
      is_complete(is_complete_),
      tree(tree_)
  {
    TREEBEARD_ASSERT(!all_completions.empty());
    TREEBEARD_ASSERT(tree != nullptr);
  }

  expanded_completions::expanded_completions(expanded_completions&& rhs) :
    all_completions(std::move(rhs.all_completions)),
    is_complete(rhs.is_complete),
    tree(rhs.tree)
  {
    TREEBEARD_ASSERT(!all_completions.empty());
    TREEBEARD_ASSERT(tree != nullptr);
  }

  expanded_completions& expanded_completions::operator=(
    expanded_completions&& rhs)
  {
    all_completions = std::move(rhs.all_completions);
    is_complete = rhs.is_complete;
    tree = rhs.tree;
    return *this;
  }


  // ----------------------------------------------------------------------

  void match_result::add_matches(expanded_completions&& matches)
  {
    all_matches.push_back(std::move(matches));
  }

  void match_result::merge(match_result&& other_matches)
  {
    for(expanded_completions& elem : other_matches.all_matches)
      all_matches.push_back(std::move(elem));
  }

  std::vector<std::string> match_result::flatten_completions(
    match_result&& matches)
  {
    std::vector<std::string> result;
    for(expanded_completions& elem : matches.all_matches)
      for(std::string& completion : elem.all_completions)
	result.push_back(std::move(completion));

    return result;
  }

  // ----------------------------------------------------------------------

  option_tree::~option_tree()
  {
  }

  // ----------------------------------------------------------------------

  option_leaf::~option_leaf()
  {
  }

  // ----------------------------------------------------------------------

  /* Seems to be broken in gcc 4.7
  fixed_string_option_leaf::fixed_string_option_leaf(const char* fixed_string_)
  {
    fixed_string_option_leaf(std::string(fixed_string_));
  }
  */

  fixed_string_option_leaf::fixed_string_option_leaf(const char* fixed_string_) :
    fixed_string(fixed_string_)
  {
    TREEBEARD_ASSERT(!fixed_string.empty());
  }

  fixed_string_option_leaf::fixed_string_option_leaf(
    std::string fixed_string_) :
      fixed_string(std::move(fixed_string_))
  {
    TREEBEARD_ASSERT(!fixed_string.empty());
  }

  fixed_string_option_leaf::~fixed_string_option_leaf()
  {
  }

  match_result fixed_string_option_leaf::find_matches(
    const std::string& current_token, const std::string& lookahead) const
  {
    match_result result;
    if(boost::starts_with(fixed_string, current_token))
    {
      const bool is_complete = fixed_string == current_token;
      result.add_matches(
        expanded_completions({ fixed_string }, is_complete, this));
    }
    return result;
  }

  bool fixed_string_option_leaf::is_complete_match(
    const std::string& current_token) const
  {
    return fixed_string == current_token;
  }

  std::string fixed_string_option_leaf::describe(int current_level) const
  {
    return indent(current_level) + '<' + fixed_string + '>';
  }

  // ----------------------------------------------------------------------

  multiple_option_tree::multiple_option_tree(
    std::vector<std::unique_ptr<option_tree>> options_) :
      options(std::move(options_))
  {
    TREEBEARD_ASSERT(options.size() >= 2);
  }

  multiple_option_tree::~multiple_option_tree()
  {
  }

  match_result multiple_option_tree::find_matches(
    const std::string& current_token, const std::string& lookahead) const
  {
    match_result result;
    for(const auto& next_option_tree : options)
      result.merge(next_option_tree->find_matches(current_token, lookahead));
    return result;
  }

  std::string multiple_option_tree::describe(int current_level) const
  {
    std::string result = indent(current_level) + "one_of\n";
    for(const auto& next_option_tree : options)
    {
      result += next_option_tree->describe(current_level + 1) + '\n';
    }
    return result;
  }

  // ----------------------------------------------------------------------

  consecutive_option_tree::consecutive_option_tree(
    std::unique_ptr<option_leaf> predecessor_,
    std::unique_ptr<option_tree> successor_) :
      predecessor(std::move(predecessor_)),
      successor(std::move(successor_))
  {
    TREEBEARD_ASSERT(predecessor != nullptr);
    TREEBEARD_ASSERT(successor   != nullptr);
  }

  consecutive_option_tree::~consecutive_option_tree()
  {
  }

  match_result consecutive_option_tree::find_matches(
    const std::string& current_token, const std::string& lookahead) const
  {
    if(lookahead.empty())
    {
      // no lookahead available
      // -> only the predecessor must match at least partly
      return predecessor->find_matches(current_token, lookahead);
    }
    else
    {
      // lookahead available
      // -> the predecessor must be completed and the successor
      //    must match at least partly
      if(predecessor->is_complete_match(current_token))
      {
	const parser::tokenize_result& tokenized_lookahead =
	  parser::tokenize(lookahead);
	return successor->find_matches(tokenized_lookahead.next_token,
				       tokenized_lookahead.remaining_line);
      }
      else
      {
	match_result empty_result;
	return empty_result;
      }
    }
  }

  std::string consecutive_option_tree::describe(int current_level) const
  {
    return indent(current_level) + "first\n" +
      predecessor->describe(current_level + 1) +
      '\n' + indent(current_level) + "followed by\n" +
      successor->describe(current_level + 1);
  }

} // end namespace treebeard
