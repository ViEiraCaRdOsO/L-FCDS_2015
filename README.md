There's a useful Makefile in each of the tasks:

 * [make]: creates dependable folder and compiles program to executables

 * [make run_seq]: runs the sequential program
    * first prompted up in console to asks for input file path, then runs the program
    * after running, output/seq.out and output/time.out would be created
      * output/seq.out could be later used to compare if result is correct.
      * output/time.out records how much time it takes, and is later used to measure speedup(in ''%.4lf'' format)

 * [make final_scale]: runs, check answer and measures speedup of concurrent program on 0, ..., to #procs
    * first asks how many processor you want to use, then
     * ask for compare file path(like output/seq.out or specify somewhere), and last
     * ask for input file.
    * after each run, it automatically checks correctness and measures speedup
     * check correctness: checks if answer is equal to the compare file.
     * measure speedup:
       * depends on output/time.out, and automatically calculates speedup comparing to the first ''%.4lf'' in output/time.out
         * create output/time.out by running make run_seq first or by yourself
       * If you don't need it, just ignore it.
