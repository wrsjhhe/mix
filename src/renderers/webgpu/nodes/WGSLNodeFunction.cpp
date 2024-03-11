#include <renderers/webgpu/nodes/WGSLNodeFunction.h>
#include <utils/StringUtils.h>
#include <nodes/core/NodeFunctionInput.h>

#include <regex>
#include <unordered_map>
#include <format>

using namespace mix;

static const std::regex declarationRegexp(R"(^[fn]*\s*([a-z_0-9]+)?\s*\(([\s\S]*?)\)\s*[\-\>]*\s*([a-z_0-9]+)?)", std::regex_constants::icase);
static const std::regex propertiesRegexp(R"([a-z_0-9]+|<(.*?)>+)",std::regex_constants::icase);

static const std::unordered_map<std::string, std::string> wgslTypeLib{ {"f32","float"} };

struct ParsedSource
{
    std::string type;
    std::vector<NodeFunctionInput> inputs;
    std::string name;
    std::string inputsCode;
    std::string blockCode;
};

static ParsedSource parse(std::string source) {
	source = utils::trim(source);
	
    ParsedSource res;
    auto words = std::sregex_iterator(source.begin(), source.end(), declarationRegexp);
    if (words->size() == 4) {
        auto strIter = words->begin();
        res.inputsCode = (strIter + 2)->str();
        std::vector<std::string> propsMatches;

        auto nameMatch = std::sregex_iterator(res.inputsCode.begin(), res.inputsCode.end(), propertiesRegexp);
        auto nameEnd = std::sregex_iterator();
        for (; nameMatch != nameEnd; ++nameMatch) {
            auto iter = nameMatch->begin();
            for (; iter != nameMatch->end(); ++iter) {
                if (iter->matched)
                    propsMatches.emplace_back(iter->str());
            }
        }

        std::vector<NodeFunctionInput> inputs;
        uint32_t i = 0;

        while (i < propsMatches.size()) {
            // default
            auto& name = propsMatches[i++];
            auto& type = propsMatches[i++];

            type = wgslTypeLib.find(type) != wgslTypeLib.end() ? wgslTypeLib.at(type) : type;

            // precision
            if (i < propsMatches.size() && propsMatches[i].front() == '<')
                i++;

            // add input
            NodeFunctionInput input{};
            input.type = type;
            input.name = name;
            res.inputs.emplace_back(input);
        }

        res.blockCode = source.substr(0, propsMatches[0].size());
        res.name = (strIter + 1)->str();
        res.type = (strIter + 3)->str();


    }
    else {
        throw std::exception("FunctionNode: Function is not a WGSL code.");
    }
    return res;
}

WGSLNodeFunction::WGSLNodeFunction(const std::string& source) {
    const ParsedSource& parsed = parse(source);
    type = parsed.type;
    inputs = std::move(parsed.inputs);
    name = parsed.name;

    inputsCode = parsed.inputsCode;
    blockCode = parsed.blockCode;
}

WGSLNodeFunction::~WGSLNodeFunction() {

}

std::string WGSLNodeFunction::getCode() {
    std::string type = this->type != "void" ? "-> " + this->type : "";

    return std::format("fn {} ( {} ) {}",name,utils::trim(inputsCode),type) + blockCode;
}