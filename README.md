# Multithreaded Pomodoro Timer & Logger

A desktop productivity application written in C++ that features a multithreaded timer, user authentication, and statistics tracking.

## Features
* **Multithreaded Core:** The timer logic runs on a dedicated background thread, ensuring the UI remains responsive with thread-safe updates.
* **Database Integration:** Utilizes SQLite3 for user login, authentication, and persistent logging of completed work sessions.
* **GUI:** Clean desktop interface built with the wxWidgets framework.
* **Unit Testing:** Integrated testing using the Catch2 framework.

## Tech Stack
* C++ 
* wxWidgets
* SQLite3
* CMake & Catch2

---

## Prerequisites
To build and run this project, you will need:
* A C++ compiler (GCC/Clang)
* CMake (optional, but recommended)
* Make (or your preferred build system)
* wxWidgets Library
* sqlite3 Library
* Catch2 Library (for testing)

### Installation (macOS via Homebrew)
```bash
brew update
brew install sqlite cmake wxwidgets
