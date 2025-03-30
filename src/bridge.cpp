#ifndef BRIDGE_CPP
#define BRIDGE_CPP

#include <math.h>
#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

// No name mangling
extern "C" {

    // Functions JS can run on app
    int int_sqrt(int x) {
        return sqrt(x);
    }

}

#ifdef __EMSCRIPTEN__
// Function CPP can run on JS
EM_JS(void, open_embed, (const char* link), {
    window.open(UTF8ToString(link));
});  
#else
void open_embed(const char* link);
#endif

#endif