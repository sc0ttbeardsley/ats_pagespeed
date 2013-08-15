/*
 * Copyright (c) 2013 LinkedIn Corp. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of the license at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the
 * License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied.
 *
 */

#pragma once
#ifndef ATS_PAGESPEED_LOGGING_H_
#define ATS_PAGESPEED_LOGGING_H_

#include <atscppapi/Logger.h>
#define TAG "ats_pagespeed"

// blow away the macros in Logger.h
#undef LOG_DEBUG
#undef LOG_ERROR

#define LOG_DEBUG(fmt, ...) \
  do { TS_DEBUG(TAG, fmt,## __VA_ARGS__); } while (false)

#define LOG_ERROR(fmt, ...) \
  do { TS_ERROR(TAG, fmt,## __VA_ARGS__); } while (false)

#define LOG_ENTRY LOG_DEBUG("LOG_ENTRY");

#endif /* ATS_PAGESPEED_LOGGING_H_ */
