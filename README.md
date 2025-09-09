# JCurl
J language routines involving the libcurl shared library

To compile the dynamic link library from the C source file I use the following on macOS:
gcc -dynamiclib -o libjcurl.dylib libjcurl.c -lcurl
