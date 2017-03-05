/* # The `main()` function
 *
 * Create a new file named `kilo.c` and type the following code into it. Do not
 * copy and paste it. Type it in.
 */

int main() {
  return 0;
}

/* The `main()` function in C is special. It is where the program will start
 * executing. When the function returns, the program exits. Returning `0` sets
 * the program's exit status to `0`, which indicates success.
 *
 * # Compiling and running
 *
 * C programs must be compiled into executables. A compiler will turn our
 * `kilo.c` file into a `kilo` or `kilo.exe` executable, which we then run like
 * any other program on the command line.
 *
 * We'll use a simple `Makefile` to compile the program. Create a new file
 * named `Makefile` in the same directory as `kilo.c` and type the following
 * code into it. Make sure that the second line is indented with a single tab
 * character, rather than multiple spaces.
 *
 *     kilo: kilo.c
 *     	$(CC) -o kilo kilo.c -Wall -W -pedantic -std=c99
 *
 * Now you should be able to compile `kilo.c` by running `make` at the command
 * line. Then you should be able to execute the program by running `./kilo`.
 * After that, you can try running `echo $?` to observe the program's exit
 * status, which was `0`.
 *
 *     $ make
 *     $ ./kilo
 *     $ echo $?
 *     0
 *
 * Try changing `return 0` in the program to return a different number. Make
 * sure to recompile the program by running `make`, then run the program and
 * check its exit status. Then change it back to `0`, recompile again, and
 * confirm that the program now returns `0`.
 *
 * Making a change to the code, recompiling with `make`, and running the
 * program to observe the changes you made is what you'll be doing over and
 * over again throughout this tutorial. It can be easy to forget to run `make`,
 * in which case when you run `./kilo` it won't incorporate any of the changes
 * you made since the last compile, which can be confusing. So keep that in
 * mind.
 */

