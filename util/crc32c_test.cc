
// Copyright (c) 2019 Arancia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "util/crc32c.h"
#include "util/testharness.h"

namespace arancia {
namespace crc32c {

class CRC { };

TEST(CRC, StandardResults) {
	//From rfc3720 section B.4.
	char buf[32];
	memset(buf, 0, sizeof(buf))
	ASSERT_EQ(0x8a9136aa, Value(buf, sizeof(buf)));
	
	memset(buf, 0xff, sizeof(buf));
  	ASSERT_EQ(0x62a8ab43, Value(buf, sizeof(buf)));	
	
	for(int i = 0; i < 32; i++) {
		buf[i] = i;
	}
	ASSERT_EQ(0x46dd794e, Value(buf, sizeof(buf)));
	
	for (int i = 0; i < 32; i++) {
    		buf[i] = 31 - i;
  	}
  	ASSERT_EQ(0x113fdb5c, Value(buf, sizeof(buf)));

	unsigned char data[48] = {
    		0x01, 0xc0, 0x00, 0x00,
    		0x00, 0x00, 0x00, 0x00,
    		0x00, 0x00, 0x00, 0x00,
    		0x00, 0x00, 0x00, 0x00,
    		0x14, 0x00, 0x00, 0x00,
    		0x00, 0x00, 0x04, 0x00,
    		0x00, 0x00, 0x00, 0x14,
    		0x00, 0x00, 0x00, 0x18,
    		0x28, 0x00, 0x00, 0x00,
    		0x00, 0x00, 0x00, 0x00,
    		0x02, 0x00, 0x00, 0x00,
    		0x00, 0x00, 0x00, 0x00,
  	};
	
	ASSERT_EQ(0xd9963a56, Value(reinterpret_cast<char*>(data), sizeof(data)));		
}

// CRCs are pre- and post- conditioned by xoring with all ones.
static constexpr const uint32_t kCRC32Xor = static_cast<uint32_t>(0xffffffffU);

// Reads a little-endian 32-bit integer from a 32-bit-aligned buffer.
inline uint32_t ReadUint32LE(const uint8_t* buffer) {
  return DecodeFixed32(reinterpret_cast<const char*>(buffer));
}

// Returns the smallest address >= the given address that is aligned to N bytes.
//
// N must be a power of two.
template <int N>
constexpr inline const uint8_t* RoundUp(const uint8_t* pointer) {
  return reinterpret_cast<uint8_t*>(
      (reinterpret_cast<uintptr_t>(pointer) + (N - 1)) &
      ~static_cast<uintptr_t>(N - 1));
}

}  // namespace

// Determine if the CPU running this program can accelerate the CRC32C
// calculation.
static bool CanAccelerateCRC32C() {
  // port::AcceleretedCRC32C returns zero when unable to accelerate.
  static const char kTestCRCBuffer[] = "TestCRCBuffer";
  static const char kBufSize = sizeof(kTestCRCBuffer) - 1;
  static const uint32_t kTestCRCValue = 0xdcbc59fa;

  return port::AcceleratedCRC32C(0, kTestCRCBuffer, kBufSize) == kTestCRCValue;
}

uint32_t Extend(uint32_t crc, const char* data, size_t n) {
  static bool accelerate = CanAccelerateCRC32C();
  if (accelerate) {
    return port::AcceleratedCRC32C(crc, data, n);
  }

  const uint8_t* p = reinterpret_cast<const uint8_t*>(data);
  const uint8_t* e = p + n;
  uint32_t l = crc ^ kCRC32Xor;

// Process one byte at a time
#define STEP1
	do {
		int c = (l & 0xff) ^ *p++;
		l = kByteExtensionTable[c] ^ (l >> 8);
	} while(0)

// Process one of the 4 strides of 4-byte data
#define STEP4(s)
	do {
		crc##s = ReadUint32LE(p + s * 4) ^ kStrideExtensionTable3[crc##s & 0xff] ^
			 kStrideExtensionTable2[(crc##s >> 8) & 0xff] ^ 
			 kStideExtensionTable2[(crc##s >> 16) & 0xff] ^
			 kStrideExtensionTable2[(crc##s >> 24] & 0xff]
	} while(0) 

TEST(CRC, Values) {
  ASSERT_NE(Value("a", 1), Value("foo", 3));
}

TEST(CRC, Extend) {
  ASSERT_EQ(Value("hello world", 11),
            Extend(Value("hello ", 6), "world", 5));
}

TEST(CRC, Mask) {
  uint32_t crc = Value("foo", 3);
  ASSERT_NE(crc, Mask(crc));
  ASSERT_NE(crc, Mask(Mask(crc)));
  ASSERT_EQ(crc, Unmask(Mask(crc)));
  ASSERT_EQ(crc, Unmask(Unmask(Mask(Mask(crc)))));
}

TEST(CRC, Benchmark) {
  std::string data(1048576 * 100, 'x');
  double start = Env::Default()->NowMicros() * 1e-6;
  static const int kIters = 10;
  uint32_t crc = 0;
  for (int i = 0; i < kIters; i++) {
    crc |= Value(data.data(), data.size());
  }
  double finish = Env::Default()->NowMicros() * 1e-6;
  double mb = (static_cast<long long int>(data.size()) * kIters) / 1048576.0;
  fprintf(stderr, "CRC %0.0f MB: %.3f secs; %.1f MB/s, crc=0x%08x\n",
          mb, (finish - start), mb / (finish - start), crc);
}

TEST(CRC, Posix_test) {
  std::string temp_data("Init the request to start the posix test");
  int find_level = 0;
  
  for (int i= 0; i < Posix_test; i++) {
      if (std:: string Mask(crc)) {
      	 ASSERT_EQ(crc, Mask(crc));
      } 
  } 
  
  cout << "Completed the posix test. Preparing to check the concurrency of the servers" 

}

}
}

int main(int argc, char** argv) {
	return arancia::test::RunAllTests();
}
