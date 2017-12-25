# CircuitLang Runtime Documentation
## Welcome to the CircuitLang Runtime Documentation!

This documentation contains information on how this runtime is built, how it can be improved, and resources/guides on how to use this. As of the first version of this runtime, well, it is the first time I'm writing my own programming language, forget a runtime for a language. It's practically guaranteed to have bugs somewhere or the other. If it doesn't, holy crap!

## Runtime Design

### Compile

This runtime is designed to execute a program as compile-and-run. It begins by cleaning up your source code by removing comments and formatting, and placing exactly one expression per line of code. The runtime then separates all the instructions by their function, creating a list of instructions for each function. Each of the instructions are then replaced with an "opcode". An opcode is a static C function, which takes an argument of void*. Even function calls are replaced with a function with this fashion. Each "opcode" is given the required information to work. A new vector is created, which holds all the opcodes contiguously. A hashmap is generated while completing this, which holds the function name as a string, as the key. Its value is an integer offset in the vector, which corresponds to the first instruction for the function. After this hashmap is completed, the opcode vector is iterated, and all function calls are assigned with their target jump point.

While this is a very heavy compile process, compared to most interpreted languages, once the program is running, it should execute quickly! To execute, create a loop which begins executing the vector of function pointers (opcode vector). When it reached the terminate opcode, it returns to the function dispatcher.

### Function Dispatcher

CircuitLang dictates that all functions that are called in a function shall execute simultaneously. Of course, that's not necessarily going to be the case, as the function dispatcher could be written single threaded, as is in this prototype. Or, the system the program is being run on may not have enough cores.

In order to execute functions in the right order, a function dispatcher is used. This is a very simple function, which contains a queue. The queue is initialized with the function, `/begin`, the entry point of the program. The job of the function dispatcher is to call functions, remove them from the queue, and repeat until the queue is empty, which indicates that the program is over. Suppose `/begin` calls three functions. Each of these functions will be added to the function dispatcher's execution queue, along with the data and resources required to call them (their context).

### Next Steps

After this runtime is complete, there are a few features that would make this language much more viable and powerful. These include an I/O loop and a debugger. The I/O loop would be a background loop, which is activated whenever an I/O interface is created, whether it be a socket or a pipe, whatever. This loop would use `select`, or an alternative if in a different platform, to multiplex all of the I/O into a single thread. When a CircuitLang function calls an I/O function, instead of the function being sent to the execution queue, it will be stored for future execution. Once data is received, the I/O thread will unstore the function and add it to the execution queue. If one or more functions are stored, waiting for I/O, a flag will be set to force the function dispatcher to not exit once the queue has been completely emptied. Instead, the function dispatcher will wait for the I/O thread to signal that a function is ready for execution. This will likely be done using mutexes.
