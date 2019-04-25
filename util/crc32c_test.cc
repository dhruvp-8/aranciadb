
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

}
}

int main(int argc, char** argv) {
	return arancia::test::RunAllTests();
}
