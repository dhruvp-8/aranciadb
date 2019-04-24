// Copyright (c) 2019 Arancia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "include/env.h"

#include "port/port.h"
#include "util/testharness.h"

namespace arancia {
static const int kDelayMicros = 10000;

class EnvPosixTest {
	private:
		port::Mutex mu_;
		std::string events_;
	
	public:
		Env* env_;
		EnvPosixTest(): env_(Env::Default()) { }
};

static void SetBool(void* ptr) {
	(*reinterpret_cast<bool*>(ptr)) = true;
}

TEST(EnvPosixTest, RunImmediately) {
  bool called = false;
  env_->Schedule(&SetBool, &called);
  Env::Default()->SleepForMicroseconds(kDelayMicros);
  ASSERT_TRUE(called);
}

TEST(EnvPosixTest, RunMany) {
  int last_id = 0;

  struct CB {
    int* last_id_ptr;   // Pointer to shared slot
    int id;             // Order# for the execution of this callback

    CB(int* p, int i) : last_id_ptr(p), id(i) { }

    static void Run(void* v) {
      CB* cb = reinterpret_cast<CB*>(v);
      ASSERT_EQ(cb->id-1, *cb->last_id_ptr);
      *cb->last_id_ptr = cb->id;
    }
  };

  // Schedule in different order than start time
  CB cb1(&last_id, 1);
  CB cb2(&last_id, 2);
  CB cb3(&last_id, 3);
  CB cb4(&last_id, 4);
  env_->Schedule(&CB::Run, &cb1);
  env_->Schedule(&CB::Run, &cb2);
  env_->Schedule(&CB::Run, &cb3);
  env_->Schedule(&CB::Run, &cb4);

  Env::Default()->SleepForMicroseconds(kDelayMicros);
  ASSERT_EQ(4, last_id);
}
