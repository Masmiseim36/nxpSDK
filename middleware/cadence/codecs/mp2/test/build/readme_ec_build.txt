This "readme" file describes building and executing error concealment
post processing using the test bench. For test bench usage other than
invoking the post processing, refer to Chapter 4 of Ref 1

Optionally, the mp2 and mp3 test bench can test error concealment post
processing.  Error concealment post processing is released as a
seperate package.

Assume the root directory for the error concealment is located at
ROOT_EC_DIRECTORY

-------------------------------------------
Source code location
-------------------------------------------
In addition to the test bench code described in Chapter 4 of Ref 1, 
the error handler source code for error concealment can be found at 
ROOT_EC_DIRECTORY/test/src/xa_error_conceal_error_handler.c

-------------------------------------------
Build the test bench with error concealment 
-------------------------------------------
1. Go to ./build
2. > To build mp2 with error concealment, type:
   make clean mp2ec INCLUDE_EC=1 ECDIR=ROOT_EC_DIRECTORY

   > To build mp3 with error concealment, type:
   
   make clean mp3ec INCLUDE_EC=1 ECDIR=ROOT_EC_DIRECTORY

This will create a decoder example application with error concealment. 

-------------------------------------------
Running the application
-------------------------------------------
Refer to Ref 1



References:
-----------
1. MP3 Programmer's Guide
