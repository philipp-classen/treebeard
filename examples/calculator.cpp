#include <iostream>
#include <treebeard.h>
#include <initializer_list>
#include <utility>

using namespace treebeard;

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

std::unique_ptr<option_tree> make_option_tree(
  std::unique_ptr<option_tree>&& value)
{
  return std::move(value);
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

std::unique_ptr<option_leaf> make_option_leaf(
  std::unique_ptr<option_leaf>&& value)
{
  return std::move(value);
}

// ----------------------------------------------------------------------

void one_of__construction_helper__(
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

template <typename... Args>
std::unique_ptr<option_tree> one_of(Args... options)
{
  std::vector<std::unique_ptr<option_tree>> converted_option_trees;
  one_of__construction_helper__(converted_option_trees,
				std::forward<Args>(options)...);
  return std::unique_ptr<option_tree>(
    new multiple_option_tree(std::move(converted_option_trees)));
}

template <typename Arg1, typename Arg2>
std::unique_ptr<option_tree> concat(Arg1 predecessor, Arg2 successor)
{
  return std::unique_ptr<option_tree>(new consecutive_option_tree(
    std::move(make_option_leaf(std::forward<Arg1>(predecessor))),
    std::move(make_option_tree(std::forward<Arg2>(successor)))));
}

// ----------------------------------------------------------------------

std::unique_ptr<option_tree> create_options_regression()
{
  return one_of("help", "quit",
		one_of("version", "print"));
}

std::unique_ptr<option_tree> create_options()
{
  return one_of("help", "quit",
		concat("echo", one_of("on", "off")));
}

int main()
{
  const bool wait_for_gdb_session = false;
  if(wait_for_gdb_session)
  {
    std::cout << "Hit return when you have attached the GDB session..."
	      << std::endl;
    std::string dummy;
    std::getline(std::cin, dummy);
  }

#ifdef TREEBEARD_HAVE_EDITLINE
  typedef editline_input input_impl;
#else
  typedef raw_input input_impl;
#endif

  input_impl in(create_options());
  in.set_prompt("$ ");

  std::cout << "Calculator started:\n";
  for(;;)
  {
    try
    {
      const std::string& next_line = in.getline();
      std::cout << "You entered: " << next_line << '\n';
    }
    catch(end_of_line& e)
    {
      std::cout << "Bye.\n";
      return 0;
    }
  }
}
