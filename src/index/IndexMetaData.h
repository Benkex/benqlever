// Copyright 2015, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Björn Buchhold (buchhold@informatik.uni-freiburg.de)
#pragma once

#include <array>
#include <vector>
#include <utility>
#include <unordered_map>

#include "../global/Id.h"
#include "../util/File.h"


using std::array;
using std::vector;
using std::pair;
using std::unordered_map;

// Check IndexLayout.txt for explanations (expected comments).
// Removed comments here so that not two places had to be kept up-to-date.

static const uint64_t IS_FUNCTIONAL_MASK = 0x0100000000000000;
static const uint64_t HAS_BLOCKS_MASK = 0x0200000000000000;
static const uint64_t NOF_ELEMENTS_MASK = 0x00FFFFFFFFFFFFFF;

class BlockMetaData {
public:
  BlockMetaData() : _firstLhs(0), _startOffset(0) {}

  BlockMetaData(Id lhs, off_t start) : _firstLhs(lhs), _startOffset(start) {}

  Id _firstLhs;
  off_t _startOffset;
};

class FullRelationMetaData {
public:
  FullRelationMetaData();

  FullRelationMetaData(Id relId, off_t startFullIndex, size_t nofElements,
                       bool isFunctional, bool hasBlocks);

  size_t getNofBytesForFulltextIndex() const;


  // Returns true if there is exactly one RHS for each LHS in the relation
  bool isFunctional() const;

  bool hasBlocks() const;

  // Handle the fact that those properties are encoded in the same
  // size_t as the number of elements.
  void setIsFunctional(bool isFunctional);

  void setHasBlocks(bool hasBlocks);

  size_t getNofElements() const;


  // Restores meta data from raw memory.
  // Needed when registering an index on startup.
  FullRelationMetaData& createFromByteBuffer(unsigned char* buffer);

  // The size this object will require when serialized to file.
  size_t bytesRequired() const;

  off_t getStartOfLhs() const;

  Id _relId;
  off_t _startFullIndex;

  friend ad_utility::File& operator<<(ad_utility::File& f,
                                      const FullRelationMetaData& rmd);

private:
  // first byte: type
  // other 7 bytes: the nof elements.
  uint64_t _typeAndNofElements;
};

inline ad_utility::File& operator<<(ad_utility::File& f,
                                    const FullRelationMetaData& rmd) {
  f.write(&rmd._relId, sizeof(rmd._relId));
  f.write(&rmd._startFullIndex, sizeof(rmd._startFullIndex));
  f.write(&rmd._typeAndNofElements, sizeof(rmd._typeAndNofElements));
  return f;
}

class BlockBasedRelationMetaData {
public:
  BlockBasedRelationMetaData();

  BlockBasedRelationMetaData(off_t startRhs, off_t offsetAfter,
                             const vector<BlockMetaData>& blocks);

  // The size this object will require when serialized to file.
  size_t bytesRequired() const;

  // Restores meta data from raw memory.
  // Needed when registering an index on startup.
  BlockBasedRelationMetaData& createFromByteBuffer(unsigned char* buffer);

  // Takes a LHS and returns the offset into the file at which the
  // corresponding block can be read as well as the nof bytes to read.
  // If the relation is functional, this offset will be located in the
  // range of the FullIndex, otherwise it will be reference into the lhs list.
  // Reading nofBytes from the offset will yield a block which contains
  // the desired lhs if such a block exists at all.
  // If the lhs does not exists at all, this will only be clear after reading
  // said block.
  pair<off_t, size_t> getBlockStartAndNofBytesForLhs(Id lhs) const;

  // Gets the block after the one returned by getBlockStartAndNofBytesForLhs.
  // This is necessary for finding rhs upper bounds for the last item in a
  // block.
  // If this is equal to the block returned by getBlockStartAndNofBytesForLhs,
  // it means it is the last block and the offsetAfter can be used.
  pair<off_t, size_t> getFollowBlockForLhs(Id lhs) const;

  off_t _startRhs;
  off_t _offsetAfter;
  vector<BlockMetaData> _blocks;
};

inline ad_utility::File& operator<<(ad_utility::File& f,
                                    const BlockBasedRelationMetaData& rmd) {
  f.write(&rmd._startRhs, sizeof(rmd._startRhs));
  f.write(&rmd._offsetAfter, sizeof(rmd._offsetAfter));
  auto nofBlocks = rmd._blocks.size();
  f.write(&nofBlocks, sizeof(nofBlocks));
  f.write(rmd._blocks.data(), nofBlocks * sizeof(BlockMetaData));
  return f;
}

class RelationMetaData {
public:

  explicit RelationMetaData(const FullRelationMetaData& rmdPairs) :
      _rmdPairs(rmdPairs),
      _rmdBlocks(nullptr) {}

  off_t getStartOfLhs() const {
    return _rmdPairs.getStartOfLhs();
  }

  size_t getNofBytesForFulltextIndex() const {
    return _rmdPairs.getNofBytesForFulltextIndex();
  }

  // Returns true if there is exactly one RHS for each LHS in the relation
  bool isFunctional() const {
    return _rmdPairs.isFunctional();
  }

  bool hasBlocks() const {
    return _rmdPairs.hasBlocks();
  }

  size_t getNofElements() const {
    return _rmdPairs.getNofElements();
  }

  const FullRelationMetaData& _rmdPairs;
  const BlockBasedRelationMetaData* _rmdBlocks;
};

inline ad_utility::File& operator<<(ad_utility::File& f,
                                    const RelationMetaData& rmd) {
  f << rmd._rmdPairs << *rmd._rmdBlocks;
  return f;
}

class IndexMetaData {
public:
  IndexMetaData();

  void
  add(const FullRelationMetaData& rmd, const BlockBasedRelationMetaData& bRmd);

  off_t getOffsetAfter() const;

  const RelationMetaData getRmd(Id relId) const;

  void createFromByteBuffer(unsigned char* buf);

  bool relationExists(Id relId) const;

  string statistics() const;

private:
  off_t _offsetAfter;
  unordered_map<Id, FullRelationMetaData> _data;
  unordered_map<Id, BlockBasedRelationMetaData> _blockData;

  friend ad_utility::File& operator<<(ad_utility::File& f,
                                      const IndexMetaData& rmd);

  size_t getNofBlocksForRelation(const Id relId) const;

  size_t getTotalBytesForRelation(const FullRelationMetaData& frmd) const;
};

ad_utility::File& operator<<(ad_utility::File& f, const IndexMetaData& imd);