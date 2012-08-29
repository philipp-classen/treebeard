#ifndef HAVE_TREEBEARD_LIB_INPUT_H
#define HAVE_TREEBEARD_LIB_INPUT_H

#include <memory>
#include <string>
#include <vector>

#include "treebeard/common.h"
#include "treebeard/exceptions.h"

namespace treebeard
{
  class option_tree; // forward

#ifdef TREEBEARD_HAVE_EDITLINE

  // can only be used as a singleton
  // (as editline uses global variables)
  class editline_input
  {
  public:
    /*!
     * \throw initialization_error
     */
    editline_input(std::unique_ptr<option_tree>&& options_);

    ~editline_input();

    void set_prompt(std::string prompt_);

    void set_program_name_for_global_readline_config(
      std::string program_name_);

    const std::string& get_program_name_for_global_readline_config() const;

    /*!
     * \throw end_of_line
     */
    std::string getline();

    std::vector<std::string> find_all_matches(const std::string& text) const;

  private:

    std::unique_ptr<option_tree> options;

    bool initialized;

    // default: no prompt (empty string)
    std::string prompt;

    // default: disabled (empty string)
    std::string program_name;
  };

#endif // TREEBEARD_HAVE_EDITLINE

  class raw_input
  {
  public:
    raw_input(std::unique_ptr<option_tree> options_) :
      options(std::move(options_))
    {
      TREEBEARD_ASSERT(options != nullptr);
    }

    void set_prompt(std::string prompt_)
    {
      prompt = std::move(prompt_);
    }

    void set_program_name_for_global_readline_config(const std::string&)
    {
      // ignored (raw_input cannot be configured like readline)
    }

    /*!
     * \throw end_of_line
     */
    std::string getline();

  private:
    std::unique_ptr<option_tree> options;

    // default: no prompt
    std::string prompt;
  };

} // end namespace treebeard

#endif // inclusion guard
