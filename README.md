# Classwork

 ## Operating Systems

  ### Compiling the code
   You can do the following with the makefiles.

   * Run `make` in the root folder to compile everything and put the output in an `Output` folder.
   * Run `make` in the `CLibExtensions` folder or the `Scheduler` folder to build what is in the respective folders.
   * Run `make clean` to delete all intermediate and binary files generated from running make.
   * Run `make PRINT_PROCESSES_DATA=true` from the root folder or the `Scheduler` folder to make the Scheduler print the data it   reads   in before executing the rest of main.

  ### Running Scheduler
   Make sure that `processes.in` is inside the `Output` folder.
   
   `cd` into the `Output` folder and run `./Scheduler processes.in`.  The output will be generated in the same folder as `processes.out`.

  ### Running BufferDriver
   run `sudo insmod BufferDriverWriter.ko` and `sudo insmod BufferDriverReader.ko` to get the drivers into the kernel.  Run `dmesg` to read the messages.  Run `sudo -i` to stay in admin mode.  Run `echo "yourString" > /dev/bdwriter` to read to the buffer.  Run `cat /dev/bdreader` to write to the buffer.  Run `sudo head --bytes=10 /dev/bdreader` to read 10 bytes from the buffer.

  ### Using CLib Extensions
   There are a few additional data structures that can be used for this project (e.g. stacks and vectors).  See their header files in the `CLibExtensions` folder for how to utilize them.
