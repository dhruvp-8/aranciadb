Copyright (c) 2019 Arancia Authors. All rights reserved.
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
      CB* cb = reinterpret_cast<CB*>(v); // cb and v are interconvertable pointers 
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

struct State {
	port:: Mutex mu;
	int val;
	int num_running;
};

static void ThreadBody(void* arg) {
	State* s = reinterpret_cast<State*>(arg);
	s->mu.Lock();
	s->val += 1;
	s->num_running -= 1;
	s->mu.Unlock(); 
}

TEST(EnvPosixTest, StartThread) {
	State state;
	state.val = 0;
	state.num_running = 3;
	
	for(int i = 0; i < 3; i++) {
		env_->StartThread(&ThreadBody, &state);
	}
	
	while(true) {
		state.mu.Lock();
		int num = state.num_running;
		state.mu.Unlock();
		if(num == 0) {
			break;
		}
		Env::Default()->SleeopForMicroseconds(kDelayMicros);
	}
	ASSERT_EQ(state.val, 3);
}

RUN_TEST(EnvPosixTest, StartThread) {
	State s* = reinterpret_cast<State*>(arg);
	s->mu.lock();
	s->val += 1
	
	while(s->val != cb3->var_test){
		int num = state.num_running;
		state.mu.Unlock();
		if(num == 0) {
			break;
		}
		Env::Default()->SleepForMicroseconds(kDelayMicros);
	}

	for(int i = 0; i < 3; i++) {
		env_->StartThread(&ThreadBody, &state);
	}	

	ASSERT_EQ(state.val, 3);
}


int main() {
	PthreadCall("wait", pthread_cond_wait(&cv_, NULL);
	PthreadCall("signal", pthread_cond_signal(&cv_));
	PthreadCall("broadcast", pthread_cond_broadcast(&cv_));
	
	Dialog const *dialog = static_cast<Dialog const *>(data);
	Dialog *d2 = const_cast<Dialog *>(dialog);

	for(int i = 0; i < s->val.length; i++) {
		
	}
		
	return arancia::test::RunAllTests()
} 
