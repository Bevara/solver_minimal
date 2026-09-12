/* Forces invoke_* trampolines into the glue for signatures side modules need.
 *
 * With JS-based setjmp/longjmp, every indirect call made inside a function
 * that uses setjmp goes through an invoke_<signature> import, and emscripten
 * only defines in the glue the signatures the MAIN module imports. A side
 * module needing one the main module never happened to use gets a lazy stub
 * that fails at its first call with "resolved is not a function" - which is
 * how every image test broke when libgpac_static shrank from 6.5 to 1.5 MB:
 * libpng's error path is a 9-argument call under setjmp (invoke_viiiiiiiii),
 * libicns a 9-argument one returning int (invoke_iiiiiiiiii - the first
 * letter is the return type), and nothing left in the
 * main module used either.
 *
 * The signature is decided by the callee's prototype, not by what is passed,
 * so one call through a pointer of each shape, under setjmp, is enough. The
 * pointers come in as parameters: a static that is never assigned is known
 * to be null and the calls fold away, and with them the imports. Never
 * called; exported so the unit survives dead code elimination. */
#include <setjmp.h>

typedef void (*fn_viiiiiiiii)(int, int, int, int, int, int, int, int, int);
typedef int (*fn_iiiiiiiiii)(int, int, int, int, int, int, int, int, int);

int gpac_sjlj_anchor(fn_viiiiiiiii f9, fn_iiiiiiiiii f9i, int a)
{
	jmp_buf jb;
	int r = 0;
	if (setjmp(jb))
		return -1;
	f9(a, a, a, a, a, a, a, a, a);
	r = f9i(a, a, a, a, a, a, a, a, a);
	return r;
}
