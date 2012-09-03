#include <iostream>

#include "treebeard/option_tree.h"
#include "treebeard/parser.h"
#include "treebeard/input.h"

#ifdef TREEBEARD_HAVE_EDITLINE
  #include <editline/readline.h>
  //#include <editline/history.h>

  #include <boost/algorithm/string/predicate.hpp>
  #include <cstring>
  #include <array>

namespace
{
  /*!
   * \throw treebeard::illegal_state_error
   */
  void init_editline(treebeard::editline_input* libedit_input_instance_);

  /*!
   * \throw treebeard::illegal_state_error
   */
  void cleanup_editline();

  /*!
   * \throw treebeard::illegal_state_error
   */
  void set_editline_program_name(std::string program_name_);

  char** treebeard_completion_callback(const char* text, int start, int end);
  char* treebeard_option_generator_callback(const char* text, int state);

  // used to implement the callback
  treebeard::editline_input* libedit_input_instance = nullptr;
  
  // --- implementation ---

  // must only be called once!
  void init_editline(treebeard::editline_input* libedit_input_instance_)
  {
    if(libedit_input_instance != nullptr)
    {
      throw treebeard::illegal_state_error(
        "Only one instance of editline_input is allowed");
    }

    libedit_input_instance = libedit_input_instance_;
    TREEBEARD_ASSERT(libedit_input_instance != nullptr);

    set_editline_program_name(
      libedit_input_instance->get_program_name_for_global_readline_config());
    rl_attempted_completion_function = treebeard_completion_callback;
  }

  void cleanup_editline()
  {
    libedit_input_instance = nullptr;
  }

  void set_editline_program_name(std::string program_name_)
  {
    static std::string program_name;
    program_name = std::move(program_name_);
    rl_readline_name = const_cast<char*>(program_name.c_str());
  }

  char** treebeard_completion_callback(const char* text, int start, int end)
  {
    return completion_matches(text, treebeard_option_generator_callback);
  }

  char* treebeard_option_generator_callback(const char* text, int state)
  {
    static std::vector<std::string> options;
    static size_t pos = 0;

    if(state == 0 /*RL_STATE_NONE*/)
    {
      // state resetted -> begin with the first match
      pos = 0;
      std::string complete_line_to_cursor(rl_line_buffer, rl_point);
      options = libedit_input_instance->find_all_matches(
        std::move(complete_line_to_cursor));
    }

    while(pos < options.size())
    {
      const std::string& next_option = options[pos++];
      if(boost::starts_with(next_option, text))
	return strdup(next_option.c_str()); // will be freed by editline
    }
    return nullptr;
  }

}
#endif // TREEBEARD_HAVE_EDITLINE

namespace treebeard
{

#ifdef TREEBEARD_HAVE_EDITLINE
  editline_input::editline_input(std::unique_ptr<option_tree>&& options_) :
    options(std::move(options_)),
    initialized(false)
  {
    TREEBEARD_ASSERT(options  != nullptr);
    TREEBEARD_ASSERT(options_ == nullptr);

    // FIXME: Only for debugging
    std::cout << "Describe active options:\n" << options->describe(0) << '\n';
  }

  editline_input::~editline_input()
  {
    if(initialized)
      cleanup_editline();
  }

  void editline_input::set_prompt(std::string prompt_)
  {
    prompt = std::move(prompt_);
  }

  void editline_input::set_program_name_for_global_readline_config(
    std::string program_name_)
  {
    program_name = std::move(program_name_);
    if(initialized)
      set_editline_program_name(program_name);
  }

  const std::string&
  editline_input::get_program_name_for_global_readline_config() const
  {
    return program_name;
  }

  std::string editline_input::getline()
  {
    if(!initialized)
    {
      init_editline(this);
      initialized = true;
    }

    const char* next_line = readline(prompt.c_str());
    if(next_line != nullptr)
      return next_line;
    else
      throw end_of_line();
  }

  std::vector<std::string> editline_input::find_all_matches(
    const std::string& text) const
  {
    TREEBEARD_ASSERT(options != nullptr);
    parser parser(options.get());
    return parser.find_all_matches(text);
  }

#endif // TREEBEARD_HAVE_EDITLINE

  // ----------------------------------------------------------------------

  std::string raw_input::getline()
  {
    std::cout << prompt;

    std::string next_line;
    if(std::getline(std::cin, next_line))
      return next_line;
    else
      throw end_of_line();
  }

} // end namespace treebeard
