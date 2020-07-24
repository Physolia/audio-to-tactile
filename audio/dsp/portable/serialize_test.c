/* Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "audio/dsp/portable/serialize.h"

#include <stdlib.h>

#include "audio/dsp/portable/logging.h"

/* Test serialization / deserialization of uint16_t. */
void TestU16() {
  uint8_t buffer[sizeof(uint16_t)];
  LittleEndianWriteS16(0x0102, buffer);
  CHECK(buffer[0] == 0x02);
  CHECK(buffer[1] == 0x01);

  BigEndianWriteS16(0x0102, buffer);
  CHECK(buffer[0] == 0x01);
  CHECK(buffer[1] == 0x02);

  /* Check round trip of serializing and deserializing some test values. */
  static const uint16_t kTestValues[] = {0, 1, 2, 300, 50000, UINT16_MAX};
  int i;
  for (i = 0; i < sizeof(kTestValues) / sizeof(*kTestValues); ++i) {
    const uint16_t value = kTestValues[i];
    LittleEndianWriteU16(value, buffer);
    CHECK(LittleEndianReadU16(buffer) == value);

    BigEndianWriteU16(value, buffer);
    CHECK(BigEndianReadU16(buffer) == value);
  }
}

/* Test serialization / deserialization of uint32_t. */
void TestU32() {
  uint8_t buffer[sizeof(uint32_t)];
  LittleEndianWriteU32(UINT32_C(0x01020304), buffer);
  CHECK(buffer[0] == 0x04);
  CHECK(buffer[1] == 0x03);
  CHECK(buffer[2] == 0x02);
  CHECK(buffer[3] == 0x01);

  BigEndianWriteU32(UINT32_C(0x01020304), buffer);
  CHECK(buffer[0] == 0x01);
  CHECK(buffer[1] == 0x02);
  CHECK(buffer[2] == 0x03);
  CHECK(buffer[3] == 0x04);

  /* Check round trip of serializing and deserializing some test values. */
  static const uint32_t kTestValues[] = {0, 1, 2, 250000UL, UINT32_MAX};
  int i;
  for (i = 0; i < sizeof(kTestValues) / sizeof(*kTestValues); ++i) {
    const uint32_t value = kTestValues[i];
    LittleEndianWriteU32(value, buffer);
    CHECK(LittleEndianReadU32(buffer) == value);

    BigEndianWriteU32(value, buffer);
    CHECK(BigEndianReadU32(buffer) == value);
  }
}

/* Test serialization / deserialization of uint64_t. */
void TestU64() {
  uint8_t buffer[sizeof(uint64_t)];
  LittleEndianWriteU64(UINT64_C(0x0102030405060708), buffer);
  CHECK(buffer[0] == 0x08);
  CHECK(buffer[1] == 0x07);
  CHECK(buffer[2] == 0x06);
  CHECK(buffer[3] == 0x05);
  CHECK(buffer[4] == 0x04);
  CHECK(buffer[5] == 0x03);
  CHECK(buffer[6] == 0x02);
  CHECK(buffer[7] == 0x01);

  BigEndianWriteU64(UINT64_C(0x0102030405060708), buffer);
  CHECK(buffer[0] == 0x01);
  CHECK(buffer[1] == 0x02);
  CHECK(buffer[2] == 0x03);
  CHECK(buffer[3] == 0x04);
  CHECK(buffer[4] == 0x05);
  CHECK(buffer[5] == 0x06);
  CHECK(buffer[6] == 0x07);
  CHECK(buffer[7] == 0x08);

  /* Check round trip of serializing and deserializing some test values. */
  static const uint64_t kTestValues[] = {0, 1, UINT64_C(92) << 55, UINT64_MAX};
  int i;
  for (i = 0; i < sizeof(kTestValues) / sizeof(*kTestValues); ++i) {
    const uint64_t value = kTestValues[i];
    LittleEndianWriteU64(value, buffer);
    CHECK(LittleEndianReadU64(buffer) == value);

    BigEndianWriteU64(value, buffer);
    CHECK(BigEndianReadU64(buffer) == value);
  }
}

/* Test serialization / deserialization of int16_t. */
void TestS16() {
  uint8_t buffer[sizeof(int16_t)];
  LittleEndianWriteS16(-2, buffer);  /* -2 in two's complement is 0xfffe. */
  CHECK(buffer[0] == 0xfe);
  CHECK(buffer[1] == 0xff);

  BigEndianWriteS16(-2, buffer);
  CHECK(buffer[0] == 0xff);
  CHECK(buffer[1] == 0xfe);

  /* Check round trip of serializing and deserializing some test values. */
  static const int16_t kTestValues[] = {0, 1, INT16_MAX, -1, -25000, INT16_MIN};
  int i;
  for (i = 0; i < sizeof(kTestValues) / sizeof(*kTestValues); ++i) {
    const int16_t value = kTestValues[i];
    LittleEndianWriteS16(value, buffer);
    CHECK(LittleEndianReadS16(buffer) == value);

    BigEndianWriteS16(value, buffer);
    CHECK(BigEndianReadS16(buffer) == value);
  }
}

