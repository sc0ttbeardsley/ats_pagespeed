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

#include <vector>
#include <cstring>
#include <fstream>
#include <sstream>
#include <map>
#include <tr1/unordered_set>
#include <yaml-cpp/yaml.h>
#include "ats_pagespeed_logging.h"
#include "ats_rewrite_options.h"
#include "ats_rewrite_driver_factory.h"
#include "ats_thread_system.h"
#include "ats_message_handler.h"
#include "ats_pagespeed_configuration.h"

using namespace ats_pagespeed;
using namespace ats_pagespeed::config;
using std::map;
using std::string;
using std::ifstream;

// declared in configuration.h
std::string config::CONFIG_FILE_CMDLINE_ARG = "--pagespeed-config";
string config::pagespeed_config_file;
ListRegexRules config::PagespeedRegexRules; // The rules loaded from config.

/*
 * Follow sequences recursively pulling out all scalars and flattening them into a single sequence.
 */
std::vector<std::string> FlattenScalarSequence(const YAML::Node &node) {
  std::vector<std::string> scalars;

  if (node.Type() == YAML::NodeType::Scalar) {
    scalars.push_back(node.to<std::string>());
    return scalars;
  }

  if (node.Type() != YAML::NodeType::Sequence) {
    return scalars;
  }

  for (YAML::Iterator vals = node.begin(); vals != node.end(); ++vals) {
    if((*vals).Type() == YAML::NodeType::Scalar) {
      string val = (*vals).to<std::string>();
      scalars.push_back(val);
    } else if ((*vals).Type() == YAML::NodeType::Sequence) {
      std::vector<std::string> nested_scalars = FlattenScalarSequence(*vals);
      scalars.insert(scalars.end(), nested_scalars.begin(), nested_scalars.end());
    }
  }

  return scalars;
}

// Parse options structures
void operator >>(const YAML::Node& node, OptionsMap& options) {
  if (node.Type() != YAML::NodeType::Map)
    return;

  for (YAML::Iterator it=node.begin(); it!=node.end(); ++it) {
    std::string name = it.first().to<std::string>();
    std::vector<std::string> scalars = FlattenScalarSequence(it.second());
    for(std::vector<std::string>::iterator si = scalars.begin(); si != scalars.end(); ++si) {
    }
    options[name] = scalars;
  }
}

// Process a RegexRule
void operator >>(const YAML::Node& node, RegexRule& rule) {
  node["UrlRegex"] >> rule.url_regex;
  TS_DEBUG(TAG, "Compiling and optimize rule with url regex: %s", rule.url_regex.c_str());

  if(node.FindValue("CaseSensitive")) {
    node["CaseSensitive"] >> rule.case_sensitive;
  }
  TS_DEBUG(TAG, "Rule will be CaseSensitive: %s", rule.case_sensitive ? "true" : "false");


  if (node.FindValue("MatchMethod")) {
    string match_method;
    node["MatchMethod"] >> match_method;
    rule.match_method =
        (match_method == "PreRemap") ?
            RegexRule::PRE_REMAP :
            RegexRule::POST_REMAP;
  }
  TS_DEBUG(TAG, "Using match method: %s", rule.match_method == RegexRule::POST_REMAP ? "POST_REMAP" : "PRE_REMAP" );

  if(node.FindValue("DisablePagepseed")) {
    node["DisablePagepseed"] >> rule.disable_pagespeed;
  }
  TS_DEBUG(TAG, "Rule will DisablePagespeed: %s", rule.case_sensitive ? "true" : "false");

  // compile the rule
  int pcre_error_offset;
  const char *pcre_error_str;

  // First, the regex string must be compiled.
  int options = PCRE_EXTENDED | ((rule.case_sensitive) ? 0 : PCRE_CASELESS);
  rule.pcre_compiled_regex = pcre_compile(rule.url_regex.c_str(), options, &pcre_error_str, &pcre_error_offset, NULL);

  if(rule.pcre_compiled_regex == NULL) {
    TS_ERROR(TAG, "Could not compile '%s': %s. This rule will be ignored.", rule.url_regex.c_str(), pcre_error_str);
    return;
  }

  rule.pcre_ext = pcre_study(rule.pcre_compiled_regex, 0, &pcre_error_str);
  if(pcre_error_str != NULL) {
    TS_ERROR(TAG, "Could not study '%s': %s. This rule will be ignored.", rule.url_regex.c_str(), pcre_error_str);
    pcre_free(rule.pcre_compiled_regex);
    rule.pcre_compiled_regex = NULL;
    return;
  }

  if(rule.disable_pagespeed) {
    // We dont need any pagespeed options in this situation.
    return;
  }

  OptionsMap map;
  if(node.FindValue("PagespeedOptions")) {
    node["PagespeedOptions"] >> map;
  }

  rule.rule_options->SetOptionsFromOptionsMap(map, global_config.handler, NULL);
  rule.server_context->global_options()->Merge(*rule.rule_options);
  rule.server_context->Init();

}

