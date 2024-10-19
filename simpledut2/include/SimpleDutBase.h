#include <memory>
#include <string>
#include "Profiles.h"

#include <functional>
#include <unordered_map>
#include <unordered_set>

class SimpleDutBase {
  public:
    int loop() {
        std::string command;
        output("SimpleDut v0.1");
        while (true) {
            getCommand(command);
            process(command);
        }
        return 0;
    }

  protected:
    virtual void getCommand(std::string& command) = 0;
    virtual void output(const std::string& message) = 0;

    std::unordered_map<std::string, std::function<void(const std::string&)>> mCommandLookup;
    std::unique_ptr<ProfileBase> mActiveProfile;
    std::unordered_set<std::string> mProfileNames = {"gatt", "beacon"};

    void process(const std::string& command) {
        std::string help =
            "SimpleDut commands:\n"
            "\t- ping: answer's pong\n"
            "\t- help: this help\n"
            "\t- status: show active profile\n"
            "\t- gatt: manage GATT profile";

        if (command.empty()) {
            output("");
            return;
        }

        std::string base = command.substr(0, command.find(' '));
        std::string args = command.substr(command.find(' ') + 1);

        if (base == "ping") {
            output("pong");
            return;
        } else if (base == "help") {
            output(help);
            return;
        } else if (base == "status") {
            if (mActiveProfile) {
                output("active, " + mActiveProfile->name());
            } else {
                output("idle");
            }
            return;
        } else if (mProfileNames.find(base) == mProfileNames.end()) {
            output("Invalid command");
            return;
        } else if (mActiveProfile && mActiveProfile->name() != base) {
            output("Invalid state, active profile is " + mActiveProfile->name());
            return;
        }

        if (!mActiveProfile) {
            if (args.find("start") != std::string::npos) {
                if (base == "gatt") {
                    mActiveProfile = std::make_unique<Gatt>();
                }
            } else {
                output("Use start command to activate the profile.");
            }
        }

        if (mActiveProfile) {
            output(mActiveProfile->process(args));

            if (args == "stop") {
                mActiveProfile = nullptr;
            }
        }
    }
};