# bob - the build system
Bob is a build system written exclusively, for C projects.  
Get the hell out of here c++ (middle finger emoji)  

> and yes, i do see the irony in using Make (another build system)
> for my new build system  (i better not get any emails about this...😭)  

## Quickstart  
> Figuring out lol  

in `bob.c` file:  
```c
    #define BOB_STRIP_PREFIXES
    #define BOB_IMPL
    #include "bob.h"

    int main(int argc, char **argv) {
        rebuild_yourself(argv[0]);
        printf("Bob says Hi!\n");
        Cmd cmd = {0};

        cmd_append(&cmd, "gcc", "src/main.c", "-o", "build/main");
        cmd_run(&cmd);

        cmd_destroy(&cmd);
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
- [x] strip prefix macro for individual components (like string builder, cmd, etc...)
- [x] strip prefix macro for all components (for usage in _build script .c_ file `bob.c`)
- [x] rebuild itself if the build script is changed
- [x] functions to check if files/dirs exist
- [ ] function to make dir
- [ ] abstract away the specific compiler, especially in `bob_rebuild_yourself()`
