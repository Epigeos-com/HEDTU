#include <iostream>
#include <algorithm>
#include <string>
#include <iterator>
#include <fstream>
#include <cctype>
#include <filesystem>
#include <pwd.h>
#include <vector>
using namespace std;

// Throughout the code there are comments for base-point, language-point and format-point, these mark places where you'll want to add your code if you add a new base, language or format

string strtolower(string str){
    transform(str.begin(), str.end(), str.begin(),
    [](unsigned char c){ return tolower(c); });
    return str;
}
string strtoupper(string str){
    transform(str.begin(), str.end(), str.begin(),
    [](unsigned char c){ return toupper(c); });
    return str;
}
string replace(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return str;
    return str.replace(start_pos, from.length(), to);
}
void tryAddLayoutToList(std::string filePath, std::string triggerLine, std::string layoutNode){
    string finalContent = "";
    std::string line;
    std::ifstream fileOut (filePath);
    while (std::getline(fileOut, line)) {
        finalContent += line + "\n";
        if (line.compare(triggerLine) == 0) finalContent += layoutNode + "\n";
    }
    fileOut.close();

    std::ofstream fileIn (filePath);
    fileIn << finalContent << std::endl;
    fileIn.close();
}

struct specificLayout {
    string base;
    string alphabet;
    string keyListStatic[48];
    string keyListDynamic[48];
    string layoutString;
    string layoutStringShort;
};
specificLayout generateSpecificLayout(vector<string> availableAlphabets, vector<string> availableBases, string lang, string languageName, string format, string base, string alphabet, bool specialDegree, bool specialSpace, bool useLeftAlt){
    // Such splitting allows for easier addition of new bases, languages and formats, as they're separate, so you can e.g. add a format without recreating all the languages and bases
    string keyListStatic[48] = { // Base characters and ones that stay with base characters regardless of language
        "degree", "underscore, bar", "space, space",
        "1, question, exclam", "2, at", "3, numbersign", "4, dollar", "5, percent, permille", "6, asciicircum, asciitilde", "7, ampersand, grave", "8, asterisk, U2219, U2A2F", "9, parenleft, U2208, U2209", "0, parenright, U2282, U2284", "minus, plus, plusminus", "equal, notequal, U2248, U2249",
        "q, Q", "w, W", "e, E", "r, R", "t, T", "y, Y", "u, U", "i, I", "o, O", "p, P", "bracketleft, braceleft, U2264, U2270", "bracketright, braceright, U2265, U2271",
        "a, A", "s, S", "d, D", "f, F", "g, G", "h, H", "j, J", "k, K", "l, L", "semicolon, colon, U22BB", "apostrophe, quotedbl, U22BC, U22BD",
        "z, Z", "x, X", "c, C", "v, V", "b, B", "n, N", "m, M", "comma, less, U226E", "period, greater, U226F", "slash, backslash"
    };
    string keyListDynamic[48] = { // Characters that can be moved in case a language uses a letter in their place, appended to static character strings
        "", "", "",
        "", "", "", "", "", "", "", "", "", "", "", "",
        ", U2203, U2204", ", U2200, U00AC", "", "", ", U2225, U27C2", ", U2227, U22C2", ", U2228, U22C3", "", "", ", U221E, U221D", "", "",
        "", "", "", "", "", ", U2202", ", U2320, U2321", "", "", "", "",
        "", "", "", "", "", "", ", U2234, U2235", "", "", ""
    };
    if (specialDegree) keyListStatic[0] += ", U262D";
    if (specialSpace) keyListStatic[2] += ", U2800";
    string unicodeKeyTemplate = "U{XXXX}"; // These can be used for language-specific letters, for compatibility, they only use unicode // format-point
    string unicodeKeyTemplateDouble = "U{XXXX}, U{YYYY}";
    string unicodeKeyTemplateTripple = "U{XXXX}, U{YYYY}, U{ZZZZ}";
    string unicodeKeyTemplateQuadruple = "U{XXXX}, U{YYYY}, U{ZZZZ}, U{WWWW}";

    if (lang == "ell"){
        string toReplace[25] = {
            "03C2", "03B5", "03C1", "03C4", "03C5", "03B8", "03B9", "03BF", "03C0",
            "03B1", "03C3", "03B4", "03C6", "03B3", "03B7", "03BE", "03BA", "03BB",
            "03B6", "03C7", "03C8", "03C9", "03B2", "03BD", "03BC"
        };
        string toReplaceShift[25] = {
            "03A3", "0395", "03A1", "03A4", "03A5", "03F4", "0399", "039F", "03A0",
            "0391", "03A3", "0394", "03A6", "0393", "0397", "039E", "039A", "039B",
            "0396", "03A7", "03A8", "03A9", "0392", "039D", "039C"
        };
        keyListStatic[15] = replace(unicodeKeyTemplate, "{XXXX}", "0301");
        for (int i = 0; i < 9; i++){
            keyListStatic[i + 16] = replace(replace(unicodeKeyTemplateDouble, "{XXXX}", toReplace[i]), "{YYYY}", toReplaceShift[i]);
        }
        for (int i = 9; i < 18; i++){
            keyListStatic[i + 18] = replace(replace(unicodeKeyTemplateDouble, "{XXXX}", toReplace[i]), "{YYYY}", toReplaceShift[i]);
        }
        for (int i = 18; i < 25; i++){
            keyListStatic[i + 20] = replace(replace(unicodeKeyTemplateDouble, "{XXXX}", toReplace[i]), "{YYYY}", toReplaceShift[i]);
        }
    }else if (lang == "isv"){
        if (alphabet == "LATN") {
            keyListDynamic[17] = ", " + replace(replace(unicodeKeyTemplateDouble, "{XXXX}", "011B"), "{YYYY}", "011A"); // E
            keyListDynamic[28] = ", " + replace(replace(unicodeKeyTemplateDouble, "{XXXX}", "0161"), "{YYYY}", "0160"); // S
            keyListDynamic[38] = ", " + replace(replace(unicodeKeyTemplateDouble, "{XXXX}", "017E"), "{YYYY}", "017D"); // Z
            keyListDynamic[40] = ", " + replace(replace(unicodeKeyTemplateDouble, "{XXXX}", "010D"), "{YYYY}", "010C"); // C
            // Etymological alphabet
            keyListDynamic[15] = ", " + replace(unicodeKeyTemplate, "{XXXX}", "0301"); // Q => ◌́
            keyListDynamic[16] = ", " + replace(replace(unicodeKeyTemplateDouble, "{XXXX}", "0119"), "{YYYY}", "0118"); // W => Ę
            keyListDynamic[18] = ", " + replace(replace(unicodeKeyTemplateDouble, "{XXXX}", "0117"), "{YYYY}", "0116"); // R => Ė
            keyListDynamic[21] = ", " + replace(replace(unicodeKeyTemplateDouble, "{XXXX}", "0173"), "{YYYY}", "0172"); // U
            keyListDynamic[27] = ", " + replace(replace(unicodeKeyTemplateDouble, "{XXXX}", "00E5"), "{YYYY}", "00C5"); // A
            keyListDynamic[23] = ", " + replace(replace(unicodeKeyTemplateDouble, "{XXXX}", "022F"), "{YYYY}", "022E"); // O
            keyListDynamic[29] = ", " + replace(replace(unicodeKeyTemplateDouble, "{XXXX}", "0111"), "{YYYY}", "0110"); // D
            keyListDynamic[39] = ", " + replace(replace(unicodeKeyTemplateDouble, "{XXXX}", "0107"), "{YYYY}", "0106"); // X => Ć
        }else if (alphabet == "CYRL"){
            string toReplace[29] = {
                "0459", "045A", "0435", "0440", "0442", "0437", "0443", "0438", "043E", "043F", "0448",
                "0430", "0441", "0434", "0444", "0433", "0445", "0458", "043A", "043B", "0447", "044B",
                "0436", "0454", "0446", "0432", "0431", "043D", "043C"
            };
            string toReplaceShift[29] = {
                "0409", "040A", "0415", "0420", "0422", "0417", "0423", "0418", "041E", "041F", "0428",
                "0410", "0421", "0414", "0424", "0413", "0425", "0408", "041A", "041B", "0427", "042B",
                "0416", "0404", "0426", "0412", "0411", "041D", "041C"
            };
            keyListStatic[15] = replace(unicodeKeyTemplate, "{XXXX}", "0301");
            for (int i = 0; i < 11; i++){
                keyListStatic[i + 15] = replace(replace(unicodeKeyTemplateDouble, "{XXXX}", toReplace[i]), "{YYYY}", toReplaceShift[i]);
            }
            for (int i = 11; i < 22; i++){
                keyListStatic[i + 16] = replace(replace(unicodeKeyTemplateDouble, "{XXXX}", toReplace[i]), "{YYYY}", toReplaceShift[i]);
            }
            for (int i = 22; i < 29; i++){
                keyListStatic[i + 16] = replace(replace(unicodeKeyTemplateDouble, "{XXXX}", toReplace[i]), "{YYYY}", toReplaceShift[i]);
            }
            keyListStatic[26] = replace(replace(replace(replace(unicodeKeyTemplateQuadruple, "{XXXX}", "005B"), "{YYYY}", "005D"), "{ZZZZ}", "007B"), "{WWWW}", "007D");
            keyListDynamic[4] = ", " + replace(replace(unicodeKeyTemplateDouble, "{XXXX}", "2264"), "{YYYY}", "2270");
            keyListDynamic[5] = ", " + replace(replace(unicodeKeyTemplateDouble, "{XXXX}", "2265"), "{YYYY}", "2271");
        }
    }else if (lang == "pol"){
        keyListDynamic[17] = ", " + replace(replace(unicodeKeyTemplateDouble, "{XXXX}", "0119"), "{YYYY}", "0118"); // E
        keyListDynamic[23] = ", " + replace(replace(unicodeKeyTemplateDouble, "{XXXX}", "00F3"), "{YYYY}", "00D3"); // O
        keyListDynamic[27] = ", " + replace(replace(unicodeKeyTemplateDouble, "{XXXX}", "0105"), "{YYYY}", "0104"); // A
        keyListDynamic[28] = ", " + replace(replace(unicodeKeyTemplateDouble, "{XXXX}", "015B"), "{YYYY}", "015A"); // S
        keyListDynamic[35] = ", " + replace(replace(unicodeKeyTemplateDouble, "{XXXX}", "0142"), "{YYYY}", "0141"); // L
        keyListDynamic[38] = ", " + replace(replace(unicodeKeyTemplateDouble, "{XXXX}", "017C"), "{YYYY}", "017B"); // Z
        keyListDynamic[39] = ", " + replace(replace(unicodeKeyTemplateDouble, "{XXXX}", "017A"), "{YYYY}", "0179"); // X
        keyListDynamic[40] = ", " + replace(replace(unicodeKeyTemplateDouble, "{XXXX}", "0107"), "{YYYY}", "0106"); // C
        keyListDynamic[43] = ", " + replace(replace(unicodeKeyTemplateDouble, "{XXXX}", "0144"), "{YYYY}", "0143"); // N
    } // language-point // all languages, including Cyrillic ones should use the QWERTY layout by default as other layouts are implemented as based on the QWERTY one, you can simply write it in your preferred format and run the swaps in reverse order

    if (base == "DVORAK"){
        swap(keyListStatic[42], keyListStatic[43]);
        swap(keyListStatic[39], keyListStatic[42]);
        swap(keyListStatic[38], keyListStatic[39]);
        swap(keyListStatic[35], keyListStatic[38]);
        swap(keyListStatic[33], keyListStatic[34]);
        swap(keyListStatic[32], keyListStatic[33]);
        swap(keyListStatic[31], keyListStatic[34]);
        swap(keyListStatic[32], keyListStatic[41]);
        swap(keyListStatic[31], keyListStatic[40]);
        swap(keyListStatic[30], keyListStatic[34]);
        swap(keyListStatic[29], keyListStatic[32]);
        swap(keyListStatic[28], keyListStatic[38]);
        swap(keyListStatic[24], keyListStatic[28]);
        swap(keyListStatic[23], keyListStatic[28]);
        swap(keyListStatic[22], keyListStatic[31]);
        swap(keyListStatic[21], keyListStatic[30]);
        swap(keyListStatic[20], keyListStatic[34]);
        swap(keyListStatic[19], keyListStatic[34]);
        swap(keyListStatic[18], keyListStatic[23]);
        swap(keyListStatic[17], keyListStatic[29]);
        swap(keyListStatic[15], keyListStatic[39]);
        swap(keyListStatic[13], keyListStatic[15]);
        swap(keyListStatic[13], keyListStatic[47]);
        swap(keyListStatic[36], keyListStatic[38]);
        swap(keyListStatic[17], keyListStatic[46]);
        swap(keyListStatic[16], keyListStatic[45]);
        swap(keyListStatic[15], keyListStatic[37]);
        swap(keyListStatic[14], keyListStatic[26]);
        swap(keyListStatic[13], keyListStatic[25]);

        swap(keyListDynamic[42], keyListDynamic[43]);
        swap(keyListDynamic[39], keyListDynamic[42]);
        swap(keyListDynamic[38], keyListDynamic[39]);
        swap(keyListDynamic[35], keyListDynamic[38]);
        swap(keyListDynamic[33], keyListDynamic[34]);
        swap(keyListDynamic[32], keyListDynamic[33]);
        swap(keyListDynamic[31], keyListDynamic[34]);
        swap(keyListDynamic[32], keyListDynamic[41]);
        swap(keyListDynamic[31], keyListDynamic[40]);
        swap(keyListDynamic[30], keyListDynamic[34]);
        swap(keyListDynamic[29], keyListDynamic[32]);
        swap(keyListDynamic[28], keyListDynamic[38]);
        swap(keyListDynamic[24], keyListDynamic[28]);
        swap(keyListDynamic[23], keyListDynamic[28]);
        swap(keyListDynamic[22], keyListDynamic[31]);
        swap(keyListDynamic[21], keyListDynamic[30]);
        swap(keyListDynamic[20], keyListDynamic[34]);
        swap(keyListDynamic[19], keyListDynamic[34]);
        swap(keyListDynamic[18], keyListDynamic[23]);
        swap(keyListDynamic[17], keyListDynamic[29]);
        swap(keyListDynamic[15], keyListDynamic[39]);
        swap(keyListDynamic[13], keyListDynamic[15]);
        swap(keyListDynamic[13], keyListDynamic[47]);
        swap(keyListDynamic[36], keyListDynamic[38]);
        swap(keyListDynamic[17], keyListDynamic[46]);
        swap(keyListDynamic[16], keyListDynamic[45]);
        swap(keyListDynamic[15], keyListDynamic[37]);
        swap(keyListDynamic[14], keyListDynamic[26]);
        swap(keyListDynamic[13], keyListDynamic[25]);
    }else if (base == "JCUKEN"){
        swap(keyListStatic[42], keyListStatic[44]);
        swap(keyListStatic[41], keyListStatic[42]);
        swap(keyListStatic[40], keyListStatic[16]);
        swap(keyListStatic[39], keyListStatic[37]);
        swap(keyListStatic[38], keyListStatic[39]);
        swap(keyListStatic[36], keyListStatic[39]);
        swap(keyListStatic[35], keyListStatic[42]);
        swap(keyListStatic[34], keyListStatic[42]);
        swap(keyListStatic[32], keyListStatic[42]);
        swap(keyListStatic[31], keyListStatic[43]);
        swap(keyListStatic[29], keyListStatic[35]);
        swap(keyListStatic[28], keyListStatic[40]);
        swap(keyListStatic[27], keyListStatic[30]);
        swap(keyListStatic[25], keyListStatic[42]);
        swap(keyListStatic[24], keyListStatic[31]);
        swap(keyListStatic[23], keyListStatic[33]);
        swap(keyListStatic[22], keyListStatic[42]);
        swap(keyListStatic[21], keyListStatic[43]);
        swap(keyListStatic[20], keyListStatic[24]);
        swap(keyListStatic[19], keyListStatic[43]);
        swap(keyListStatic[18], keyListStatic[32]);
        swap(keyListStatic[17], keyListStatic[19]);
        swap(keyListStatic[38], keyListStatic[28]);

        // language-point // Cyrillic languages will have this layout vary a bunch, rn it's implemented for isv, feel free to change the base to something more universal and use ifs to adapt it for other languages
    } // base-point // Using swap allows for use in any implemented format, so you should only use swap

    string layoutString = languageName + " (HEDTU";
    if (availableAlphabets.size() > 1) layoutString += ", " + alphabet;
    if (availableBases.size() > 1) layoutString += ", " + base;
    layoutString += ")";
    string layoutStringShort = "";
    if (availableAlphabets.size() > 1) layoutStringShort += strtolower(alphabet);
    if (availableBases.size() > 1) layoutStringShort += strtolower(base);

    specificLayout layout;
    layout.base = base;
    layout.alphabet = alphabet;
    std::ranges::copy(keyListStatic, layout.keyListStatic);
    std::ranges::copy(keyListDynamic, layout.keyListDynamic);
    layout.layoutString = layoutString;
    layout.layoutStringShort = layoutStringShort;
    return layout;
}

