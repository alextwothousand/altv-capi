#include "tool.hpp"

static auto enumMatcher = enumDecl(

    unless(isExpansionInSystemHeader())
).bind("enum");

static Handler enumHandler(enumMatcher, [](const MatchFinder::MatchResult& result){
    auto enumdecl = result.Nodes.getNodeAs<EnumDecl>("enum");

    // Data
    json enumjson;
    auto enumname = GetCName(enumdecl->getQualifiedNameAsString());
    //logfile << "Enum: " << enumname << std::endl;

    // Write to header
    capiheader << "typedef enum " << enumname << " {\n";

    // enums
    for(auto e = enumdecl->enumerator_begin(); e != enumdecl->enumerator_end(); )
    {
        // data
        auto ename = ToUpper(enumname)+("_")+e->getName().str();
        auto eval = e->getInitVal().getLimitedValue();
        //logfile << ename << " = " << eval << std::endl;

        // header
        capiheader << "    " << ename << " = " << eval 
            << (++e != enumdecl->enumerator_end() ? ",\n" : "\n");

        // json
        enumjson["enumerators"].push_back({
            {"name", ename},
            {"value", eval},
        });
    }

    capiheader << "};\n" << std::endl;

    // json
    capijson["enums"][enumname] = enumjson;
    
    // ast
    //enumdecl->dump(ast);
});
