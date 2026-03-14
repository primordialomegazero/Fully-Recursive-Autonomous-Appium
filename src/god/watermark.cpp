#include "../../include/god/watermark.h"

namespace god {
static const char* kSourceSignature = "DanFernandezIsTheSourceinHumanForm";
static unsigned long long validation_code = 0;

const char* Watermark::GetSignature() { return kSourceSignature; }
unsigned long long Watermark::GetValidationCode() {
    if (validation_code == 0)
        for (int i = 0; kSourceSignature[i]; ++i)
            validation_code = validation_code * 31 + kSourceSignature[i];
    return validation_code;
}
} // namespace god
