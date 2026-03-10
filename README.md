# 🖥️ Mini Shell (Custom Linux Command Interpreter)

## 📌 Overview

The **Mini Shell** is a simplified implementation of a Unix/Linux command-line shell written in **C using system calls**.
It mimics the behavior of standard shells like **bash** by reading user commands, parsing them, and executing them through process management and system-level APIs.

The project demonstrates key **operating system concepts** such as **process creation, inter-process communication, signal handling, and command execution**.

---

# 🚀 Key Features

| Capability           | Description                                                      |
| -------------------- | ---------------------------------------------------------------- |
| ⚡ Command Execution  | Executes external Linux commands using `fork()` and `execvp()`   |
| 🔧 Built-in Commands | Supports internal commands like `cd`, `pwd`, `echo`, and `exit`  |
| 🔗 Pipe Support      | Handles command pipelines such as `ls \| wc` and `grep .c \| wc` |
| 🧠 Command Parsing   | Tokenizes user input and separates commands/arguments            |
| ⚙️ Signal Handling   | Handles `Ctrl+C` and `Ctrl+Z` using custom signal handlers       |
| 🎛 Custom Prompt     | Allows dynamic prompt modification using `PS1`                   |

---

# 🧠 How It Works

The shell operates in a **continuous loop**:

1. Display a prompt (`minishell$`)
2. Read user input
3. Parse the command and arguments
4. Determine command type:

   * **Built-in command**
   * **External command**
5. Execute using:

   * `fork()` to create a child process
   * `execvp()` to run the command
6. Handle pipes if present
7. Wait for process completion

---

## Example Execution Flow

Example command:

```
ls | wc
```

Process structure:

```
Parent Shell
      │
      ├── Child 1 → ls
      │              │
      │              ▼
      │            PIPE
      │              │
      └── Child 2 → wc
```

Output of `ls` is redirected through a **pipe** into `wc`.

---

# 📂 Project Structure

```
📁 MiniShell
│
├── main.c
├── scan_input.c
├── commands.c
├── signal.c
│
├── include/
│   └── header.h
│
├── ext_cmd.txt
│
└── README.md
```

---

# ▶️ Usage Example

```
minishell$ pwd
/home/user/projects

minishell$ ls | wc
      8      8     72

minishell$ echo $$
3245

minishell$ cd Desktop

minishell$ exit
```

---

# 🛠️ Compilation & Run

Compile the project:

```
gcc *.c -o minishell
```

Run the shell:

```
./minishell
```

---

# 📈 Future Enhancements

| Feature             | Description                             |
| ------------------- | --------------------------------------- |
| 🔄 I/O Redirection  | Support `<`, `>`, and `>>` operators    |
| ⚙️ Background Jobs  | Execute commands using `&`              |
| 🧠 Command History  | Store and recall previous commands      |
| 📁 Auto PATH Lookup | Automatically detect external commands  |
| 🎨 Advanced Prompt  | Display username, directory, and colors |

---

# 🤝 Contributions

Contributions, suggestions, and improvements are welcome.
You can **fork the repository**, create enhancements, and submit a **pull request**.

---

# 📜 License

This project is released under the **MIT License** and is free to use and modify.

-
If you want, I can also give you a **much stronger GitHub README version that looks like a real production project (with diagrams + badges)**. It will look **10× more professional**.
