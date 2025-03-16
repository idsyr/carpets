#define ERR_PRIO1(S) do { \
		const char *func = #S; \
		fprintf(stderr, "ERROR_CONTINUABLE: FUNC: %s: %s\n", func, SDL_GetError()); \
	} while (0)
#define ERR_PRIO2(S, SEVTY) do { \
		const char *func = #S; \
		fprintf(stderr, "ERROR_CRITICAL: FUNC: %s: %s\b", func, SDL_GetError()); \
		if(SEVTY) {fprintf(stderr, "abort...\n"); abort();}									\
	} while (0)
#define GET_3RD_ARG(A1, A2, A3, ...) A3
#define ERR_MACRO_CHOOSER(...) GET_3RD_ARG(__VA_ARGS__, ERR_PRIO2, ERR_PRIO1)
#define ERR(...) do {ERR_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__);} while(0)


