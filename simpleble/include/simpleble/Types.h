#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "external/kvn_bytearray.h"
#include "external/kvn_uuid.h"

/**
 * @file Types.h
 * @brief Defines types and enumerations used throughout the SimpleBLE library.
 */

namespace SimpleBLE {

using BluetoothAddress = std::string;

/**
 * @typedef BluetoothUUID
 * @brief Provides an homogenous interface to build and manage bluetooth UUIDs.
 */
using BluetoothUUID = kvn::uuid;

/**
 * @typedef ByteArray
 * @brief Represents a byte array using kvn::bytearray from the external library.
 */
using ByteArray = kvn::bytearray;

enum class OperatingSystem {
    WINDOWS,
    MACOS,
    LINUX,
};

// TODO: Add to_string functions for all enums.
enum BluetoothAddressType : int32_t { PUBLIC = 0, RANDOM = 1, UNSPECIFIED = 2 };

}  // namespace SimpleBLE
