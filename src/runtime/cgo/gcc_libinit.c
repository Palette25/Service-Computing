// Copyright 2015 The Go Authors.  All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// +build cgo
// +build darwin dragonfly freebsd linux netbsd solaris
// +build !ppc64,!ppc64le

#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strerror
#include <time.h>
#include "libcgo.h"
#include "libcgo_unix.h"

static pthread_cond_t runtime_init_cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t runtime_init_mu = PTHREAD_MUTEX_INITIALIZER;
static int runtime_init_done;

void
x_cgo_sys_thread_create(void* (*func)(void*), void* arg) {
	pthread_t p;
	int err = _cgo_try_pthread_create(&p, NULL, func, arg);
	if (err != 0) {
		fprintf(stderr, "pthread_create failed: %s", strerror(err));
		abort();
	}
}

void
_cgo_wait_runtime_init_done() {
	pthread_mutex_lock(&runtime_init_mu);
	while (runtime_init_done == 0) {
		pthread_cond_wait(&runtime_init_cond, &runtime_init_mu);
	}
	pthread_mutex_unlock(&runtime_init_mu);
}

void
x_cgo_notify_runtime_init_done(void* dummy) {
	pthread_mutex_lock(&runtime_init_mu);
	runtime_init_done = 1;
	pthread_cond_broadcast(&runtime_init_cond);
	pthread_mutex_unlock(&runtime_init_mu);
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
