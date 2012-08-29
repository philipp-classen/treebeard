#include "treebeard/option_tree.h"
#include "treebeard/parser.h"

namespace treebeard
{
  parser::parser(const option_tree* option_root_) :
    option_root(option_root_)
  {
    TREEBEARD_ASSERT(option_root != nullptr);
  }

  std::vector<std::string> parser::find_all_matches(const std::string& line)
  {
    TREEBEARD_ASSERT(option_root != nullptr);


    const tokenize_result& tokenized_line = tokenize(line);
    return match_result::flatten_completions(
      option_root->find_matches(tokenized_line.next_token,
				tokenized_line.remaining_line));
  }

  parser::tokenize_result parser::tokenize(const std::string& line)
  {
    boost::tokenizer<> tokenizer(line);
    tokenize_result result;
    
    if(tokenizer.begin() != tokenizer.end())
    {
      result.next_token = *tokenizer.begin();
      result.remaining_line =
	boost::trim_left_copy(line).substr(result.next_token.size());
    }
    return result;
  }

} // end namespace treebeard
