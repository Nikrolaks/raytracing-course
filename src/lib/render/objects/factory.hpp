#pragma once

#include <render/elementary.hpp>

#include <map>
#include <memory>
#include <string>
#include <sstream>

// bad first try but working :(((
#define REGISTRY_OBJECT(objectType) \
  template<> \
  struct inserter<objectType> : private objectFactory { \
    inserter(); \
  };

#define REGISTRY_OBJECT_IMPL(objectType, objectName) \
  std::shared_ptr<object> parse##objectType(std::stringstream& stream) { \
    return objectType::fromStream(stream); \
  } \
  inserter<objectType>::inserter() : objectFactory(objectName, parse##objectType) { }

#define USE_OBJECT(objectType) inserter<objectType>()

namespace raytracing::render::objects {

class objectFactory {
public:
    static std::shared_ptr<object> generate(std::stringstream stream);
protected:
    objectFactory(
        const std::string& name,
        std::shared_ptr<object>(*generator)(std::stringstream&));

    static std::map<std::string, std::shared_ptr<object>(*)(std::stringstream&)> collection_;
private:
    objectFactory();
    static objectFactory instance;
};

template<class objectT>
struct inserter : private objectFactory {};

template <class objectT>
class Counter {
public:
    static std::string birth(const std::string& name) {
        std::stringstream ss;
        ss << name << "_" << count_;
        ++count_;
        return ss.str();
    }
private:
    static size_t count_;
};

} // namespace raytracing::render::objects
