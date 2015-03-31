// Copyright 2011, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Björn Buchhold <buchholb>

#include <gtest/gtest.h>
#include <cstdio>
#include "../src/index/Vocabulary.h"


TEST(VocabularyTest, getIdForWordTest) {
  Vocabulary v;
  v.push_back("a");
  v.push_back("ab");
  v.push_back("ba");
  v.push_back("car");
  Id id;
  ASSERT_TRUE(v.getId("ba", &id));
  ASSERT_EQ(Id(2), id);
  ASSERT_TRUE(v.getId("a", &id));
  ASSERT_EQ(Id(0), id);
  ASSERT_FALSE(v.getId("foo", &id));
};

TEST(VocabularyTest, getIdRangeForFullTextPrefixTest) {
  Vocabulary v;
  v.push_back("wordA0");
  v.push_back("wordA1");
  v.push_back("wordB2");
  v.push_back("wordB3");
  v.push_back("wordB4");

  Id word0 = 0;
  IdRange retVal;
  // Match exactly one
  ASSERT_TRUE(v.getIdRangeForFullTextPrefix("wordA1*", &retVal));
  ASSERT_EQ(word0 + 1, retVal._first);
  ASSERT_EQ(word0 + 1, retVal._last);

  // Match all
  ASSERT_TRUE(v.getIdRangeForFullTextPrefix("word*", &retVal));
  ASSERT_EQ(word0, retVal._first);
  ASSERT_EQ(word0 + 4, retVal._last);

  // Match first two
  ASSERT_TRUE(v.getIdRangeForFullTextPrefix("wordA*", &retVal));
  ASSERT_EQ(word0, retVal._first);
  ASSERT_EQ(word0 + 1, retVal._last);

  // Match last three
  ASSERT_TRUE(v.getIdRangeForFullTextPrefix("wordB*", &retVal));
  ASSERT_EQ(word0 + 2, retVal._first);
  ASSERT_EQ(word0 + 4, retVal._last);

  ASSERT_FALSE(v.getIdRangeForFullTextPrefix("foo*", &retVal));
}

TEST(VocabularyTest, readWriteTest) {
  Vocabulary v;
  v.push_back("wordA0");
  v.push_back("wordA1");
  v.push_back("wordB2");
  v.push_back("wordB3");
  v.push_back("wordB4");
  ASSERT_EQ(size_t(5), v.size());

  v.writeToFile("_testtmp_vocfile");
  v.push_back("foo");
  ASSERT_EQ(size_t(6), v.size());
  v.readFromFile("_testtmp_vocfile");
  ASSERT_EQ(size_t(5), v.size());
  remove("_testtmp_vocfile");
}

TEST(VocabularyTest, createFromSetTest) {
  std::unordered_set<string> s;
  s.insert("a");
  s.insert("ab");
  s.insert("ba");
  s.insert("car");
  Vocabulary v;
  v.createFromSet(s);
  Id id;
  ASSERT_TRUE(v.getId("ba", &id));
  ASSERT_EQ(Id(2), id);
  ASSERT_TRUE(v.getId("a", &id));
  ASSERT_EQ(Id(0), id);
  ASSERT_FALSE(v.getId("foo", &id));
};


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

