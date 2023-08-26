# ASDC (Algoritmi, Structuri de Date, Complexitate)

This repository contains assignments, implemented as part of the course **Algoritme, Structuri de Date, Complexitate** (**Algorithms, Data Structures, Complexity**).

The project includes 4 assignments in one. For explanation documents, see the markdown files in the `doc` folder. Assignments are all in *Romanian*.

> The markdown files use [tex math formulas, which won't show on github](https://stackoverflow.com/questions/11256433/how-to-show-math-equations-in-general-githubs-markdownnot-githubs-blog). To see formulas, you will either have to convert markdown to html or pdf, with [`MathJax`](https://www.mathjax.org/) enabled, or find the compiled pdf's [on my google drive](https://drive.google.com/drive/folders/1Rs0-qy6ivSDuHh5JadrP4Ta4YDhuVRiC).

All test code is dumped into `src/main.cpp`.

PR's with grammar corrections, bug fixes, improvement suggestions or translations are very welcome.

Leave a star as a way to say "Thank you". Enjoy!

## Approximate build instructions

1. `git clone --recursive https://github.com/AntonC9018/uni-cnmo`. `--recursive` is needed to load submodules.
2. Install GNU C++ compiler, which can be done by installing `MinGW` or `Cygwin`, and adding their `bin` folder in `PATH` environent variable. Alternatively, you should be able to easily compile it with `MSVC`, see `build.bat` for the appropriate includes and compiler flags.
3. Running `build` should build the executable. Then, execute that executable.
