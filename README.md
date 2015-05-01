csvtools
========

Introduction
------------

  csvtools is simple, fast library for working with delimited text files.  It
provides command line tools for prerforming common tasks, such as subsetting, 
searching and cutting.

  The following tools are incuded:

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

Building
--------

  cd csvtools; make;

Examples
--------

  We will use the following example to demonstrate csvtools.

    $cat example.csv
    num,greeting,int
    1,"Hello.",1
    0,"Goodbye.",2
    0,"Hello, Dave.",3
    1,"Hello, \"Matt\", if that is your real name!",4

Note that the header may be included as the first line in the file when 
working with csvtools, and that files may include quoting characters.

Csvtools has utilities for: 

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
 
    $csv-diff example.csv example-sample.csv
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

Acknoledgements:

    Thanks to my employer, Allstate Insurance, for allowing me time to work on this, and the freedom to redistribute it.
    Thanks to Robert Olive and Sam Kim for thier valuable feedback.
