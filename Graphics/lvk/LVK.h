//
// Created by daisu on 29/04/2025.
//
#pragma once

#include <cstdint>

#include <minilog/minilog.h>

namespace lvk {

    struct Result {
        enum class Code {
            Ok,
            ArgumentOutOfRange,
            RuntimeError,
          };

        Code code = Code::Ok;
        const char* message = "";
        explicit Result() = default;
        explicit Result(Code code, const char* message = "") : code(code), message(message) {}

        bool isOk() const {
            return code == Result::Code::Ok;
        }

        static void setResult(Result* outResult, Code code, const char* message = "") {
            if (outResult) {
                outResult->code = code;
                outResult->message = message;
            }
        }

        static void setResult(Result* outResult, const Result& sourceResult) {
            if (outResult) {
                *outResult = sourceResult;
            }
        }
    };

    void logShaderSource(const char* text);

}
