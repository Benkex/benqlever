// Copyright 2015, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Björn Buchhold (buchhold@informatik.uni-freiburg.de)
#pragma once

#include <list>
#include <utility>
#include <vector>

#include "./Operation.h"
#include "./IndexScan.h"
#include "./QueryExecutionTree.h"

using std::list;

using std::pair;
using std::vector;

class OrderBy : public Operation {
 public:
  virtual size_t getResultWidth() const;

 public:

  OrderBy(QueryExecutionContext* qec,
          std::shared_ptr<QueryExecutionTree> subtree,
          const vector<pair<size_t, bool>>& sortIndices);

  virtual string asString() const;

  virtual size_t resultSortedOn() const {
    return std::numeric_limits<size_t>::max();
  }

  virtual void setTextLimit(size_t limit) {
    _subtree->setTextLimit(limit);
  }

  virtual size_t getSizeEstimate() {
    return _subtree->getSizeEstimate();
  }

  virtual float getMultiplicity(size_t col) {
    return _subtree->getMultiplicity(col);
  }

  virtual size_t getCostEstimate() {
    size_t size = getSizeEstimate();
    size_t logSize = std::max(size_t(1), static_cast<size_t>(logb(
        static_cast<double>(getSizeEstimate()))));
    size_t nlogn = size * logSize;
    size_t subcost = _subtree->getCostEstimate();
    return nlogn + subcost;
  }

  virtual bool knownEmptyResult() {
    return _subtree->knownEmptyResult();
  }

 private:
  std::shared_ptr<QueryExecutionTree> _subtree;
  vector<pair<size_t, bool>> _sortIndices;

  virtual void computeResult(ResultTable* result) const;
};
