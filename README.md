# solver
The solver select an optimal arrangement of filters to decode a given source. 
## Publishing it as a runtime

`solver_minimal.json` makes this directory a **runtime** for the Bevara registry: the
main module that accessors link against, named by pages as `using="solver_minimal_1"`.

```bash
source <emsdk 6.0.8>/emsdk_env.sh      # third_parties/emsdk_vers.txt
bevara publish --build                  # builds .bevara/.build/solver_minimal_1.js and .wasm, signs, uploads
```

The version is the generation (`set(VERSION "1")` in CMakeLists.txt), the
licence comes from `LICENCE`, and the built-in filters from `loader.js`.
Filters whose `"using"` names `solver_minimal_1` can only be published once this
runtime is.

## Provenance of lib/libgpac_static.a

The static GPAC library is prebuilt. It reports `GPAC 26.08-DEV` with the
revision string `release`, so the exact GPAC commit is not recorded in the
binary. It is presumably built from the player's GPAC tree
(`filters/third_parties/gpac`, commit 5a9988d13 at the time of writing) -
**to confirm**, with the configure options, before relying on a published
build for compliance.
