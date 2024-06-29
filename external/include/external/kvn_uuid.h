#ifndef KVN_UUID_H
#define KVN_UUID_H

#include <algorithm>
#include <iostream>
#include <regex>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <string>

namespace kvn {

class uuid {
  public:
    uuid() : uuid_("") {}
    uuid(std::string_view str) {
        if (str.length() == 4) {
            uuid_ = "0000" + std::string(str) + "-0000-1000-8000-00805f9b34fb";
        } else {
            uuid_ = str;
        }
        toLower();
        if (!isValid()) throw std::invalid_argument("Invalid UUID format");
    }
    uuid(const char * str) : uuid(std::string_view(str)) {}
    uuid(std::string str) : uuid(std::string_view(str)) {}
    uuid(uint16_t uuid16bit) : uuid(_toHex(uuid16bit)) {}

    const std::string str() const { return uuid_; }
    const char* c_str() const { return uuid_.c_str(); }
    bool operator<(const uuid& other) const { return uuid_ < other.uuid_; }
    bool operator==(const uuid& other) const { return uuid_ == other.uuid_; }
    bool operator!=(const uuid& other) const { return uuid_ != other.uuid_; }
    operator std::string() const {
        return uuid_;
    }
    friend std::ostream& operator<<(std::ostream& os, const uuid& uuid) {
        os << uuid.uuid_;
        return os;
    }
  private:
    std::string uuid_;

    std::string _toHex(uint16_t uuid16bit) {
        std::stringstream ss;
        ss << std::hex << std::setw(4) << std::setfill('0') << uuid16bit;
        return ss.str();
    }

    void toLower() { std::transform(uuid_.begin(), uuid_.end(), uuid_.begin(), ::tolower); }

    bool isValid() const {
        // Regular expression for matching UUID format
        std::regex pattern("^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$");
        return std::regex_match(uuid_, pattern);
    }
};

}  // namespace kvn
#endif  // KVN_UUID_H
