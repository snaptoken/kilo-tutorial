This repo contains the entire source code for each step of
[kilo-tutorial](https://github.com/snaptoken/kilo-tutorial). Each step is
represented by a commit. Each step name (found in the upper-right corner of
each step diff in the [tutorial](http://viewsourcecode.org/snaptoken/kilo)) is
a ref to that step's commit.

If you want to compare your version of `kilo.c` with the version in this repo
for a particular step, say `keypresses`, you could do it like this:

    $ git clone https://github.com/snaptoken/kilo-src
    $ cd kilo-src
    $ git checkout keypresses
    $ git diff --no-index -b ../path/to/your/kilo.c kilo.c

`--no-index` lets you use `git diff` as an ordinary diff tool, and `-b` ignores
differences in whitespace, which is important if you use a different indent
style than the one in the tutorial.

