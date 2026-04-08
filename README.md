*This project has been created as part of the 42 curriculum by danzamor, dplazas-.*

## Description
Minishell is a minimal version of a Unix shell developed in C. Its primary goal is to replicate the core behavior of Bash, including command execution, process management, and the use of pipes and redirections. This project focuses on handling file descriptors, signal management, and robust command parsing.

Key features:
* Command execution through $PATH or absolute/relative paths.
* Redirections: '<', '>', '<<', '>>'.
* Pipes: '|' to connect command outputs and inputs.
* Built-ins: echo, cd, pwd, export, unset, env, exit.
* Environment variable expansion ($VAR).
* Signal handling (Ctrl-C, Ctrl-D, Ctrl-\\).

## Instructions
### Compilation
To compile the project, use the provided Makefile:
```bash
make
```

### Execution
Once compiled, start the shell with:
```bash
./minishell
```

### Cleaning
To remove object files:
```bash
make clean
```

To remove all generated files including the executable:
```bash
make fclean
```

## Resources
* [Bash Reference Manual](https://www.gnu.org/software/bash/manual/) - Official documentation for shell behavior.
* [GNU Readline Library](https://tiswww.case.edu/php/chet/readline/rltop.html) - Documentation for command line editing.
* [GeeksForGeeks](https://www.geeksforgeeks.org/c/signals-c-language/) - Signal handling documentation.
* [StackOverflow](https://stackoverflow.com/questions) - Specific doubts.

### AI Usage
AI was utilized for the following tasks:
* **Conceptual Clarification:** Clarifying bash concepts and insights.
* **Internal bash behaviour** Explaining bash behaviour for parsing and action order.