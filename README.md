# corpus-filter
Simple utility to filter out text corpus according to frequencies of words consisting sentences in it

# detail
There are files stored in a parallel corpus in following format: https://object.pouta.csc.fi/OPUS-CCMatrix/v1/moses/en-pl.txt.zip (ingnore idx file only PL and EN matters).
This is just plain text in UTF-8.

Those 2 files are parallel at the level of line numbers. So the same line has corresponding translations between 2 files.

This utility works in Ubuntu and will do the followings:
- Take as input 2 files *.pl and *.en, and a number e.g. 80
- Make a dictionary from *.pl file and count how frequent each word was after tokenization.
- If input number was 80 - remove all lines that do not consist of 100% of 80% most frequent words.
  (e.g. if there is at least one word from remaining 20%, remove that line in both files)
- Save remaining lines in the same format (2 files) UTF-8 - remaining those 2 files parallel.
