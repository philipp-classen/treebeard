#include "treebeard/file_option.h"

#include <boost/filesystem.hpp>

namespace
{
  // TODO: Duplicated file_option <-> option_tree.cpp
  std::string indent(int current_level)
  {
    TREEBEARD_ASSERT(current_level >= 0);
    std::string result;
    return result.assign(2 * current_level, ' ');
  }
}

namespace treebeard
{
  file_filter::file_filter(const char* filter_) :
    filter(filter_)
  {
    TREEBEARD_ASSERT(!filter.empty());
  }

  file_filter::file_filter(std::string filter_) :
    filter(std::move(filter_))
  {
    TREEBEARD_ASSERT(!filter.empty());
  }

  const std::string& file_filter::to_string() const
  {
    return filter;
  }

  // ------------------------------------------------------------

  file_option_leaf::file_option_leaf()
  {
  }

  file_option_leaf::file_option_leaf(
    std::vector<std::unique_ptr<file_filter>>&& filter_list_) :
      filter_list(std::move(filter_list_))
  {
  }

  file_option_leaf::~file_option_leaf()
  {
  }

  match_result file_option_leaf::find_matches(
    const std::string& current_token, const std::string& lookahead) const
  {
    match_result result;
    if(!current_token.empty())
    {
      using namespace boost::filesystem;
      const auto& end = directory_iterator();
      try
      {
	for(auto it = directory_iterator(path(".")); it != end; ++it)
	{
	  //	  std::cout << *it << '\n';
	}
      }
      catch(boost::filesystem::filesystem_error&)
      {
	// ignore the following results
      }
    }
    return result;
  }

  bool file_option_leaf::is_complete_match(
      const std::string& current_token) const
  {
    return !current_token.empty() && boost::filesystem::exists(current_token);
  }

  std::string file_option_leaf::describe(int current_level) const
  {
    std::string result = indent(current_level) + "<file{";
    if(!filter_list.empty())
    {
      result += filter_list[0]->to_string();
      for(size_t i = 1; i < filter_list.size(); i++)
	result += ", " + filter_list[i]->to_string();
    }
    return std::move(result) + "}>";
  }


} // end namespace treebeard
