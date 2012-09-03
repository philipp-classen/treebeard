treebeard
=========

* C++ Wrapper for Editline (readline)
* Supports context dependent command expansion
  (e.g., set variable o<TAB>{on,off})

Still in early alpha phase.

Known issues
------------

* Arch Linux: `rl_line_buffer` returns only the previous line,
  therefore the completion is broken.
  When linking to readline, there is no problem, though.
