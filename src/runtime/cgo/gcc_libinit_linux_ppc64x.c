// Copyright 2015 The Go Authors.  All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// +build cgo

// TODO: see issue #10410
// +build linux
// +build ppc64 ppc64le

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libcgo.h"
#include "libcgo_unix.h"

void
x_cgo_sys_thread_create(void* (*func)(void*), void* arg) {
	fprintf(stderr, "x_cgo_sys_thread_create not implemented");
	abort();
}

void
_cgo_wait_runtime_init_done() {
	// TODO(spetrovic): implement this method.
}

void
x_cgo_notify_runtime_init_done(void* dummy) {
	// TODO(spetrovic): implement this method.
}

// _cgo_try_pthread_create retries pthread_create if it fails with
// EAGAIN.
int
_cgo_try_pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*pfn)(void*), void* arg) {
	int tries;
	int err;
	struct timespec ts;

	for (tries = 0; tries < 20; tries++) {
		err = pthread_create(thread, attr, pfn, arg);
		if (err != EAGAIN) {
			return err;
		}
		ts.tv_sec = 0;
		ts.tv_nsec = (tries + 1) * 1000 * 1000; // Milliseconds.
		nanosleep(&ts, nil);
	}
	return EAGAIN;
}
