// Copyright 2015, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Björn Buchhold (buchhold@informatik.uni-freiburg.de)

#include "./ResultTable.h"
#include <cassert>

// _____________________________________________________________________________
ResultTable::ResultTable()
    : _nofColumns(0),
      _sortedBy(0),
      _varSizeData(),
      _fixedSizeData(nullptr),
      _status(ResultTable::OTHER) {}

// _____________________________________________________________________________
ResultTable::ResultTable(const ResultTable& other)
    : _nofColumns(other._nofColumns),
      _sortedBy(other._sortedBy),
      _varSizeData(other._varSizeData),
      _fixedSizeData(nullptr),
      _status(other._status) {
  if (other._nofColumns <= 5) {
    if (other._nofColumns == 1) {
      _fixedSizeData = new vector<array<Id, 1>>;
      *static_cast<vector<array<Id, 1>>*>(_fixedSizeData) =
          *static_cast<vector<array<Id, 1>>*>(other._fixedSizeData);
    }
    if (other._nofColumns == 2) {
      _fixedSizeData = new vector<array<Id, 2>>;
      *static_cast<vector<array<Id, 2>>*>(_fixedSizeData) =
          *static_cast<vector<array<Id, 2>>*>(other._fixedSizeData);
    }
    if (other._nofColumns == 3) {
      _fixedSizeData = new vector<array<Id, 3>>;
      *static_cast<vector<array<Id, 3>>*>(_fixedSizeData) =
          *static_cast<vector<array<Id, 3>>*>(other._fixedSizeData);
    }
    if (other._nofColumns == 4) {
      _fixedSizeData = new vector<array<Id, 4>>;
      *static_cast<vector<array<Id, 4>>*>(_fixedSizeData) =
          *static_cast<vector<array<Id, 4>>*>(other._fixedSizeData);
    }
    if (other._nofColumns == 5) {
      _fixedSizeData = new vector<array<Id, 5>>;
      *static_cast<vector<array<Id, 5>>*>(_fixedSizeData) =
          *static_cast<vector<array<Id, 5>>*>(other._fixedSizeData);
    }
  } else {
    assert(!other._fixedSizeData);
    _fixedSizeData = nullptr;
  }
}

// _____________________________________________________________________________
void ResultTable::clear() {
  if (_fixedSizeData) {
    if (_nofColumns == 1) {
      vector<array<Id, 1>>* ptr =
          static_cast<vector<array<Id, 1>>*>(_fixedSizeData);
      delete ptr;
    }
    if (_nofColumns == 2) {
      vector<array<Id, 2>>* ptr =
          static_cast<vector<array<Id, 2>>*>(_fixedSizeData);
      delete ptr;
    }
    if (_nofColumns == 3) {
      vector<array<Id, 3>>* ptr =
          static_cast<vector<array<Id, 3>>*>(_fixedSizeData);
      delete ptr;
    }
    if (_nofColumns == 4) {
      vector<array<Id, 4>>* ptr =
          static_cast<vector<array<Id, 4>>*>(_fixedSizeData);
      delete ptr;
    }
    if (_nofColumns == 5) {
      vector<array<Id, 5>>* ptr =
          static_cast<vector<array<Id, 5>>*>(_fixedSizeData);
      delete ptr;
    }
  }
  _fixedSizeData = nullptr;
  _varSizeData.clear();
  _status = OTHER;
}

// _____________________________________________________________________________
ResultTable::~ResultTable() { clear(); }

// _____________________________________________________________________________
string ResultTable::asDebugString() const {
  std::ostringstream os;
  os << "First (up to) 5 rows of result with size:\n";
  if (_fixedSizeData) {
    if (_nofColumns == 1) {
      vector<array<Id, 1>>* ptr =
          static_cast<vector<array<Id, 1>>*>(_fixedSizeData);
      for (size_t i = 0; i < std::min<size_t>(5, ptr->size()); ++i) {
        os << (*ptr)[i][0] << '\n';
      }
    }
    if (_nofColumns == 2) {
      vector<array<Id, 2>>* ptr =
          static_cast<vector<array<Id, 2>>*>(_fixedSizeData);
      for (size_t i = 0; i < std::min<size_t>(5, ptr->size()); ++i) {
        os << (*ptr)[i][0] << '\t' << (*ptr)[i][1] << '\n';
      }
    }
    if (_nofColumns == 3) {
      vector<array<Id, 3>>* ptr =
          static_cast<vector<array<Id, 3>>*>(_fixedSizeData);
      for (size_t i = 0; i < std::min<size_t>(5, ptr->size()); ++i) {
        os << (*ptr)[i][0] << '\t' << (*ptr)[i][1] << '\t' << (*ptr)[i][2]
           << '\n';
      }
    }
    if (_nofColumns == 4) {
      vector<array<Id, 4>>* ptr =
          static_cast<vector<array<Id, 4>>*>(_fixedSizeData);
      for (size_t i = 0; i < std::min<size_t>(5, ptr->size()); ++i) {
        os << (*ptr)[i][0] << '\t' << (*ptr)[i][1] << '\t' << (*ptr)[i][2]
           << '\t' << (*ptr)[i][3] << '\n';
      }
    }
    if (_nofColumns == 5) {
      vector<array<Id, 5>>* ptr =
          static_cast<vector<array<Id, 5>>*>(_fixedSizeData);
      for (size_t i = 0; i < std::min<size_t>(5, ptr->size()); ++i) {
        os << (*ptr)[i][0] << '\t' << (*ptr)[i][1] << '\t' << (*ptr)[i][2]
           << '\t' << (*ptr)[i][3] << '\t' << (*ptr)[i][4] << '\n';
      }
    }
  } else {
    for (size_t i = 0; i < std::min<size_t>(5, _varSizeData.size()); ++i) {
      for (size_t j = 0; j < _varSizeData[i].size(); ++j) {
        os << _varSizeData[i][j] << '\t';
      }
      os << '\n';
    }
  }
  return os.str();
}

// _____________________________________________________________________________
size_t ResultTable::size() const {
  size_t rv = 0;
  if (_fixedSizeData) {
    if (_nofColumns == 1) {
      rv = static_cast<vector<array<Id, 1>>*>(_fixedSizeData)->size();
    }
    if (_nofColumns == 2) {
      rv = static_cast<vector<array<Id, 2>>*>(_fixedSizeData)->size();
    }
    if (_nofColumns == 3) {
      rv = static_cast<vector<array<Id, 3>>*>(_fixedSizeData)->size();
    }
    if (_nofColumns == 4) {
      rv = static_cast<vector<array<Id, 4>>*>(_fixedSizeData)->size();
    }
    if (_nofColumns == 5) {
      rv = static_cast<vector<array<Id, 5>>*>(_fixedSizeData)->size();
    }
  } else {
    rv = _varSizeData.size();
  }
  return rv;
}
