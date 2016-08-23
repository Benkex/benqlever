// Copyright 2015, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Björn Buchhold (buchhold@informatik.uni-freiburg.de)
#pragma once

static const int STXXL_MEMORY_TO_USE = 1024 * 1024 * 1024;
static const int STXXL_DISK_SIZE_INDEX_BUILDER = 300000;
static const int STXXL_DISK_SIZE_INDEX_TEST = 10;

static const size_t NOF_SUBTREES_TO_CACHE = 50;
static const size_t MAX_NOF_ROWS_IN_RESULT = 100000;
static const size_t MIN_WORD_PREFIX_SIZE = 4;
static const char PREFIX_CHAR = '*';

static const size_t BUFFER_SIZE_RELATION_SIZE = 1000 * 1000 * 1000;
static const size_t BUFFER_SIZE_DOCSFILE_LINE = 1024 * 1024 * 100;
static const size_t DISTINCT_LHS_PER_BLOCK = 10 * 1000;
static const size_t USE_BLOCKS_INDEX_SIZE_TRESHOLD = 20 * 1000;

static const size_t IN_CONTEXT_CARDINALITY_ESTIMATE = 1000 * 1000 * 1000;

static const char IN_CONTEXT_RELATION[] = "<in-context>";
static const char IN_CONTEXT_RELATION_NS[] = ":in-context";
static const char HAS_CONTEXT_RELATION[] = "<contains-context>";
static const char HAS_CONTEXT_RELATION_NS[] = ":contains-context";

static const char VALUE_PREFIX[] = ":v:";
static const char VALUE_DATE_PREFIX[] = ":v:date:";
static const char VALUE_FLOAT_PREFIX[] = ":v:float:";
static const char XSD_DATETIME_SUFFIX[] = "^^<http://www.w3.org/2001/XMLSchema#dateTime>";
static const char XSD_INT_SUFFIX[] = "^^<http://www.w3.org/2001/XMLSchema#int>";
static const char XSD_FLOAT_SUFFIX[] = "^^<http://www.w3.org/2001/XMLSchema#float>";
static const char VALUE_DATE_TIME_SEPARATOR[] = "T";
static const int DEFAULT_NOF_VALUE_INTEGER_DIGITS = 50;
static const int DEFAULT_NOF_VALUE_EXPONENT_DIGITS = 20;
static const int DEFAULT_NOF_VALUE_MANTISSA_DIGITS = 30;
static const int DEFAULT_NOF_DATE_YEAR_DIGITS = 19;
