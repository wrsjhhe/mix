
#include <materials/MeshBasicMaterial.h>

using namespace mix;

MeshBasicMaterial::MeshBasicMaterial()
    : MaterialWithColor(0xffffff),
    MaterialWithAoMap(1),
    MaterialWithLightMap(1),
    MaterialWithCombine(CombineOperation::Multiply),
    MaterialWithReflectivity(1, 0.98f),
    MaterialWithWireframe(false, 1) {}


std::string MeshBasicMaterial::type() const {

    return "MeshBasicMaterial";
}

std::shared_ptr<Material> MeshBasicMaterial::clone() const {

    auto m = create();
    copyInto(m.get());

    m->color.copy(color);

    m->map = map;

    m->lightMap = lightMap;
    m->lightMapIntensity = lightMapIntensity;

    m->aoMap = aoMap;
    m->aoMapIntensity = aoMapIntensity;

    m->specularMap = specularMap;

    m->alphaMap = alphaMap;

    m->envMap = envMap;
    m->combine = combine;
    m->reflectivity = reflectivity;
    m->refractionRatio = refractionRatio;

    m->wireframe = wireframe;
    m->wireframeLinewidth = wireframeLinewidth;

    return m;
}

std::shared_ptr<MeshBasicMaterial> MeshBasicMaterial::create(const std::unordered_map<std::string, MaterialValue>& values) {

    auto m = std::shared_ptr<MeshBasicMaterial>(new MeshBasicMaterial());
    m->setValues(values);

    return m;
}


bool MeshBasicMaterial::setValue(const std::string& key, const MaterialValue& value) {

    if (key == "color") {

        color.copy(extractColor(value));

        return true;

    }
    else if (key == "wireframe") {

        wireframe = std::get<bool>(value);
        return true;

    }
    else if (key == "wireframeLinewidth") {

        wireframeLinewidth = extractFloat(value);
        return true;

    }
    else if (key == "map") {

        map = std::get<std::shared_ptr<Texture>>(value);
        return true;

    }
    else if (key == "alphaMap") {

        alphaMap = std::get<std::shared_ptr<Texture>>(value);
        return true;

    }
    else if (key == "specularMap") {

        specularMap = std::get<std::shared_ptr<Texture>>(value);
        return true;

    }
    else if (key == "aoMap") {

        aoMap = std::get<std::shared_ptr<Texture>>(value);
        return true;

    }
    else if (key == "aoMapIntensity") {

        aoMapIntensity = extractFloat(value);
        return true;

    }
    else if (key == "lightMap") {

        lightMap = std::get<std::shared_ptr<Texture>>(value);
        return true;

    }
    else if (key == "lightMapIntensity") {

        lightMapIntensity = extractFloat(value);
        return true;

    }
    else if (key == "envMap") {

        envMap = std::get<std::shared_ptr<Texture>>(value);
        return true;

    }
    else if (key == "combine") {

        combine = std::get<CombineOperation>(value);
        return true;

    }
    else if (key == "reflectivity") {

        reflectivity = extractFloat(value);
        return true;

    }
    else if (key == "refractionRatio") {

        refractionRatio = extractFloat(value);
        return true;
    }

    return false;
}
