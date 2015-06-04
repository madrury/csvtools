csvtools
========

Introduction
------------

  csvtools is simple, fast library for working with delimited text files.  It
provides command line tools for preforming common tasks, such as subsetting, 
searching and cutting.

  The following tools are included:

  - csv-chdelim: Change the delimiter in a text file.
  - csv-cut: Output only a requested subset of columns from a delimited
    file.
  - csv-grep: Output the lines in a delimited file where a specific 
    column matches a pattern.
  - csv-sample: Randomly select a given proportion of fields from a 
    delimited file.
  - csv-set: Set operations on csv, take unions, intersections and
    differences of lines.

  csvtools is written in a combination of C and bash to be as fast as possible.

Why csvtools?
------------

  csvtools was built by the author at Allstate Insurance to process massive csv
files used in auto insurance predictive modeling projects.  It was designed to
be smaller and more agile than other tools and to be useful in sophisticated
command line and hadoop map-reduce workflows.

Is It Fast?
-----------

  It is really quite fast.  Comapred to csvkit usuing an 80M pipe delimited
file:

Cutting is over 20 times faster:

    # csvkit
    $ time csvcut -d '|' -c 'DOLLAR' random.csv > /dev/null
    real    0m12.873
    #csvtools
    $$ time csv-cut -f random.csv -d '|' 'DOLLAR' > /dev/null
    real    0m0.589s

Searching is also over 20 times faster:

     # csvkit
     $ time csvgrep -d '|' -c 'ADDR' -r '.*Rd\..*' random.csv > /dev/null
     real    0m13.721s
     # csvtools
     $ time csv-grep -f random.csv -d '|' -p "Rd\\." -c ADDR > /dev/null
     real    0m0.548s

Building
--------

  cd csvtools; make;

Examples
--------

  We will use the following example delimeited file to demonstrate csvtools.

    $cat example.csv
    num,greeting,int
    1,"Hello.",1
    0,"Goodbye.",2
    0,"Hello, Dave.",3
    1,"Hello, \"Matt\", if that is your real name!",4

  Note that the header may be included as the first line in the file when
working with csvtools or supplied as a seperate file with the `-r` options.
csvtools is aware of quoting, see the `-q` argument to many of the tools.
csvtools is also aware of alternate delimiters, see the `-d` arhument to many of the
tools.  All of the tools has detailed help available with `-h`.

csvtools has utilities for: 

Changing the delimiter:

    $csv-chdelim -f example.csv -d , -n "|" 
    num|greeting|int
    1|"Hello."|1
    0|"Goodbye."|2
    0|"Hello, Dave."|3
    1|"Hello, \"Matt\", if that is your real name!"|4

Cutting out columns:

    $csv-cut -f example.csv num int
    num,int
    1,1
    0,2
    0,3
    1,4

    $csv-cut -f example.csv greeting
    greeting
    "Hello."
    "Goodbye."
    "Hello, Dave."
    "Hello, \"Matt\", if that is your real name!"

Searching for patterns:

    $csv-grep -f example.csv -c num -p 0
    num,greeting,int
    0,"Goodbye.",2
    0,"Hello, Dave.",3

    $csv-grep -f example.csv -c greeting -p "Dave\|Matt"
    num,greeting,int
    0,"Hello, Dave.",3
    1,"Hello, \"Matt\", if that is your real name!",4

Random sampling:

    $csv-sample -f example.csv -p .5
    num,greeting,int
    0,"Goodbye.",2
    0,"Hello, Dave.",3

Differences:
 
    $csv-set -d example.csv example-sample.csv
    num,greeting,int
    1,"Hello.",1
    1,"Hello, \"Matt\", if that is your real name!",4

Frequency tables:

    $csv-hist -f example.csv num int
    1 0,2
    1 0,3
    1 1,1
    1 1,4


  Any of the tools can take data from standard input:

    $ cat example.csv | csv-cut num int
    num,int
    1,1
    0,2
    0,3
    1,4

which allows tools to be chained together:

    $ cat example.csv | csv-cut num int | csv-grep -c num -p 0
    0,2
    0,3

Acknowledgements:
-----------------

Thanks to my employer, Allstate Insurance, for allowing me time to work on this, and the freedom to redistribute it.

Thanks to Robert Olive and Sam Kim for thier valuable feedback.
