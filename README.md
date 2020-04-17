# CABPP
 C++-implemented Automata Based Programming Pattern

## Requirements
- __Windows OS__
- __MinGW__ with __g++__ or __clang++__
- __tinyxml2__ (included in this repo as a submodule)
- __yEd Graph Editor__

## Steps to install
- Clone this git repository (recursively)
- `make all` in main folder (Makefiles are included)
- if everything went ok all binaries will be built in the ./build folder
- settings and graphs are handled in the xmlsettings folder
- Execute the code with `./build/framework.exe`

## Project Description
I already did something similar in Python which you can check in my other repo. This would be a good start to learn about how this programming pattern works.

Goal of this project would be to use the documentation as the code.

The graphs serve as the behaviour logic of each C++ dll module. Each module has its own states it executes when changed on a specific command.

Macros are included in the common folder headers.

## Graphs
I included the possibility to add one of two types of graphs:
- __Directed__ : when your module must follow certain design flow. Every module starts from the __\_start\___ state.
- __Fully connected__ : also called complete. When using this graph type it is best to follow the REST design principles. This kind of graph does not need to have edges as it assumes every node is connected to every other node. The command to execute the given node is the same as the name of the node.

## Example included
The current example implements a completely simplicated TCP socket server, where one module handles all the socket connections while the other module handles the message to be sent to each client.

The TCP_Socket module must be handled as a directed graph while the Connection module is registered as fully connected.

Naturally this is a oversimplification of what can be done using this programming pattern.


## TODO
I need to implement some more explanations here. In short this is a framework with makefile for building modules which it can handle.

Some code can also be improved (e.g. using futexes, headers to handle C code, even better makefile, ... )

## LICENSE:
GPLv3
