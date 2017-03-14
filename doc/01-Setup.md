# Setup

![Step 1 of a Lego instruction booklet: a single Lego piece](i/lego-step-one.png)

Ahh, step 1. Don't you love a fresh start on a blank slate? And then selecting
that singular brick onto which you will build your entire palatial estate?

Unfortunately, when you're building a computer program, step 1 can get...
complicated. And frustrating. This tutorial contains about 100 steps divided
into 7 chapters, and step 1 takes up this entire first chapter.

Please don't give up during this chapter. If you can get through it, the rest
of the tutorial will be a breeze. Fortunately, the program we are building is
contained in a single C file and doesn't depend on any external libraries.

## The `main()` function

Create a new file named `kilo.c` and give it a `main()` function. (`kilo` is
the name of the text editor we are building.)

{{main}}

In C, you have to put all your executable code inside functions. The `main()`
function in C is special. It is the default starting point when you run your
program. When you `return` from the `main()` function, the program exits
and passes the returned integer back to the operating system. A return value of
`0` indicates success.

C is a compiled language. That means we need to run our program through a C
compiler to turn it into an executable file. We then run that executable like
we would run any other program on the command line.

To compile `kilo.c`, run `cc -o kilo kilo.c` in your shell. If no errors occur,
this will produce an executable named `kilo`.

To run `kilo`, type `./kilo` in your shell and press enter. The program doesn't
print any output, but you can check its exit status (the value we returned from
`main()`) by running `echo $?`, which should print `0`. (Try changing `kilo.c`
to return a different value, recompile, run `./kilo`, and run `echo $?` to
observe the change.)

After each step in this tutorial, you will want to recompile `kilo.c`, check if
it finds any errors in your code, and then run `./kilo`. It is easy for one to
forget to recompile, and just run `./kilo`, and wonder why your changes to
`kilo.c` don't seem to have any effect. You must recompile in order for changes
in `kilo.c` to be reflected in `kilo`.

## Makefile

You quickly get tired of typing `cc -o kilo kilo.c` every time you want to
recompile. The `make` program allows you to simply run `make` and it will
compile your program for you. You just have to supply a `Makefile` to tell it
how to compile your file.

{{makefile}}

The first line says that `kilo` is something we want to build, and that
`kilo.c` is required to build it. The second line specifies the command to run
in order to actually build `kilo` out of `kilo.c`. Make sure to indent the
second line with an actual tab character, and not with spaces. You can indent C
files however you want, but `Makefile`s must use tabs.

We have added a few things to the compilation command. `$(CC)` is a variable
that is expanded by `make` to _TODO_. `-Wall -Wextra -pedantic` are all flags
that tell the compiler to look for any bad usage of the C language and warn us
about it. `-std=c99` specifies the exact version of the C language we're using,
which is C99. _TODO_: why C99? what language features are we using?

Now that we have a `Makefile`, try running `make` to compile the program. It
may output ``make: `kilo' is up to date.``. If it sees that the `kilo` file was
last modified later than the `kilo.c` file was, then it assumes that `kilo` is
already built and doesn't do anything.

