//
// Currently, the whole example makes no sense.
// This is OK, as it just serves as a sandbox to improve the syntax.
//

#include <iostream>
#include <treebeard.h>
#include <initializer_list>
#include <utility>

using treebeard::option_tree;
using treebeard::one_of;
using treebeard::concat;
using treebeard::file;

std::unique_ptr<option_tree> create_options_regression()
{
  return one_of("help", "quit",
		one_of("version", "print"));
}

std::unique_ptr<option_tree> create_options()
{
  return one_of("help",
		"quit",
		concat("echo", one_of("on",
				      "off")
		       ),
		concat("cat", file(".pdf")),
		concat("show", concat("pdf", file(".pdf")))
		);
}

int main()
{
  const bool wait_for_gdb_session = true;
  if(wait_for_gdb_session)
  {
    std::cout << "Hit return when you have attached the GDB session..."
	      << std::endl;
    std::string dummy;
    std::getline(std::cin, dummy);
  }

#ifdef TREEBEARD_HAVE_EDITLINE
  typedef treebeard::editline_input input_impl;
#else
  typedef treebeard::raw_input input_impl;
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
    catch(treebeard::end_of_line& e)
    {
      std::cout << "Bye.\n";
      return 0;
    }
  }
}
