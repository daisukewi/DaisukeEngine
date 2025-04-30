//
// Created by daisu on 30/04/2025.
//
#include "LVK.h"

// Logs GLSL shaders with line numbers annotation
void lvk::logShaderSource(const char* text) {
    uint32_t line = 0;
    uint32_t numChars = 0;
    const char* lineStart = text;

    while (text && *text) {
        if (*text == '\n') {
            if (numChars) {
#if defined(MINILOG_RAW_OUTPUT)
                LLOGL("(%3u) %.*s\n", ++line, numChars, lineStart);
#else
                LLOGL("(%3u) %.*s", ++line, numChars, lineStart);
#endif // MINILOG_RAW_OUTPUT
            } else {
#if defined(MINILOG_RAW_OUTPUT)
                LLOGL("(%3u)\n", ++line);
#else
                LLOGL("(%3u)", ++line);
#endif // MINILOG_RAW_OUTPUT
            }
            numChars = 0;
            lineStart = text + 1;
        } else if (*text == '\r') {
            // skip it to support Windows/UNIX EOLs
            numChars = 0;
            lineStart = text + 1;
        } else {
            numChars++;
        }
        text++;
    }
    if (numChars) {
#if defined(MINILOG_RAW_OUTPUT)
        LLOGL("(%3u) %.*s\n", ++line, numChars, lineStart);
#else
        LLOGL("(%3u) %.*s", ++line, numChars, lineStart);
#endif // MINILOG_RAW_OUTPUT
    }
    LLOGL("\n");
}