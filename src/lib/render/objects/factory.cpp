#include "factory.hpp"

#include "plane.hpp"
#include "ellipsoid.hpp"
#include "box.hpp"

namespace raytracing::render::objects {

std::map<std::string, std::shared_ptr<object>(*)(std::stringstream&)> objectFactory::collection_;

objectFactory::objectFactory() {
    USE_OBJECT(plane);
    USE_OBJECT(ellipsoid);
    USE_OBJECT(box);
}

objectFactory::objectFactory(
    const std::string& name,
    std::shared_ptr<object>(*generator)(std::stringstream&)) {

    collection_[name] = generator;
}

objectFactory objectFactory::instance;

std::shared_ptr<object> objectFactory::generate(std::stringstream stream) {
    std::string name;
    stream >> name;
    if (collection_.find(name) == collection_.end()) {
        return nullptr;
    }
    return collection_[name](stream);
}

} // namespace raytracing::render::objects
