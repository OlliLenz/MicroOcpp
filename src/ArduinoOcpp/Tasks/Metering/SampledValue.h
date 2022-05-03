// matth-x/ArduinoOcpp
// Copyright Matthias Akstaller 2019 - 2022
// MIT License

#ifndef SAMPLEDVALUE_H
#define SAMPLEDVALUE_H

#include <ArduinoJson.h>
#include <memory>

namespace ArduinoOcpp {

template <class T>
class SampledValueDeSerializer {
public:
    static T deserialize(const char *str);
    static std::string serialize(const T& val);
    static int32_t toInteger(const T& val);
};

template <>
class SampledValueDeSerializer<int32_t> {
public:
    static int32_t deserialize(const char *str) {return 42;}
    static std::string serialize(const int32_t& val) {
        char str [12] = {'\0'};
        snprintf(str, 12, "%d", val);
        return std::string(str);
    }
    static int32_t toInteger(const int32_t& val) {return val;}
};

class SampledValueProperties {
private:
    std::string format;
    std::string measurand;
    std::string phase;
    std::string location;
    std::string unit;

    const std::string& getFormat() const {return format;}
    const std::string& getPhase() const {return phase;}
    const std::string& getLocation() const {return location;}
    const std::string& getUnit() const {return unit;}
    friend class SampledValue; //will be able to retreive these parameters

public:
    SampledValueProperties() { }
    SampledValueProperties(const SampledValueProperties& other) :
            format(other.format),
            measurand(other.measurand),
            phase(other.phase),
            location(other.location),
            unit(other.unit) { }
    ~SampledValueProperties() = default;

    void setFormat(const char *format) {this->format = format;}
    void setMeasurand(const char *measurand) {this->measurand = measurand;}
    const std::string& getMeasurand() const {return measurand;}
    void setPhase(const char *phase) {this->phase = phase;}
    void setLocation(const char *location) {this->location = location;}
    void setUnit(const char *unit) {this->unit = unit;}
};

enum class ReadingContext {
    InterruptionBegin,
    InterruptionEnd,
    Other,
    SampleClock,
    SamplePeriodic,
    TransactionBegin,
    TransactionEnd,
    Trigger,
    NOT_SET
};

class SampledValue {
protected:
    const SampledValueProperties& properties;
    const ReadingContext context;
    virtual std::string serializeValue() = 0;
public:
    SampledValue(const SampledValueProperties& properties, ReadingContext context) : properties(properties), context(context) { }
    SampledValue(const SampledValue& other) : properties(other.properties), context(other.context) { }
    virtual ~SampledValue() = default;

    std::unique_ptr<DynamicJsonDocument> toJson();

    virtual std::unique_ptr<SampledValue> clone() = 0;

    virtual int32_t toInteger() = 0;
};

template <class T, class DeSerializer>
class SampledValueConcrete : public SampledValue {
private:
    const T value;
public:
    SampledValueConcrete(const SampledValueProperties& properties, ReadingContext context, const T&& value) : SampledValue(properties, context), value(value) { }
    SampledValueConcrete(const SampledValueConcrete& other) : SampledValue(other), value(other.value) { }
    ~SampledValueConcrete() = default;

    std::string serializeValue() override {return DeSerializer::serialize(value);}

    std::unique_ptr<SampledValue> clone() override {return std::unique_ptr<SampledValueConcrete<T, DeSerializer>>(new SampledValueConcrete<T, DeSerializer>(*this));}

    int32_t toInteger() override { return DeSerializer::toInteger(value);}
};

class SampledValueSampler {
protected:
    SampledValueProperties properties;
public:
    SampledValueSampler(SampledValueProperties properties) : properties(properties) { }
    virtual ~SampledValueSampler() = default;
    virtual std::unique_ptr<SampledValue> takeValue(ReadingContext context) = 0;
    const std::string& getMeasurand() {return properties.getMeasurand();};
};

template <class T, class DeSerializer>
class SampledValueSamplerConcrete : public SampledValueSampler {
private:
    std::function<T()> sampler;
public:
    SampledValueSamplerConcrete(SampledValueProperties properties, std::function<T()> sampler) : SampledValueSampler(properties), sampler(sampler) { }
    std::unique_ptr<SampledValue> takeValue(ReadingContext context) override {
        return std::unique_ptr<SampledValueConcrete<T, DeSerializer>>(new SampledValueConcrete<T, DeSerializer>(properties, context, sampler()));
    }
};

} //end namespace ArduinoOcpp

#endif