/* Test serialization / deserialization of int32_t. */
void TestS32() {
  uint8_t buffer[sizeof(int32_t)];
  LittleEndianWriteS32(-2, buffer);
  CHECK(buffer[0] == 0xfe);
  CHECK(buffer[1] == 0xff);
  CHECK(buffer[2] == 0xff);
  CHECK(buffer[3] == 0xff);

  BigEndianWriteS32(-2, buffer);
  CHECK(buffer[0] == 0xff);
  CHECK(buffer[1] == 0xff);
  CHECK(buffer[2] == 0xff);
  CHECK(buffer[3] == 0xfe);

  /* Check round trip of serializing and deserializing some test values. */
  static const int32_t kTestValues[] = {0, 1, INT32_MAX, -1, -25000, INT32_MIN};
  int i;
  for (i = 0; i < sizeof(kTestValues) / sizeof(*kTestValues); ++i) {
    const int32_t value = kTestValues[i];
    LittleEndianWriteS32(value, buffer);
    CHECK(LittleEndianReadS32(buffer) == value);

    BigEndianWriteS32(value, buffer);
    CHECK(BigEndianReadS32(buffer) == value);
  }
}

/* Test serialization / deserialization of int64_t. */
void TestS64() {
  uint8_t buffer[sizeof(int64_t)];
  LittleEndianWriteS64(-2, buffer);
  CHECK(buffer[0] == 0xfe);
  CHECK(buffer[1] == 0xff);
  CHECK(buffer[2] == 0xff);
  CHECK(buffer[3] == 0xff);
  CHECK(buffer[4] == 0xff);
  CHECK(buffer[5] == 0xff);
  CHECK(buffer[6] == 0xff);
  CHECK(buffer[7] == 0xff);

  BigEndianWriteS64(-2, buffer);
  CHECK(buffer[0] == 0xff);
  CHECK(buffer[1] == 0xff);
  CHECK(buffer[2] == 0xff);
  CHECK(buffer[3] == 0xff);
  CHECK(buffer[4] == 0xff);
  CHECK(buffer[5] == 0xff);
  CHECK(buffer[6] == 0xff);
  CHECK(buffer[7] == 0xfe);

  /* Check round trip of serializing and deserializing some test values. */
  static const int64_t kTestValues[] = {0, 1, INT64_MAX, -1, -25000, INT64_MIN};
  int i;
  for (i = 0; i < sizeof(kTestValues) / sizeof(*kTestValues); ++i) {
    const int64_t value = kTestValues[i];
    LittleEndianWriteS64(value, buffer);
    CHECK(LittleEndianReadS64(buffer) == value);

    BigEndianWriteS64(value, buffer);
    CHECK(BigEndianReadS64(buffer) == value);
  }
}

/* Test serialization / deserialization of 32-bit float. */
void TestF32() {
  uint8_t buffer[sizeof(float)];
  /* Check round trip of serializing and deserializing some test values. */
  static const float kTestValues[] = {0.0f, 3.71f, -3.71f, 2.5e-6f, 2.5e6f};
  int i;
  for (i = 0; i < sizeof(kTestValues) / sizeof(*kTestValues); ++i) {
    const float value = kTestValues[i];
    LittleEndianWriteF32(value, buffer);
    CHECK(LittleEndianReadF32(buffer) == value);

    BigEndianWriteF32(value, buffer);
    CHECK(BigEndianReadF32(buffer) == value);
  }
}

/* Test serialization / deserialization of 64-bit double. */
void TestF64() {
  uint8_t buffer[sizeof(double)];
  /* Check round trip of serializing and deserializing some test values. */
  static const double kTestValues[] = {0.0, 3.71, -3.71, 2.5e-6, 2.5e6};
  int i;
  for (i = 0; i < sizeof(kTestValues) / sizeof(*kTestValues); ++i) {
    const double value = kTestValues[i];
    LittleEndianWriteF64(value, buffer);
    CHECK(LittleEndianReadF64(buffer) == value);

    BigEndianWriteF64(value, buffer);
    CHECK(BigEndianReadF64(buffer) == value);
  }
}

int main(int argc, char** argv) {
  srand(0);
  TestU16();
  TestU32();
  TestU64();
  TestS16();
  TestS32();
  TestS64();
  TestF32();
  TestF64();

  puts("PASS");
  return EXIT_SUCCESS;
}
