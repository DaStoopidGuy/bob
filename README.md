# bob - the build system
Bob is a build system written exclusively, for C projects.  
Get the hell out of here c++ (middle finger emoji)  

> and yes, i do see the irony in using Make (another build system)
> for my new build system  (i better not get any emails about this...😭)  

## Quickstart  
> Figuring out lol  

in `bob.c` file:  
```c
    #define BOB_IMPL
    #include "bob.h"

    int main(int argc, char **argv) {
        printf("Bob says Hi!\n");
        Bob_Cmd cmd = {0};

        bob_cmd_append(&cmd, "gcc", "src/main.c", "-o", "build/main");
        bob_cmd_run(&cmd);

        bob_cmd_destroy(&cmd);
        return 0;
    }
```  

now in your terminal:  
```shell
    $ gcc bob.c -o bob
    $ ./bob
```

your target executable is `./build/main`:  
```shell
    $ ./build/main
```

The `bob.c` file uses the `bob.h` header-only library functions
to build your C project  

So effectively all you'll need to build your C project
is just a C compiler  

## Todo
- [x] run shell commands
- [ ] strip prefix macro for individual components (like string builder, cmd, etc...)
- [ ] strip prefix macro for all components (for usage in _build script .c_ file `bob.c`)
- [ ] rebuild itself if the build script is changed
