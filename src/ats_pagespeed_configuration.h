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
#ifndef ATS_PAGESPEED_CONFIGURATION_H_
#define ATS_PAGESPEED_CONFIGURATION_H_

#include <set>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>
#include <pcre.h>
#include "ats_pagespeed_logging.h"
#include "ats_rewrite_driver_factory.h"
#include "ats_rewrite_options.h"
#include "ats_server_context.h"
#include "ats_message_handler.h"

#include "net/instaweb/automatic/public/proxy_fetch.h"


namespace ats_pagespeed {
namespace config {

extern std::string CONFIG_FILE_CMDLINE_ARG;
extern std::string pagespeed_config_file;

bool readYamlConfigFile(const std::string &);
bool loadConfig(int argc, char *argv[]);

struct RegexRule {

  enum MatchMethod {
    PRE_REMAP = 0,
    POST_REMAP
  };

  std::string url_regex;
  MatchMethod match_method;
  bool case_sensitive;
  bool override_content_type_restriction;
  bool disable_pagespeed;
  pcre *pcre_compiled_regex;
  pcre_extra *pcre_ext;

  // Location configuration
  AtsMessageHandler *handler;
  AtsRewriteOptions *rule_options;
  AtsServerContext *server_context;
  //net_instaweb::ProxyFetchFactory proxy_fetch_factory;

  RegexRule() : match_method(POST_REMAP), case_sensitive(false),
      override_content_type_restriction(false), disable_pagespeed(false), pcre_compiled_regex(NULL), pcre_ext(NULL),
      handler(NULL) {
    LOG_ENTRY

    rule_options = global_config.options->Clone();
    server_context = new AtsServerContext(global_config.driver_factory);
    //server_context->InitWorkersAndDecodingDriver();
    //proxy_fetch_factory = new net_instaweb::ProxyFetchFactory(server_context);
  }

  ~RegexRule() {
    LOG_ENTRY
    // Not cleaning up here is intentional, these will live for the life of the program
    // so rather than dealing with shared pointers because this struct will live in a
    // standard library container we'll just let this leak for the life of the program
    // since it's not really a leak anyway.
  }
};

typedef std::list<RegexRule> ListRegexRules;
extern ListRegexRules PagespeedRegexRules;

/*
 * This predicate will return true if one string contains another
 */
class ExistsSubstring: public std::unary_function<std::string, bool> {
private:
  std::string haystack_;
public:
  ExistsSubstring(std::string haystack) :
      haystack_(haystack) {
  }

  bool operator()(const std::string &needle) const {
    return (haystack_.find(needle) != std::string::npos);
  }
};

/*
 * This predicate will return true if two strings are equal
 */
class EqualString: public std::unary_function<std::string, bool> {
private:
  std::string target_;
public:
  EqualString(std::string target) :
      target_(target) {
  }

  bool operator()(const std::string &value) const {
    return (target_ == value);
  }
};

/*
 * This predicate will return true if the url matches the regex
 */
class MatchesRegexRule: public std::unary_function<RegexRule, bool> {
private:
  std::string url_pre_remap_;
  std::string url_post_remap_;
public:
  MatchesRegexRule(const std::string &url_pre_remap, const std::string &url_post_remap) :
    url_pre_remap_(url_pre_remap), url_post_remap_(url_post_remap) {  }

  bool operator()(const RegexRule &rule) const {
    if (rule.pcre_compiled_regex == NULL)
      return false;

    if (rule.match_method == RegexRule::PRE_REMAP) {
      return isMatch(rule, url_pre_remap_);
    } else if (rule.match_method == RegexRule::POST_REMAP) {
      return isMatch(rule, url_post_remap_);
    }

    return false;
  }

  /*
   * Attempt to match the Regex with the given url
   */
  bool isMatch(const RegexRule &rule, const std::string &url) const {
      int pcre_exec_result =  pcre_exec(const_cast<const pcre *>(rule.pcre_compiled_regex),
                                        const_cast<const pcre_extra *>(rule.pcre_ext),
                                        url.c_str(),
                                        url.length(),  // length of string
                                        0,             // Start looking at this point
                                        0,             // OPTIONS
                                        NULL,          // sub string vector (we don't care)
                                        0);            // Length of sub string vector

      // Report what happened in the pcre_exec call..
      if(pcre_exec_result < 0) {
        if (pcre_exec_result == PCRE_ERROR_NOMATCH) {
          TS_DEBUG(TAG, "Regex Failed Match: '%s' failed to match '%s'.", rule.url_regex.c_str(), url.c_str());
        } else {
          TS_ERROR(TAG,"Regex '%s' Error. error code: %d", rule.url_regex.c_str(), pcre_exec_result);
        }
        return false;
      } else {
        TS_DEBUG(TAG, "Regex Match: '%s' matched on url '%s'.", rule.url_regex.c_str(), url.c_str());
        return true;
      }
  }
};

}
}

#endif /* ATS_PAGESPEED_CONFIGURATION_H_ */
