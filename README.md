# Design Patterns in C++

This repository is part of the [Refactoring.Guru](https://refactoring.guru/design-patterns) project.

It contains C++ examples for all classic GoF design patterns. Each pattern includes two examples:

- [x] **Conceptual** examples show the internal structure of patterns with detailed comments.
- [ ] **RealWold** examples show how the patterns can be used in a real-world C++ application.


## Requirements

The examples were written as cross platform console application using c++11. It means that you should be able to compile and execute those examples with any recent compiler.

we recommend working with Visual Studio Code because it is a lightweight and cross-platform tool .It is a very complete IDE and is available for free (https://code.visualstudio.com/). You may need to install c++ extension and the compiler you prefer (The extension is still in preview and its focus is code editing, navigation, and debugging support for C and C++). For more information on how to use VSCode with c++ refer to: https://code.visualstudio.com/docs/languages/cpp .

For code execution in VSCode you will need to set up your task first. An example using g++ :  

```sh
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build",
            "type": "shell",
            "command": "g++ -g -std=c++11 Conceptual/main.cc -o main",
            "group":{
                "kind": "build",
                "isDefault": true    
            },
            "problemMatcher":"$gcc"
        }
    ]
}
```
Then you just need to start the executable. In case you have some doubts here you have an useful [tutorial] using vscode.   

## Contributor's Guide

I appreciate any help, whether it's a simple fix of a typo or a whole new example. Just make a fork, make your change and submit a pull request.

Here's a style guide which might help you to keep your changes consistent with the rest of the project's code:

1. All code should match the [Google style guide].
2. Aim to put all code within one .cc file. Yes, I realize that it's not how it supposed to be done in production. However, it helps people to understand examples better, since all code fits into one screen.
3. The comments doesn't follow the style guide for compatibility reasons withe other language examples. 



## License

This work is licensed under a Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.

<a rel="license" href="http://creativecommons.org/licenses/by-nc-nd/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-nd/4.0/80x15.png" /></a>



[Google style guide]: <https://google.github.io/styleguide/cppguide.html#C++_Version>
[tutorial]: <https://www.youtube.com/watch?v=-erXR6k9TeE>
