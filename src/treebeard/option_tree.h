#ifndef HAVE_TREEBEARD_LIB_OPTION_TREE_H
#define HAVE_TREEBEARD_LIB_OPTION_TREE_H

#include <string>
#include <vector>
#include <memory>
#include <initializer_list>

#include "treebeard/common.h"

namespace treebeard
{
  // forward definitions
  class option_leaf;
  class option_tree;

  // helper class (TODO: Hide in separate namespace)
  struct expanded_completions
  {
    expanded_completions(std::vector<std::string>&& all_completions_,
			 bool is_complete_, const option_tree* tree_);

    expanded_completions(expanded_completions&& rhs);
    expanded_completions& operator=(expanded_completions&& rhs);

    std::vector<std::string> all_completions;

    // Normally, true means that the last token is contained in the list of
    // all_completions. At least for fixed strings, it should be true.
    //
    // TODO: Maybe it is not true for dynamic options
    //       (which are not implemented yet)
    bool is_complete;

    // TODO: Is this needed?
    const option_tree* tree;
  };

  // helper class (TODO: Hide in separate namespace)
  class match_result
  {
  public:
    void add_matches(expanded_completions&& matches);
    void merge(match_result&& other_matches);

    static std::vector<std::string> flatten_completions(match_result&& matches);

  private:
    std::vector<expanded_completions> all_matches;
  };

  class option_tree
  {
  public:
    virtual ~option_tree() = 0;

    virtual match_result find_matches(const std::string& current_token,
				      const std::string& lookahead) const = 0;

    // debugging options
    virtual std::string describe(int current_level) const = 0;

  };

  // matches only a single token
  class option_leaf : public option_tree
  {
  public:
    virtual ~option_leaf() = 0;

    virtual bool is_complete_match(const std::string& current_token) const = 0;

  };

  class dynamic_option_leaf : public option_leaf
  {
  public:
    virtual ~dynamic_option_leaf() = 0;
  };

  class fixed_string_option_leaf : public option_leaf
  {
  public:
    fixed_string_option_leaf(const char* fixed_string_);
    fixed_string_option_leaf(std::string fixed_string_);

    virtual ~fixed_string_option_leaf();

    virtual match_result find_matches(
      const std::string& current_token,
      const std::string& lookahead) const override;

    virtual bool is_complete_match(
      const std::string& current_token) const override;

    // debugging options
    virtual std::string describe(int current_level) const override;

  private:
    std::string fixed_string;
  };

  class multiple_option_tree : public option_tree
  {
  public:

    multiple_option_tree(std::vector<std::unique_ptr<option_tree>> options_);

    virtual ~multiple_option_tree();

    virtual match_result find_matches(
      const std::string& current_token,
      const std::string& lookahead) const override;

    // debugging options
    virtual std::string describe(int current_level) const override;

  private:

    std::vector<std::unique_ptr<option_tree>> options;

  };

  class consecutive_option_tree : public option_tree
  {
  public:
    consecutive_option_tree(std::unique_ptr<option_leaf> predecessor_,
			    std::unique_ptr<option_tree> successor_);

    virtual ~consecutive_option_tree();

    virtual match_result find_matches(
      const std::string& current_token,
      const std::string& lookahead) const override;

    // debugging options
    virtual std::string describe(int current_level) const override;

  private:

    // must only match a single token
    std::unique_ptr<option_leaf> predecessor;

    std::unique_ptr<option_tree> successor;
    
  };

} // end namespace treebeard

#endif // inclusion guard
