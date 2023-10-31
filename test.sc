# This is a comment, comments will be ignored by the interpreter

# Error Handling: file doesn't exist
use ./non_existent.so 

# Error Handling: file is not a shared library
use ./not_a_lib.txt

# Error Handling: Syntax Error
user ./lib1.s
callo funcA

# Error Handling: No library is loaded
call funcB

# Error Handling: Function does not exist
use ./lib1.so
call nonExistentFunc

use ./lib1.so
call funcA
call funcB
use ./lib2.so
call funcX
call funcY
