// Copyright 2015, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Björn Buchhold (buchhold@informatik.uni-freiburg.de)
#pragma once

#include <vector>
#include "../parser/ParsedQuery.h"
#include "QueryExecutionTree.h"

using std::vector;

class QueryPlanner {
public:
  explicit QueryPlanner(QueryExecutionContext *qec);

  QueryExecutionTree createExecutionTree(const ParsedQuery& pq) const;

  class TripleGraph {
  public:

    TripleGraph();

    TripleGraph(const TripleGraph& other);

    TripleGraph& operator=(const TripleGraph& other);

    TripleGraph(const TripleGraph& other, vector<size_t> keepNodes);


    class Node {
    public:
      Node(size_t id, const SparqlTriple& t) : _id(id), _triple(t),
                                               _variables(), _cvar(),
                                               _wordPart() {
        if (isVariable(t._s)) { _variables.insert(t._s); }
        if (isVariable(t._p)) { _variables.insert(t._p); }
        if (isVariable(t._o)) { _variables.insert(t._o); }
      }

      Node(size_t id, const string& cvar, const string& wordPart,
           const vector<SparqlTriple>& trips) :
          _id(id), _triple(cvar,
                           IN_CONTEXT_RELATION,
                           wordPart),
          _variables(), _cvar(cvar), _wordPart(wordPart) {
        _variables.insert(cvar);
        for (const auto& t: trips) {
          if (isVariable(t._s)) { _variables.insert(t._s); }
          if (isVariable(t._p)) { _variables.insert(t._p); }
          if (isVariable(t._o)) { _variables.insert(t._o); }
        }
      }

      Node(const Node& other) = default;

      Node& operator=(const Node& other) = default;

      size_t _id;
      SparqlTriple _triple;
      std::set<string> _variables;
      string _cvar;
      string _wordPart;
    };

    string asString() const;

    bool isTextNode(size_t i) const;

    vector<vector<size_t>> _adjLists;
    std::unordered_map<size_t, Node *> _nodeMap;
    std::list<TripleGraph::Node> _nodeStorage;

    unordered_map<string, vector<size_t>> identifyTextCliques() const;


    vector<size_t> bfsLeaveOut(size_t startNode,
                               unordered_set<size_t> leaveOut) const;

    void collapseTextCliques();

    bool isPureTextQuery();

  private:
    vector<pair<TripleGraph, vector<SparqlFilter>>> splitAtContextVars(
        const vector<SparqlFilter>& origFilters,
        unordered_map<string, vector<size_t>>& contextVarTotextNodes) const;

    vector<SparqlFilter> pickFilters(
        const vector<SparqlFilter>& origFilters,
        const vector<size_t>& nodes) const;
  };

  class SubtreePlan {
  public:
    explicit SubtreePlan(QueryExecutionContext *qec) : _qet(qec) { }

    QueryExecutionTree _qet;
    std::unordered_set<size_t> _idsOfIncludedNodes;
    std::unordered_set<size_t> _idsOfIncludedFilters;

    size_t getCostEstimate() const;

    size_t getSizeEstimate() const;
  };

  TripleGraph createTripleGraph(const ParsedQuery& query) const;


  static unordered_map<string, size_t> createVariableColumnsMapForTextOperation(
      const string& contextVar,
      const string& entityVar,
      const unordered_set<string>& freeVars,
      const vector<pair<QueryExecutionTree, size_t>>& subtrees);

  static unordered_map<string, size_t> createVariableColumnsMapForTextOperation(
      const string& contextVar,
      const string& entityVar,
      const vector<pair<QueryExecutionTree, size_t>>& subtrees) {
    return createVariableColumnsMapForTextOperation(
        contextVar, entityVar, unordered_set<string>(), subtrees);
  };

  static unordered_map<string, size_t> createVariableColumnsMapForTextOperation(
      const string& contextVar,
      const string& entityVar) {
    return createVariableColumnsMapForTextOperation(
        contextVar, entityVar,
        vector<pair<QueryExecutionTree, size_t>>());
  }

  static unordered_map<string, size_t> createVariableColumnsMapForTextOperation(
      const string& contextVar,
      const string& entityVar,
      const unordered_set<string>& freeVars) {
    return createVariableColumnsMapForTextOperation(
        contextVar, entityVar, freeVars,
        vector<pair<QueryExecutionTree, size_t>>());
  };

private:
  QueryExecutionContext *_qec;

  static bool isVariable(const string& elem);

  static bool isWords(const string& elem);

  void getVarTripleMap(const ParsedQuery& pq,
                       unordered_map<string, vector<SparqlTriple>>& varToTrip,
                       unordered_set<string>& contextVars) const;

  vector<SubtreePlan> seedWithScansAndText(const TripleGraph& tg) const;

  vector<SubtreePlan> merge(const vector<SubtreePlan>& a,
                            const vector<SubtreePlan>& b,
                            const TripleGraph& tg) const;

  vector<SubtreePlan> getOrderByRow(
      const ParsedQuery& pq,
      const vector<vector<SubtreePlan>>& dpTab) const;

  bool connected(const SubtreePlan& a, const SubtreePlan& b,
                 const TripleGraph& graph) const;

  vector<array<size_t, 2>> getJoinColumns(
      const SubtreePlan& a, const SubtreePlan& b) const;

  string getPruningKey(const SubtreePlan& plan, size_t orderedOnCol) const;

  void applyFiltersIfPossible(vector<SubtreePlan>& row,
                              const vector<SparqlFilter>& filters) const;

  vector<vector<SubtreePlan>> fillDpTab(const TripleGraph& graph,
                                        const vector<SparqlFilter>& fs) const;

  SubtreePlan pureTextQuery(const TripleGraph& tg) const;

  size_t getTextLimit(const string& textLimitString) const;

  SubtreePlan getTextLeafPlan(const TripleGraph::Node& node) const;
};

