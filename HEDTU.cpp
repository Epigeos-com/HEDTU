#include <iostream>
#include <algorithm>
#include <string>
#include <iterator>
#include <fstream>
#include <cctype>
#include <filesystem>
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

int main(int argc, char *argv[]) {
    bool specialDegree = false;
    bool specialSpace = false;
    bool useLeftAlt = false;
    string languageName = "English";
    string lang = "eng";
    string base = "QWERTY";
    string format = "xkb";
    if (argc > 1) lang = strtolower(argv[1]);
    if (argc > 2) base = strtoupper(argv[2]);
    if (argc > 3) format = strtolower(argv[3]);
    for (int i = 0; i < argc; i++) {
        if (strtolower(argv[i]) == "--from-each-according-to-their-ability-to-each-according-to-their-needs") // easter-egg
        {
            specialDegree = true;
        }else if (strtolower(argv[i]) == "--widzisz-mnie") // easter-egg
        {
            specialSpace = true;
        }else if (strtolower(argv[i]) == "--use-left-alt")
        {
            useLeftAlt = true;
        }
    }

    // Such splitting allows for easier addition of new bases, languages and formats, as they're separate, so you can e.g. add a format without recreating all the languages and bases
    string keyListStatic[48] = { // Base characters and ones that stay with base characters regardless of language
        "degree", "underscore, bar", "space, space",
        "1, question, exclam", "2, at", "3, numbersign, cent", "4, dollar, EuroSign", "5, percent, permille", "6, asciicircum, asciitilde", "7, ampersand, grave", "8, asterisk", "9, parenleft, U2208, U2209", "0, parenright, U2282, U2284", "minus, plus, U2219, U2A2F", "equal, notequal, U2248, U2249",
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
    string unicodeKeyTemplate = "U{XXXX}"; // These can be used for language-specific letters, for compatibility, they only use unicode
    string unicodeKeyTemplateDouble = "U{XXXX}, U{YYYY}";
    string unicodeKeyTemplateTripple = "U{XXXX}, U{YYYY}, U{ZZZZ}";
    string unicodeKeyTemplateQuadruple = "U{XXXX}, U{YYYY}, U{ZZZZ}, U{WWWW}";
    
    if (format == "xkb"){
        if (specialDegree) keyListStatic[0] += ", U262D";
        if (specialSpace) keyListStatic[2] += ", U2800";
        // useLeftAlt is set in the final format if
    }else { // format-point // change keyLists and unicodeKeyTemplates to fit the format, implement support for specialDegree, specialSpace and where supported useLeftAlt
        cout << format + " is not supported. Available options: xkb."; // format-point
        return -1;
    }

    if (lang == "eng"){
        languageName = "English";
    }else if (lang == "ell"){
        languageName = "Ελληνικά";
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
    }else{ // language-point
        cout << lang + " is not supported. Make sure you're entering the ISO639-3 code, not ISO639-1. Available options: eng, ell."; // language-point
        return -1;
    }

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
    }else if (base != "QWERTY"){ // base-point // Using swap allows for use in any implemented format, so you should only use swap
        cout << base + " is not supported. Available options: QWERTY, Dvorak."; // base-point
        return -1;
    }

    string text = "";
    if (format == "xkb"){
        text = "// HEDTU keyboard layout for English.\n"
        "// https://epigeos.com/ge/HEDTU\n"
        "// Layout and software made by Kirki Walkowiak from epigeos.com\n\n"

        "default partial alphanumeric_keys\n"
        "xkb_symbols \"basic\" {\n\n"

        "   name[Group1] = \""+languageName+" (HEDTU, "+base+")\";\n\n";

        if (useLeftAlt) text += "   include \"level3(lalt_switch)\"\n";
        text += "   include \"level3(ralt_switch)\"\n\n"

        "   key <TLDE>  {[ "+keyListStatic[0]+keyListDynamic[0]+" ]};\n"
        "   key <BKSL>  {[ "+keyListStatic[1]+keyListDynamic[1]+" ]};\n"
        "   key <SPCE>  {[ "+keyListStatic[2]+keyListDynamic[2]+" ]};\n\n"

        ;
        for (int i = 1; i <= 12; i++){
            string iString = to_string(i);
            if (iString.size() == 1) iString = "0" + iString;
            text += "   key <AE"+iString+">  {[ "+keyListStatic[i + 2]+keyListDynamic[i + 2]+" ]};\n";
        }
        text += "\n";
        for (int i = 1; i <= 12; i++){
            string iString = to_string(i);
            if (iString.size() == 1) iString = "0" + iString;
            text += "   key <AD"+iString+">  {[ "+keyListStatic[i + 14]+keyListDynamic[i + 14]+" ]};\n";
        }
        text += "\n";
        for (int i = 1; i <= 11; i++){
            string iString = to_string(i);
            if (iString.size() == 1) iString = "0" + iString;
            text += "   key <AC"+iString+">  {[ "+keyListStatic[i + 26]+keyListDynamic[i + 26]+" ]};\n";
        }
        text += "\n";
        for (int i = 1; i <= 10; i++){
            string iString = to_string(i);
            if (iString.size() == 1) iString = "0" + iString;
            text += "   key <AB"+iString+">  {[ "+keyListStatic[i + 37]+keyListDynamic[i + 37]+" ]};\n";
        }
        text += "\n};";

        bool addToList = true;
        if (std::filesystem::exists("/usr/share/X11/xkb")){
            if (std::filesystem::exists("/usr/share/X11/xkb/symbols/" + lang)){
                cout << "/usr/share/X11/xkb/symbols/" + lang + " already exists.\n";
                std::ifstream prevIn ("/usr/share/X11/xkb/symbols/" + lang);
                if (!prevIn.is_open()){
                    cout << "Failed to open ifstream at /usr/share/X11/xkb/symbols/" + lang + ". Make sure you're running HEDTU with sudo.\n";
                    return -1;
                }
                if (std::filesystem::exists(lang + "_old")){
                    cout << "Failed copying previous version of the /symbols/ file, " + lang + "_old already exists.\n";
                    return -1;
                }
                std::ofstream prevOut (lang + "_old");
                if (!prevOut.is_open()){
                    cout << "Failed to open ofstream for the previous /symbols/ file in the current directory.\n";
                    return -1;
                }
                prevOut << prevIn.rdbuf();
                prevIn.close();
                cout << "A copy of the previous version was saved in the current directory. Replacing.\n";
                addToList = false;
            }
            std::ofstream file ("/usr/share/X11/xkb/symbols/" + lang);
            if (!file.is_open()){
                cout << "Failed to open ofstream at /usr/share/X11/xkb/symbols/" + lang + ". Make sure you're running HEDTU with sudo.\n";
                if (std::filesystem::exists(lang)){
                    cout << "Failed saving in the current directory for manual installation, " + lang + " already exists.\n";
                    return -1;
                }
                file.open(lang);
                if (!file.is_open()){
                    cout << "Failed to open ofstream for the /symbols/ file in the current directory, most likely due to permission issues.\n";
                    return -1;
                }
                cout << "The /symbols/ file will be saved to the current directory and can be added manually.\n";
                addToList = false;
            }
            file << text << std::endl;
            file.close();
            if (addToList){
                cout << "Adding layout to rules.\n";
                string triggerLine = "! layout";
                string layoutNode = "  "+lang+"    "+languageName+" (HEDTU, "+base+")";
                tryAddLayoutToList("/usr/share/X11/xkb/rules/base.lst", triggerLine, layoutNode);
                tryAddLayoutToList("/usr/share/X11/xkb/rules/evdev.lst", triggerLine, layoutNode);

                triggerLine = "  <layoutList>";
                layoutNode = "    <layout>\n"
"      <configItem>\n"
"        <name>"+lang+"</name>\n"
"        <shortDescription>"+lang+"</shortDescription>\n"
"        <description>"+languageName+" (HEDTU, "+base+")"+"</description>\n"
"        <countryList>\n"
"          <iso3166Id>US</iso3166Id>\n"
"        </countryList>\n"
"        <languageList>\n"
"          <iso639Id>"+lang+"</iso639Id>\n"
"        </languageList>\n"
"      </configItem>\n"
"    </layout>";
                tryAddLayoutToList("/usr/share/X11/xkb/rules/base.xml", triggerLine, layoutNode);
                tryAddLayoutToList("/usr/share/X11/xkb/rules/evdev.xml", triggerLine, layoutNode);
                tryAddLayoutToList("/usr/share/X11/xkb/rules/base.extras.xml", triggerLine, layoutNode);
                tryAddLayoutToList("/usr/share/X11/xkb/rules/evdev.extras.xml", triggerLine, layoutNode);

                cout << "The layout should now be available in your xkb settings.\n";
            }else{
                cout << "Remove the layout from your used layouts and add it again in xkb settings to apply changes.\n";
            }
        }else{
            cout << "/usr/share/X11/xkb not found.\n";
            if (std::filesystem::exists(lang)){
                cout << "Failed saving in the current directory for manual installation, " + lang + " already exists.\n";
                return -1;
            }
            std::ofstream file (lang);
            if (!file.is_open()){
                cout << "Failed to open ofstream for the /symbols/ file in the current directory, most likely due to permission issues.\n";
                return -1;
            }
            file << text << std::endl;
            file.close();
            cout << "The /symbols/ file was saved to the current directory and can be added manually.\n";
        }
    } // format-point

    cout << "Success." << std::endl;
    return 0;
}