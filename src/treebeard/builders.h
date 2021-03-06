#ifndef HAVE_TREEBEARD_LIB_BUILDERS_H
#define HAVE_TREEBEARD_LIB_BUILDERS_H

#include <memory>
#include <vector>
#include <string>
#include <type_traits>

#include "treebeard/common.h"
#include "treebeard/option_tree.h"
#include "treebeard/file_option.h"

namespace treebeard
{
  std::unique_ptr<option_tree> one_of(
    const std::vector<std::string>& fixed_strings);

  template <typename... Args>
  std::unique_ptr<option_tree> one_of(Args... options);

  template <typename Arg1, typename Arg2>
  std::unique_ptr<option_tree> concat(Arg1 predecessor, Arg2 successor);
  template <typename Arg1, typename... Args>
  std::unique_ptr<option_tree> concat(Arg1 predecessor, Args... successors);

  std::unique_ptr<option_tree> make_option_tree(const char* value);
  std::unique_ptr<option_tree> make_option_tree(const std::string& value);

  template <typename Arg1>
  typename std::enable_if<std::is_base_of<option_tree, Arg1>::value,
    std::unique_ptr<option_tree>>::type make_option_tree(
    std::unique_ptr<Arg1>&& value)
  {
    return std::move(value);
  }

  std::unique_ptr<option_leaf> make_option_leaf(const char* value);
  std::unique_ptr<option_leaf> make_option_leaf(const std::string& value);
  std::unique_ptr<option_leaf> make_option_leaf(
    std::unique_ptr<option_leaf>&& value);
  template <typename Arg1>
  typename std::enable_if<std::is_base_of<option_leaf, Arg1>::value,
    std::unique_ptr<option_leaf>>::type make_option_leaf(
    std::unique_ptr<Arg1>&& value)
  {
    return std::move(value);
  }

  template <typename... Args>
  std::unique_ptr<file_option_leaf> file(Args... filter);

  // ------------ implementation ------------

  namespace treebeard_impl
  {
    inline void one_of__construction_helper__(
      std::vector<std::unique_ptr<option_tree>>& converted_option_trees)
    {
      // end of recursion
    }

    template <typename Arg1, typename... Args>
    void one_of__construction_helper__(
      std::vector<std::unique_ptr<option_tree>>& converted_option_trees,
      Arg1 arg1, Args... args)
    {
      converted_option_trees.push_back(
	make_option_tree(std::forward<Arg1>(arg1)));
      one_of__construction_helper__(converted_option_trees,
				    std::forward<Args>(args)...);
    }
  } // end treebeard_impl

  template <typename... Args>
  std::unique_ptr<option_tree> one_of(Args... options)
  {
    using namespace treebeard_impl;
    std::vector<std::unique_ptr<option_tree>> converted_option_trees;
    one_of__construction_helper__(converted_option_trees,
				  std::forward<Args>(options)...);
    return std::unique_ptr<option_tree>(
      new multiple_option_tree(std::move(converted_option_trees)));
  }

  template <typename Arg1, typename Arg2>
  std::unique_ptr<option_tree> concat(Arg1 predecessor, Arg2 successor)
  {
    using namespace treebeard_impl;
    return std::unique_ptr<option_tree>(new consecutive_option_tree(
      std::move(make_option_leaf(std::forward<Arg1>(predecessor))),
      std::move(make_option_tree(std::forward<Arg2>(successor)))));
  }

  template <typename Arg1, typename... Args>
  std::unique_ptr<option_tree> concat(Arg1 predecessor, Args... successors)
  {
    return concat(predecessor, concat(successors...));
  }

  template <typename... Args>
  std::unique_ptr<file_option_leaf> file(Args... filter)
  {
    return std::unique_ptr<file_option_leaf>(
      new file_option_leaf(std::forward<Args...>(filter...)));
  }

} // end namespace treebeard

#endif // inclusion guard
