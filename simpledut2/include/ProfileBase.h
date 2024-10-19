#include <string>

class ProfileBase {
  public:
    virtual std::string process(const std::string& params) = 0;
    virtual std::string name() = 0;
    virtual std::string help() = 0;
    virtual ~ProfileBase() = default;
};
