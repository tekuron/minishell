# 42 minishell

![Language](https://img.shields.io/badge/Language-C-blue)
![Score](https://img.shields.io/badge/Score-100%2F100-brightgreen)

---

## Table of Contents
- [Overview](#overview)
- [Project Structure](#project-structure)
- [Key Features](#key-features)
- [Execution & File Descriptors](#execution--file-descriptors)
- [Parsing Strategy](#parsing-strategy)
- [Installation](#installation)
- [Usage](#usage)
- [Authors](#authors)

---

## Overview

**Minishell** is a project from the 42 curriculum that focuses on creating a minimal version of a Unix shell in C. Its primary goal is to replicate the core behavior of Bash, including command execution, process management, and the use of pipes and redirections.

This project focuses heavily on handling file descriptors, signal management, and robust command parsing to provide a seamless shell experience.

## Project Structure

Our project follows a solid and modular structure, separating logic into clear domains:
- **`src/parse/`**: Tokenizes and parses user input, correctly recognizing quotes (`'` and `"`), pipes, redirections, and environment variable expansions.
- **`src/exec/`**: Handles path resolution, child process forks, applying pipes/redirections, and launching the actual binaries or built-ins.
- **`src/builtins/`**: Contains our custom implementations of shell built-in commands (`echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`).
- **`src/env/`**: Manages the local copy of the environment variables.
- **`inc/`**: Contains the heavily structured headers defining types like `t_command`, `t_io`, `t_shell`, and enumeration for execution modes and redirections.
- **`libft/`**: Our custom C library built throughout the core 42 curriculum containing essential functions (like string manipulation, linked lists, printf, etc.).

## Key Features

* **Command execution:** Program finds binaries using the `$PATH` variable, or via exact absolute/relative paths using `execve`.
* **Redirections:** Handle input/output flows using `<`, `>`, `<<` (heredoc), `>>`.
* **Pipes:** Connect command outputs and inputs using `|`. The project manages multidirectional pipelines closing unnecessary file descriptors in parent and children.
* **Built-ins:** Fully functioning essential built-ins such as `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit`.
* **Environment variables:** Expansion of variables (`$VAR`) and exit status (`$?`).
* **Signal handling:** Accurate response to `Ctrl-C`, `Ctrl-D`, and `Ctrl-\`, dynamically changing according to whether the shell is in interactive mode, heredoc mode, or blocking execution mode.

## Execution & File Descriptors

The shell employs robust process creation mechanisms to run commands. Using our structural `t_process` and `t_command` structs, we iterate over the linked list of commands. It carefully manages parent-child processes using `pipe()` and `dup2()` to correctly capture the output, link pipes smoothly, and redirect file descriptors according to user inputs. 

Built-ins are either executed in the main loop (to affect the main shell process, like `cd` or `export`) or inside a fork if a pipe forces them to do so, replicating precisely how Bash behaves in edge cases.

## Parsing Strategy

To make sure the execution goes as expected, we parse the user's raw input string and store the final parameters and redirections inside a structured `t_command` linked list. The parsing handles:
- Properly dealing with single `\'` and double `\"` quotes using a custom tokenizer.
- Splitting by logical tokens like pipes (`|`) and generating input/output (`t_io`) items for redirections.
- Expanding environment variables where applicable.

## Installation

To compile the project, run the following command in the root directory:

```bash
make
```

To clean up object files:
```bash
make clean
```

To remove all compiled files including the executable:
```bash
make fclean
```

## Usage

Once compiled, you can start the shell by executing:

```bash
./minishell
```

You will be greeted with a prompt, and from there you can begin typing commands just as you would in standard bash.

## Authors
- **Daniel Zamora**
  - **GitHub:** [tekuron](https://github.com/tekuron)
  - **42 Intra:** [danzamor](https://profile-v3.intra.42.fr/users/danzamor)
- **Diego Plazas**
  - **GitHub:** [dplazas-g](https://github.com/dplazas-g)
  - **42 Intra:** [dplazas-](https://profile.intra.42.fr/users/dplazas-)
- **School:** 42