int main(int argc, char *argv[]) {
    bool specialDegree = false;
    bool specialSpace = false;
    bool useLeftAlt = false;
    if (argc <= 2){
        cout << "Not enough arguments. Usage: sudo ./HEDTU [language] [format] <options>.";
        return -1;
    }
    string lang = strtolower(argv[1]);
    string format = strtolower(argv[2]);
    for (int i = 0; i < argc; i++) {
        if (strtolower(argv[i]) == "--from-each-according-to-their-ability-to-each-according-to-their-needs"){
            specialDegree = true;
        }else if (strtolower(argv[i]) == "--use-hard-space"){
            specialSpace = true;
        }else if (strtolower(argv[i]) == "--use-left-alt"){
            useLeftAlt = true;
        }
    }

    string languageName = "English";
    if (lang == "ell"){
        languageName = "Ελληνικά";
    }else if (lang == "isv"){
        languageName = "Medžuslovjansky";
    }else if (lang == "pol"){
        languageName = "Polski";
    } // language-point

    std::vector<specificLayout> layouts = {};
    
    std::vector<string> availableAlphabets = {};
    if (lang == "isv"){
        availableAlphabets.push_back("LATN");
        availableAlphabets.push_back("CYRL");
    }else if (lang == "eng" || lang == "ell" || lang == "pol"){
        availableAlphabets.push_back("");
    }else { // language-point
        cout << lang + " is not supported. Make sure you're entering the ISO639-3 code, not ISO639-1. Available options: eng, ell, isv, pol."; // language-point
        return -1;
    }

    for (int i = 0; i < availableAlphabets.size(); i++) {
        const string alphabet = availableAlphabets[i];
        std::vector<string> availableBases = {};
        if (lang == "eng"){
            availableBases.push_back("QWERTY");
            availableBases.push_back("DVORAK");
        }else if (lang == "ell"){
            availableBases.push_back("QWERTY"); // TODO: Is there a Dvorak for scripts other than latin?
        }else if (lang == "isv"){
            availableBases.push_back("QWERTY");
            if (alphabet == "LATN") availableBases.push_back("DVORAK"); // TODO: CYRL Dvorak?
            if (alphabet == "CYRL") availableBases.push_back("JCUKEN");
        }else if (lang == "pol"){
            availableBases.push_back("QWERTY");
            availableBases.push_back("DVORAK");
        } // language-point

        for (int j = 0; j < availableBases.size(); j++){
            const string base = availableBases[j];
            const specificLayout layout = generateSpecificLayout(availableAlphabets, availableBases, lang, languageName, format, base, alphabet, specialDegree, specialSpace, useLeftAlt);
            layouts.push_back(layout);
        }
    }

    string text = "";
    if (format == "xkb" || format == "fcitx5"){
        text = "// HEDTU keyboard layout for " + lang + " (" + languageName + ").\n"
            "// https://github.com/Epigeos-com/HEDTU\n"
            "// Layout and software made by Kirka Walkowiak from epigeos.com\n\n";

        for (int i = 0; i < layouts.size(); i++){
            const specificLayout layout = layouts[i];

            if (i == 0) text += "default ";
            text += "partial alphanumeric_keys\n"
            "xkb_symbols \"" + layout.layoutStringShort + "\" {\n\n"

            "   name[Group1] = \"" + layout.layoutString + "\";\n\n";

            if (useLeftAlt) text += "   include \"level3(lalt_switch)\"\n";
            text += "   include \"level3(ralt_switch)\"\n\n"

            "   key <TLDE>  {[ "+layout.keyListStatic[0]+layout.keyListDynamic[0]+" ]};\n"
            "   key <BKSL>  {[ "+layout.keyListStatic[1]+layout.keyListDynamic[1]+" ]};\n"
            "   key <SPCE>  {[ "+layout.keyListStatic[2]+layout.keyListDynamic[2]+" ]};\n\n"

            ;
            for (int i = 1; i <= 12; i++){
                string iString = to_string(i);
                if (iString.size() == 1) iString = "0" + iString;
                text += "   key <AE"+iString+">  {[ "+layout.keyListStatic[i + 2]+layout.keyListDynamic[i + 2]+" ]};\n";
            }
            text += "\n";
            for (int i = 1; i <= 12; i++){
                string iString = to_string(i);
                if (iString.size() == 1) iString = "0" + iString;
                text += "   key <AD"+iString+">  {[ "+layout.keyListStatic[i + 14]+layout.keyListDynamic[i + 14]+" ]};\n";
            }
            text += "\n";
            for (int i = 1; i <= 11; i++){
                string iString = to_string(i);
                if (iString.size() == 1) iString = "0" + iString;
                text += "   key <AC"+iString+">  {[ "+layout.keyListStatic[i + 26]+layout.keyListDynamic[i + 26]+" ]};\n";
            }
            text += "\n";
            for (int i = 1; i <= 10; i++){
                string iString = to_string(i);
                if (iString.size() == 1) iString = "0" + iString;
                text += "   key <AB"+iString+">  {[ "+layout.keyListStatic[i + 37]+layout.keyListDynamic[i + 37]+" ]};\n";
            }
            text += "\n};\n\n\n";
        }

        bool addToList = true;
        if (std::filesystem::exists("/usr/share/X11/xkb")){
            if (std::filesystem::exists("/usr/share/X11/xkb/symbols/" + lang)){
                cout << "/usr/share/X11/xkb/symbols/" + lang + " already exists. Replacing without changes to /rules/ files.\n";
                addToList = false;
            }
            std::ofstream file ("/usr/share/X11/xkb/symbols/" + lang);
            if (!file.is_open()){
                cout << "Failed to open ofstream at /usr/share/X11/xkb/symbols/" + lang + ". Make sure you're running HEDTU with sudo.\n";
                return -1;
            }
            file << text << std::endl;
            file.close();

            if (addToList){
                cout << "Adding layout to rules.\n";
                string triggerLine = "! layout";
                string layoutNode = "  "+lang+"    "+languageName+" (HEDTU)";
                tryAddLayoutToList("/usr/share/X11/xkb/rules/base.lst", triggerLine, layoutNode);
                tryAddLayoutToList("/usr/share/X11/xkb/rules/evdev.lst", triggerLine, layoutNode);

                for (int i = 0; i < layouts.size(); i++){
                    const specificLayout layout = layouts[i];

                    string triggerLine = "! variant";
                    string layoutNode = "  "+layout.layoutStringShort+"    "+lang+": "+layout.layoutString;
                    tryAddLayoutToList("/usr/share/X11/xkb/rules/base.lst", triggerLine, layoutNode);
                    tryAddLayoutToList("/usr/share/X11/xkb/rules/evdev.lst", triggerLine, layoutNode);
                }

                triggerLine = "  <layoutList>";
                layoutNode = "    <layout>\n"
"      <configItem>\n"
"        <name>"+lang+"</name>\n"
"        <shortDescription>"+lang+"</shortDescription>\n"
"        <description>"+languageName+" (HEDTU)"+"</description>\n"
"        <countryList>\n"
"          <iso3166Id>US</iso3166Id>\n"
"        </countryList>\n"
"        <languageList>\n"
"          <iso639Id>"+lang+"</iso639Id>\n"
"        </languageList>\n"
"      </configItem>\n"
"      <variantList>\n";
                for (int i = 0; i < layouts.size(); i++){
                    const specificLayout layout = layouts[i];

                    layoutNode +=
"        <variant>\n"
"          <configItem>\n"
"            <name>"+layout.layoutStringShort+"</name>\n"
"            <description>"+layout.layoutString+"</description>\n"
"            <countryList>\n"
"              <iso3166Id>US</iso3166Id>\n"
"            </countryList>\n"
"          </configItem>\n"
"        </variant>\n";
                }
                layoutNode += "      </variantList>\n"
"    </layout>";
                tryAddLayoutToList("/usr/share/X11/xkb/rules/base.xml", triggerLine, layoutNode);
                tryAddLayoutToList("/usr/share/X11/xkb/rules/evdev.xml", triggerLine, layoutNode);
                tryAddLayoutToList("/usr/share/X11/xkb/rules/base.extras.xml", triggerLine, layoutNode);
                tryAddLayoutToList("/usr/share/X11/xkb/rules/evdev.extras.xml", triggerLine, layoutNode);
            }
        }else{
            cout << "/usr/share/X11/xkb not found.\n";
            std::ofstream file (lang);
            if (!file.is_open()){
                cout << "Failed to open ofstream for the /symbols/ file in the current directory, most likely due to permission issues.\n";
                return -1;
            }
            file << text << std::endl;
            file.close();
            cout << "The /symbols/ file was saved to the current directory and can be added manually.\n";
        }

        if (format == "fcitx5"){
            const char* sudoUser = std::getenv("SUDO_USER");
            struct passwd* pw = getpwnam(sudoUser);
            string home = std::getenv("HOME");
            if (pw) home = pw->pw_dir;

            if (std::filesystem::exists(home + "/.local/share/fcitx5")){
                std::filesystem::create_directory(home + "/.local/share/fcitx5/inputmethod");

                for (int i = 0; i < layouts.size(); i++){
                    const specificLayout layout = layouts[i];

                    const string path = home + "/.local/share/fcitx5/inputmethod/keyboard-" + lang + "-" + layout.layoutStringShort + ".conf";
                    if (!std::filesystem::exists(path)){
                        std::ofstream file (path);
                        if (!file.is_open()){
                            cout << "Failed to open ofstream for " + path + ", most likely due to permission issues.\n";
                            return -1;
                        }
                        file << "[InputMethod]"
"\nName=" + layout.layoutString +
"\nIcon=input-keyboard"
"\nLangCode=" + lang + "_" + layout.layoutStringShort + 
"\nAddon=keyboard"
"\nConfigurable=True"
"\nLabel=" + lang << std::endl;
                        file.close();
                        cout << "Successfully added " + path + ".\n";
                        
                    }
                }
            }else{
                cout << home + "/.local/share/fcitx5 does not exist.";
                return -1;
            }
        }
    } else {
        cout << format + " is not supported. Available options: xkb, fcitx5."; // format-point
        return -1;
    } // format-point

    cout << "Success." << std::endl;
    return 0;
}