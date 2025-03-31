#ifndef BRIDGE_CPP
#define BRIDGE_CPP

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

typedef void(*functionPtr)(void);

// No name mangling
extern "C" {
    // Functions JS can run on app
    void pointer_unlocked() {
        state.active = false;
    }

    void pointer_locked() {
        state.active = true;
    }

}

#ifdef __EMSCRIPTEN__
// Function CPP can run on JS
EM_JS(void, open_embed, (const char* link), {
    if (typeof window["open_link"] === 'function') {
        open_link(link);
    }
});  
#else
void open_embed(const char* link);
#endif

#endif