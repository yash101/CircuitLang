# CircuitLang Language Documentation

## Parsing CircuitLang Code

### Introduction

This documentation is to serve as a guide on how CircuitLang code is structured, and how to properly parse the code. This documentation explains how to properly support different types of code styles properly:

### Shebang

Shebangs will be ignored, as they begin with a hash (#). Hashes are a type of comment, as decribed below in the "Comments" section. Below are examples of shebangs.
```
#!/usr/local/bin/clg
```

### Comments

There are two types of comments in CircuitLang. There is an end-of-line comment, which starts with a hash (#). Any text after a hash, but before the newline will be ignored. A hash found within a string or character specifier will be considered part of the string or character specifier, and will not behave as an end-of-line comment. There is also a multi-line block comment, which begins and ends with a double hash (##). Any text found between both double hashes will be ignored. Block comments may NOT be nested. Below are examples of comments.

```
# Single line comment

## Multi
   Line
   Comment ##
```

### Function Declarations

A function name will be preceded with a forward slash in a function definition. Forward declarations don't exist in CircuitLang, because they are unnecessary. If forward declarations shall be prefered to keep the code more legible and maintanable, a standardized format may be described within a comment: `# /<funcname> (param1, ..., paramn)`. The function name will be read until reaching an opening parenthesis ((). The function name will be padded to remove any whitespace, and the forward slash will be removed.

Now, the parameter list for the function shall be extracted. The parser must scan forward until a closing parenthesis ()) is found. All parenthesis shall be removed from the parameter list, and the list shall be split by a comma (it is in CSV format). Each of these parameters shall be padded for whitespace. If any parameter has a length of zero (just a comma, no parameter), a syntax error shall be thrown. Each of these parameters shall be split by whitespace. The first word will be the type of the paramter. The second will be the name of the parameter. Below is example(s) of function declarations.

```
/func1 (string a, int b)

/func2 ()

/func3
       (string a
        string b
        char c)
```

### Structures

TODO

### Function Bodies

A function body will consist of numerous statements, separated by semicolons - every statement whithin a function other than exceptional control structures must end with a semicolon. Function calls shall be preceded with a greater than arrow (>). For the most part, whitespace is ignored/optional. Every function is implicitly defined do have a return call at the end. When the function "returns", control is given back to the function dispatcher. This allows the next function(s) to execute. Below are examples of different statements within function bodies.

```
/func (string a, string b)
	string c = a + b;
	int a = 1;
	int b = 2;
	int c = a + b;

	>func1(a, b);
	> func2(b, c);
```

### Calling Functions

To call a function from a function, there is a special syntax. The function name is preceded with a greater than char (>). After the function name is a CSV of the parameters, enclosed within two parenthesis ( and ). 

### Branching

Decisions must be made in a programming language for it to be of any use in solving more than a very simple problem. CircuitLang only supports branching in the form of if[-elseif][-else] structures. Certain operations return a boolean value. A boolean value is an integer. A zero (0) means false, and nonzero means true. Some operations that generate booleans are comparisons (<, <=, !=, ==, >=, >). For the boolean operations with two characters (<=, !=, ==, >=), the order of the characters can be inverted just like in C. This means that (=<, =!, ==, =>) will all be valid boolean comparisons. Also exist are operations that operate on booleans: and, or, which are represented by && or ||, respectively.

If-elseif-else statements can be written as such:

```
/func3 (int a, int b)
	if (a < b) {

	} elseif(b == a) {

	} else {

	}
```

The parameter for if/elseif is contained within parenthesis after if/elseif. The parser must then scan forward until the next non-whitespace character. If it is an opening curly brace, the statements until the closing curly brace will be considered part of the branch. Otherwise, the next statement will be considered part of the branch. The same rules apply for else, but else doesn't have a parameter.
