0x2A
====

A practical course subject gone wrong in the form of a stack based
two-dimensional esoteric programming language. It was inspired from brainfuck,
Befunge and PATH.

Description
-----------

Being a two-dimensional language, the code can be interpreted from left to right, from right to left, from up to bottom and bottom to up. The direction can be changed by a few operators with or without conditions.

As a stack-based language, arguments are passed to function by being on the stack and the arithmetic operators use the reverse polish notation (at least the code is interpreted from left to right). It is to be noted that the stack is a dynamic array of `int`.

Some operators are ignored when the code is not interpreted in the right direction like `[` or `_`. In those cases, no value is removed from the stack.

Operators
---------

+ Operator
    + Stack before
    + Stack after
    + Description
+ `a`
    + [...]
    + [... a]
    + Put the character *a* (its ASCII code) on the stack.
+ `A`
    + [...]
    + [... A]
    + Put the character *A* (its ASCII code) on the stack.
+ `0-9`
    + [...]
    + [... 0-9]
    + Put an integer between 0 and 9 (included) on the stack.
+ `+`
    + [... 2 3]
    + [... 5]
    + Add the two top integers of the stack and replace them by the result. In 0x2A, it is written as `23+`.
+ `-`
    + [... 2 3]
    + [... -1]
    + Subtract the two top integers of the stack and replace them by the result. In 0x2A, it is written as `23-`.
+ `>`
    + [...]
    + [...]
    + Interpret the code from left to right.
+ `<`
    + [...]
    + [...]
    + Interpret the code from right to left.
+ `v`
    + [...]
    + [...]
    + Interpret the code from up to bottom.
+ `^`
    + [...]
    + [...]
    + Interpret the code from bottom to up.
+ `\`
    + [...]
    + [...]
    + If the code is currently interpreted from left to right then it is now interpreted from up to bottom. If the code is currently interpreted from bottom to up then it is now interpreted from right to left. If the code is currently interpreted from right to left then it is now interpreted from up to bottom. If the code is currently interpreted from up to bottom then it is now interpreted from left to right.
+ `/`
    + [...]
    + [...]
    + If the code is currently interpreted from left to right then it is now interpreted from bottom to up. If the code is currently interpreted from bottom to up then it is now interpreted from left to right. If the code is currently interpreted from right to left then it is now interpreted from up to bottom. If the code is currently interpreted from up to bottom then it is now interpreted from right to left. 
+ `%`
    + [... n]
    + [... n n]
    + Duplicate the top value of the stack and place it on top.
+ `*`
    + [... n]
    + [...]
    + Remove the top value of the stack.
+ `~`
    + [...]
    + [...]
    + Ignore the next operator while taking into account the direction in which the code is currently interpreted.
+ `@`
    + [...]
    + [... n]
    + Ask the user to enter a character and put its ASCII code on top of the stack.
+ `=`
    + [...]
    + [... n]
    + Ask the user to enter an integer and put it on top of the stack.
+ `'`
    + [... a]
    + [...]
    + Display the top value of the stack as a character and remove it.
+ `.`
    + [... a]
    + [...]
    + Display the top value of the stack as an integer and remove it. `a.` would be displayed as `97`.
+ `!`
    + [... n]
    + [... 0-1]
    + If the top value is equal to 0 then it is replaced by 1 else it is replaced by 1.
+ `` ` ``
    + [... n m]
    + [... 0-1]
    + If the value n is strictly bigger than m then both are replaced by 1 else they are replaced by 0.
+ `|`
    + [... n]
    + [...]
    + If the code is currently interpreted from left to right and the top value of the stack is not equal to 0 then the code is now interpreted from right to left. If the code is currently interpreted from right to left and the top value of the stack is not equal to 0 then the code is now interpreted from left to right. If the code is currently interpreted from up to bottom or bottom to up then the operator is ignored.
+ `_`
    + [... n]
    + [...]
    + If the code is currently interpreted from up to bottom or from bottom to up and the top value of the stack is not equal to 0 then the direction is inverted. Else the operator is ignored.
+ `[`
    + [... n]
    + [...]
    + If the code is currently interpreted from left to right and the top value of the stack is equal to 0 then the instruction pointer is placed after the associated `]`. If the code is currently interpreted from right to left and the top value of the stack is not equal to 0 then the instruction pointer is placed before the associated `]`. If the code is currently interpreted from up to bottom or bottom to up then the operator is ignored.
+ `]`
    + [... n]
    + [...]
    + If the code is currently interpreted from left to right and the top value of the stack is not equal to 0 then the instruction pointer is placed after the associated `[`. If the code is currently interpreted from right to left and the top value of the stack is equal to 0 then the instruction pointer is placed before the associated `[`. If the code is currently interpreted from up to bottom or bottom to up then the operator is ignored.
+ `b-z`
    + [...]
    + [...]
    + Define an entry point for a function.
+ `B-Z`
    + [...]
    + [...]
    + Call the function b-z. The entry point for the function is searched after the operator if the code is currently interpreted from left to right or from up to bottom or before the operator if the code is currently interpreted from right to left or from bottom to up.
+ `#`
    + [...]
    + [...]
    + Return from the program or the function.