bool config::readYamlConfigFile(const string &config_file) {
  ifstream confFile(config_file.c_str());
  if (!confFile.good()) {
    TS_ERROR(TAG, "Error while opening config file %s.", config_file.c_str());
    return false;
  }

  YAML::Parser parser(confFile);
  YAML::Node doc;
  if(!parser.GetNextDocument(doc)) {
    TS_ERROR(TAG, "Unable to load document while parsing YAML config.");
    return false;
  }

  TS_DEBUG(TAG, "Loading global options");
  // Parse global options
  OptionsMap map;
  if (doc.FindValue("GlobalPagespeedOptions")) {
    const YAML::Node& global_options_node = doc["GlobalPagespeedOptions"];
    global_options_node >> map;
  }

  global_config.options->SetOptionsFromOptionsMap(map, global_config.handler, global_config.driver_factory);
  global_config.driver_factory->default_options()->Merge(*global_config.options);

  // We need to initialize global stats before we create each ServerContext
  if (global_config.options->statistics_enabled()) {
     global_config.driver_factory->MakeGlobalSharedMemStatistics(
        global_config.options->statistics_logging_enabled(),
        global_config.options->statistics_logging_interval_ms(),
        global_config.options->statistics_logging_file());
  }


  if (!doc.FindValue("Rules")) {
    TS_ERROR(TAG, "Config file %s did not contain a Rules section!", config_file.c_str());
    return false;
  }

  const YAML::Node& rules = doc["Rules"];

  for (unsigned i = 0; i < rules.size(); i++) {
    RegexRule regex_rule;

    TS_DEBUG(TAG, "Processing rule %d of %d", (i + 1), static_cast<int>(rules.size()));
    if (!rules[i].FindValue("UrlRegex")) {
      TS_ERROR(TAG, "Skipping rule %d that is missing UrlRegex.", (i + 1));
      continue;
    }

    rules[i] >> regex_rule;

    bool valid_rule = true;

    // Validate a few things
    string file_cache_path = regex_rule.server_context->config()->file_cache_path();
    TS_DEBUG(TAG, "Using file cache patch (%s)", file_cache_path.c_str());
    if (file_cache_path.empty()) {
       TS_ERROR(TAG, "FileCachePath must be set in the global configs or on every rule.");
       valid_rule = false;
    } else if (!global_config.driver_factory->file_system()->IsDir(file_cache_path.c_str(),
         global_config.handler).is_true()) {
       TS_ERROR(TAG, "FileCachePath must be set to a writeable directory! (%s is not a directory or writeable)", file_cache_path.c_str());
       valid_rule = false;
    }

    if(regex_rule.pcre_compiled_regex != NULL && valid_rule) {
      config::PagespeedRegexRules.push_back(regex_rule);
    }
  }


  return true;
}

bool config::loadConfig(int argc, char *argv[]) {
  //We must extract the --pagespeed-config file from the command line parameters
  bool foundConfigFile = false;
  for (int i = 0; i < argc; ++i) {
    if (strncmp(argv[i], CONFIG_FILE_CMDLINE_ARG.c_str(),
        CONFIG_FILE_CMDLINE_ARG.size()) == 0) {

      // we found the config file argument, now parse it and remove it...
      string config_file = argv[i];
      config_file = config_file.substr(CONFIG_FILE_CMDLINE_ARG.size());
      if (config_file.empty() || config_file.size() == 1 || config_file[0] != '=') {
        // If it was empty it didn't contain the =
        // If it was exactly 1 character it couldn't contain a file name
        // If the first char was not an = it was malformed.
        TS_ERROR(TAG,
            "Command line argument %s was invalid/malformed. Expecting %s=FILENAME.",
            CONFIG_FILE_CMDLINE_ARG.c_str(), CONFIG_FILE_CMDLINE_ARG.c_str());
        break;
      }

      pagespeed_config_file = config_file.substr(1); // skip the =.;
      foundConfigFile = true;
      break;
    }
  }

  if (!foundConfigFile) {
    TS_ERROR(TAG,
        "Unable to continue, no configuration file specified via %s argument in plugin.config!",
        CONFIG_FILE_CMDLINE_ARG.c_str());
    return false;
  } else {
    TS_DEBUG(TAG, "Loading using config file %s", pagespeed_config_file.c_str());
  }

  return readYamlConfigFile(pagespeed_config_file);
}
