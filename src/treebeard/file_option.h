#ifndef HAVE_TREEBEARD_LIB_FILE_OPTION_H
#define HAVE_TREEBEARD_LIB_FILE_OPTION_H

#include <string>
#include <vector>
#include <memory>
#include <initializer_list>

#include "treebeard/common.h"
#include "treebeard/option_tree.h"

namespace treebeard
{
  class file_filter
  {
  public:
    file_filter(const char* filter_);
    file_filter(std::string filter_);

    const std::string& to_string() const;
  private:
    std::string filter;
  };

  class file_option_leaf : public dynamic_option_leaf
  {
  public:
    file_option_leaf();

    template <typename... Args>
    file_option_leaf(Args... filter);

    file_option_leaf(std::vector<std::unique_ptr<file_filter>>&& filter_list_);

    virtual ~file_option_leaf();

    virtual match_result find_matches(
      const std::string& current_token,
      const std::string& lookahead) const override;

    virtual bool is_complete_match(
      const std::string& current_token) const override;

    // debugging options
    virtual std::string describe(int current_level) const override;

  private:
    std::vector<std::unique_ptr<file_filter>> filter_list;
  };

  // ------------ implementation ------------

  namespace treebeard_impl
  {
    inline void fill_file_filter(std::vector<std::unique_ptr<file_filter>>&)
    {
      // end of recursion
    }

    template <typename Arg1, typename... Args>
    void fill_file_filter(std::vector<std::unique_ptr<file_filter>>& target,
			  Arg1 arg1, Args... args)
    {
      target.push_back(std::unique_ptr<file_filter>(
        new file_filter(std::forward<Arg1>(arg1))));
      fill_file_filter(target, std::forward<Args>(args)...);
    }

  }

  template <typename... Args>
  file_option_leaf::file_option_leaf(Args... filter)
  {
    treebeard_impl::fill_file_filter(
      filter_list, std::forward<Args>(filter)...);
    TREEBEARD_ASSERT(filter_list.size() == sizeof...(Args));
  }

} // end namespace treebeard

#endif // inclusion guard
