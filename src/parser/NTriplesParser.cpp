// Copyright 2015, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Björn Buchhold (buchhold@informatik.uni-freiburg.de)

#include <cassert>
#include <iostream>
#include "../util/Exception.h"
#include "./NTriplesParser.h"
#include "../util/Log.h"

using ad_utility::lstrip;

// _____________________________________________________________________________
NTriplesParser::NTriplesParser(const string& file) : _in(file) {
}

// _____________________________________________________________________________
NTriplesParser::~NTriplesParser() {
  _in.close();
}

// _____________________________________________________________________________
bool NTriplesParser::getLine(array<string, 3>& res) {
  string line;
  if (std::getline(_in, line)) {
    size_t i = 0;
    while (i < line.size() && (line[i] == ' ' || line[i] == '\t')) { ++i; }
    assert(i < line.size());
    size_t j = i + 1;
    while (j < line.size() && line[j] != '\t' && line[j] != ' ') { ++j; }
    assert(j < line.size());
    if (!(line[i] == '<' && line[j - 1] == '>')) {
      AD_THROW(ad_semsearch::Exception::BAD_INPUT, "Illegal URI in : " + line);
    }
    res[0] = line.substr(i, j - i);
    i = j;
    while (i < line.size() && (line[i] == ' ' || line[i] == '\t')) { ++i; }
    assert(i < line.size());
    j = i + 1;
    while (j < line.size() && line[j] != '\t' && line[j] != ' ') { ++j; }
    if (!(line[i] == '<' && line[j - 1] == '>')) {
      AD_THROW(ad_semsearch::Exception::BAD_INPUT, "Illegal URI in : " + line);
    }
    res[1] = line.substr(i, j - i);
    i = j;
    while (i < line.size() && (line[i] == ' ' || line[i] == '\t')) { ++i; }
    assert(i < line.size());
    if (line[i] == '<') {
      // URI
      j = line.find('>', i + 1);
      if (j == string::npos) {
        AD_THROW(ad_semsearch::Exception::BAD_INPUT,
                 "Illegal URI in : " + line);
      }
      ++j;
    } else {
      // Literal
      j = line.find('\"', i + 1);
      bool escape = false;
      while (j < line.size()) {
        if (!escape && line[j] == '\\') {
          escape = true;
        } else if (!escape && line[j] == '\"') {
          break;
        } else {
          escape = false;
        }
        ++j;
      }

      if (j == line.size()) {
        AD_THROW(ad_semsearch::Exception::BAD_INPUT,
                 "Illegal literal in : " + line);
      }
      ++j;
      while (j < line.size() && line[j] != ' ' && line[j] != '\t') { ++j; }
    }
    assert(j < line.size());
    res[2] = line.substr(i, j - i);
    if (!(line[j] == ' ' || line[j] == '\t')) {
      AD_THROW(ad_semsearch::Exception::BAD_INPUT,
               "Object not followed by space in : " + line);
    }
    return true;
  }
  return false;
}
