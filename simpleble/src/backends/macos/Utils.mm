#import "Utils.h"

#import <fmt/core.h>

SimpleBLE::BluetoothUUID uuidToSimpleBLE(CBUUID* uuid) {
    std::string uuid_raw = [[[uuid UUIDString] lowercaseString] UTF8String];

    return SimpleBLE::BluetoothUUID(uuid_raw);
}
