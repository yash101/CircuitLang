# CircuitLang

CircuitLang is a random project I'm working on. It's a whole new programming paradigm (I think)

The main paradigm of this language is that when a function is called, all of its code executes in parallel. Also, functions cannot return to their callee, so this mitigates the requirement for a stack, allows infinite recursion (thats how you implement things such as loops). I guess this description is quite confusing. Let's show some code and analyze it

```
# Comments begin with a hash
## Block comments begin/end with a double hash ##

# A function is defined by a forward slash followed by two parenthesis, containing a comma separated arguments list.
/begin () {
	# To make programming in this language practical, some operations take place iteratively; these are strictly defined before program execution.
	let Int a = 2;
	let Int b = 4;

	# These are actual function calls; each of these functions will execute together
	+ subtract (a, b)
	+ add (a, b)

	# In a perfect world, this would be identical to the setup at the top
	+ add(a, b)
	+ subtract (a, b)
}

/subtract (a, b) {
	- print(a - b)
	+ add (a, b)
}

/add (a, b) {
	- print(a + b)
}

```
