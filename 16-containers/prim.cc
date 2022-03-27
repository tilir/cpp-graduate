//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Prim algorithm example
//
//------------------------------------------------------------------------------

#include <algorithm>
#include <cassert>
#include <climits>
#include <iostream>
#include <iterator>
#include <list>
#include <queue>
#include <vector>

using VertexTy = int;
using WeightTy = int;
using PairTy = std::pair<VertexTy, WeightTy>;

struct Edge {
  VertexTy u, v;
  WeightTy w;
};

// simplistic graph, just for experiment here, pick your better one!
class Graph {
  int V_;
  using AdjList = std::list<PairTy>;
  std::vector<AdjList> Adj_;

public:
  Graph(int V) : V_(V), Adj_(V) {}
  void addEdge(Edge e) {
    Adj_[e.u].push_back(std::make_pair(e.v, e.w));
    Adj_[e.v].push_back(std::make_pair(e.u, e.w));
  }
  auto &adj(VertexTy v) { return Adj_[v]; }
  int size() const { return V_; }
};

// required interface for Prim:
// first(G) -- starting vertex in graph
// weight(G, e) -- weight of edge e
// tip(G, e) -- end of edge e
// adjacent(G, v) -- range of adjacent vertices

VertexTy first(const Graph &G) {
  if (G.size() == 0)
    throw std::runtime_error("Graph empty");
  return 0;
}

WeightTy weight(const Graph &G, const PairTy &p) { return p.second; }

VertexTy tip(const Graph &G, const PairTy &p) { return p.first; }

auto &adjacent(Graph &G, VertexTy v) { return G.adj(v); }

// prim algorithm
template <typename GraphTy> auto prim(GraphTy G) {
  std::priority_queue<PairTy> pq;
  pq.push(std::make_pair(first(G), 0));

  std::vector<bool> mst(G.size(), false);
  std::vector<WeightTy> key(G.size(), INT_MIN);
  std::vector<VertexTy> parent(G.size(), -1);

  while (!pq.empty()) {
    auto elt = pq.top().first;
    pq.pop();
    if (mst[elt])
      continue;
    mst[elt] = true;
    std::cout << "Elem: " << elt << std::endl;
    for (auto e : adjacent(G, elt)) {
      auto w = weight(G, e);
      auto v = tip(G, e);
      std::cout << "Adj: " << v << std::endl;
      if (!mst[v] && key[v] < w) {
        key[v] = w;
        pq.push(std::make_pair(v, w));
        parent[v] = elt;
      }
    }
  }
  return parent;
}

std::vector<Edge> Elist = {{0, 1, 4},  {0, 7, 8},  {1, 2, 8}, {1, 7, 11},
                           {2, 3, 7},  {2, 8, 2},  {2, 5, 4}, {3, 4, 9},
                           {3, 5, 14}, {4, 5, 10}, {5, 6, 2}, {6, 7, 1},
                           {6, 8, 6},  {7, 8, 7}};

int main() {
  Graph g(9);
  std::cout << "Input:\n";
  for (auto &&e : Elist) {
    std::cout << e.u << " -- " << e.v << std::endl;
    g.addEdge(e);
  }
  auto spanning = prim(g);
  assert(spanning.size() == 9);
  std::cout << "Answer:\n";
  for (int i = 0; i < 9; ++i)
    if (spanning[i] != -1)
      std::cout << spanning[i] << " -- " << i << std::endl;
}