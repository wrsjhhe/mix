
#include <core/Uniform.h>

#include <utility>

using namespace mix;

Uniform::Uniform(std::optional<UniformValue> value, std::optional<bool> needsUpdate)
    : value_(std::move(value)), needsUpdate(needsUpdate) {}

bool Uniform::hasValue() const {

    return value_.has_value();
}

UniformValue& Uniform::value() {

    return *value_;
}

void Uniform::setValue(UniformValue value) {

    this->value_ = std::move(value);
}
