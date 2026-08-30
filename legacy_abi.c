/* Compat shims for symbols some third-party filter side-modules (e.g.
 * libjpeg, libpng) import from "env" but that gpac_minimal's own compiled
 * code never references transitively, so the linker never pulls their
 * definitions out of the system libc archive even when explicitly listed
 * in EXPORTED_FUNCTIONS (--export-if-defined only exports a symbol that
 * already has a definition in the link, it does not force archive
 * extraction on its own). Defining them directly here guarantees they are
 * always part of the link. */

#include <stdlib.h>

void exit(int status) __attribute__((noreturn));
void exit(int status) {
    abort();
}

void _Exit(int status) __attribute__((noreturn));
void _Exit(int status) {
    abort();
}
