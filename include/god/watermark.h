#ifndef WATERMARK_H
#define WATERMARK_H

#include <string>
namespace god {
class Watermark {
public:
    static const char* GetSignature();
    static unsigned long long GetValidationCode();
};
} // namespace god
#endif
