#include <gtest/gtest.h>
#include "external/kvn_uuid.h"

using namespace kvn;

TEST(UUIDTest, StringConstructorValidUUID) {
    std::string valid_uuid = "123e4567-e89b-12d3-a456-426614174000";
    uuid uuid(valid_uuid);
    EXPECT_EQ(uuid.str(), valid_uuid);
}

TEST(UUIDTest, ShortStringUUIDFromString) {
    kvn::uuid short_uuid("1234");
    EXPECT_EQ(short_uuid.str(), "00001234-0000-1000-8000-00805f9b34fb");
}

TEST(UUIDTest, ShortStringUUIDFromInt) {
    kvn::uuid short_uuid(0xDEAD);
    EXPECT_EQ(short_uuid.str(), "0000dead-0000-1000-8000-00805f9b34fb");
}

TEST(UUIDTest, StringConstructorInvalidUUID) {
    std::string invalid_uuid = "invalid-uuid";
    EXPECT_THROW(uuid uuid(invalid_uuid), std::invalid_argument);
}

TEST(UUIDTest, CStringConstructorValidUUID) {
    const char* valid_uuid = "123e4567-e89b-12d3-a456-426614174000";
    uuid uuid(valid_uuid);
    EXPECT_STREQ(uuid.c_str(), valid_uuid);
}

TEST(UUIDTest, CStringConstructorInvalidUUID) {
    const char* invalid_uuid = "invalid-uuid";
    EXPECT_THROW(uuid uuid(invalid_uuid), std::invalid_argument);
}

TEST(UUIDTest, EqualityOperator) {
    uuid uuid1("123e4567-e89b-12d3-a456-426614174000");
    uuid uuid2("123e4567-e89b-12d3-a456-426614174000");
    EXPECT_TRUE(uuid1 == uuid2);
}

TEST(UUIDTest, InequalityOperator) {
    uuid uuid1("123e4567-e89b-12d3-a456-426614174000");
    uuid uuid2("123e4567-e89b-12d3-a456-426614174001");
    EXPECT_TRUE(uuid1 != uuid2);
}

TEST(UUIDTest, LessThanOperator) {
    uuid uuid1("123e4567-e89b-12d3-a456-426614173000");
    uuid uuid2("123e4567-e89b-12d3-a456-426614174000");
    EXPECT_TRUE(uuid1 < uuid2);
}
