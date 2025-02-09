#include <Geode>

USE_GEODE_NAMESPACE();

std::string geode::keyToStringFixed(enumKeyCodes code) {
    switch (code) {
        case KEY_None:      return "";
        case KEY_C:         return "C";
        case KEY_Multiply:  return "Mul";
        case KEY_Divide:    return "Div";
        case KEY_OEMPlus:   return "OEMPlus";
        case KEY_OEMMinus:  return "OEMMinus";
        
        case static_cast<enumKeyCodes>(-1):
            return "Unk";
        
        default:
            return CCDirector::sharedDirector()
                ->getKeyboardDispatcher()
                ->keyToString(code);
    }
}

bool Keybind::operator==(Keybind const& other) const {
    return
        other.key == this->key &&
        other.modifiers == this->modifiers &&
        other.mouse == this->mouse;
}

bool Keybind::operator<(Keybind const& other) const {
    return
        this->key < other.key ||
        this->modifiers < other.modifiers ||
        this->mouse << other.mouse;
}

std::string Keybind::toString() const {
    std::string res = "";

    if (this->modifiers & kmControl)    res += "Ctrl + ";
    if (this->modifiers & kmCommand)    res += "Command + ";
    if (this->modifiers & kmAlt)        res += "Alt + ";
    if (this->modifiers & kmShift)      res += "Shift + ";

    std::string r = "";
    if (this->mouse != kMouseButtonNone) {
        r = mouseToString(this->mouse);
    } else {
        r = keyToStringFixed(this->key);
    }

    if (r.size())
        res += r;
    else
        res = res.substr(0, res.size() - 3);
        
    return res;
}

void Keybind::save(DS_Dictionary* dict) const {
    // dict->setIntegerForKey("key", this->key);
    // dict->setIntegerForKey("modifiers", this->modifiers);
    // dict->setIntegerForKey("click", this->mouse);
}

Keybind::Keybind() {
    this->key = KEY_None;
    this->modifiers = 0;
}

Keybind::Keybind(enumKeyCodes pressed) {
    switch (pressed) {
        // same for command for mac users
        case KEY_Control:       this->key = KEY_None; break;
        case KEY_Shift:         this->key = KEY_None; break;
        case KEY_Alt:           this->key = KEY_None; break;
        default:                this->key = pressed;  break;
    }

    auto kb = CCDirector::sharedDirector()->getKeyboardDispatcher();

    this->modifiers = 0;
    if (kb->getControlKeyPressed())
        this->modifiers |= this->kmControl;
    if (kb->getCommandKeyPressed())
        this->modifiers |= this->kmCommand;
    if (kb->getShiftKeyPressed())
        this->modifiers |= this->kmShift;
    if (kb->getAltKeyPressed())
        this->modifiers |= this->kmAlt;
}

Keybind::Keybind(enumKeyCodes key, Modifiers mods) {
    this->key = key;
    this->modifiers = mods;
}

Keybind::Keybind(enumKeyCodes key, int mods) {
    this->key = key;
    this->modifiers = static_cast<Modifiers>(mods);
}

Keybind::Keybind(Modifiers mods) {
    this->modifiers = mods;
}

Keybind::Keybind(int mods) {
    this->modifiers = static_cast<Modifiers>(mods);
}

Keybind::Keybind(MouseButton btn) {
    this->mouse = btn;

    auto kb = CCDirector::sharedDirector()->getKeyboardDispatcher();

    this->modifiers = 0;
    if (kb->getControlKeyPressed())
        this->modifiers |= this->kmControl;
    if (kb->getCommandKeyPressed())
        this->modifiers |= this->kmCommand;
    if (kb->getShiftKeyPressed())
        this->modifiers |= this->kmShift;
    if (kb->getAltKeyPressed())
        this->modifiers |= this->kmAlt;
}

Keybind::Keybind(MouseButton btn, Modifiers mods) {
    this->mouse = btn;
    this->modifiers = mods;
}

Keybind::Keybind(MouseButton btn, int mods) {
    this->mouse = btn;
    this->modifiers = static_cast<Modifiers>(mods);
}

Keybind::Keybind(DS_Dictionary* dict, int version) {
    // this->key = static_cast<enumKeyCodes>(dict->getIntegerForKey("key"));
    // this->modifiers = dict->getIntegerForKey("modifiers");
    // if (version > 1)
    //     this->mouse = static_cast<decltype(this->mouse)>(
    //         dict->getIntegerForKey("click")
    //     );
}

std::ostream& geode::operator<<(std::ostream& stream, Keybind const& bind) {
    return stream << bind.toString();
}

std::ostream& geode::operator<<(std::ostream& stream, keybind_action_id const& id) {
    return stream << id.m_value;
}


keybind_action_id::keybind_action_id() {
    m_value = "";
}

keybind_action_id::keybind_action_id(const char* val) {
    if (val) {
        m_value = val;
    } else {
        m_value = "";
    }
}

keybind_action_id::keybind_action_id(std::string const& val) {
    *this = val;
}

keybind_action_id::~keybind_action_id() {}

const char* keybind_action_id::c_str() const {
    return m_value.c_str();
}

keybind_action_id keybind_action_id::operator=(std::string const& val) {
    m_value = val;
    return *this;
}

keybind_action_id keybind_action_id::operator=(std::nullptr_t const&) {
    m_value = "";
    return *this;
}

keybind_action_id::operator int() const {
    if (!m_value.size()) return 0;
    return hash(m_value.c_str());
}

size_t keybind_action_id::size() const {
    return m_value.size();
}

keybind_action_id::operator std::string() const {
    return m_value;
}

bool keybind_action_id::operator==(keybind_action_id const& other) const {
    return
        string_utils::toLower(this->m_value) ==
        string_utils::toLower(other.m_value);
}

std::size_t std::hash<Keybind>::operator()(Keybind const& key) const {
    return (key.key << 8) + (key.modifiers << 4) + (key.mouse);
}

std::size_t std::hash<keybind_action_id>::operator()(keybind_action_id const& key) const {
    return std::hash<decltype(key.m_value)>()(key.m_value);
}

