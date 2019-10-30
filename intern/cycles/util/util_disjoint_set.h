/*
 * Copyright 2011-2013 Blender Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __UTIL_DISJOINT_SET_H__
#define __UTIL_DISJOINT_SET_H__

#include <utility>

CCL_NAMESPACE_BEGIN

class DisjointSet {
 private:
  int *parent, *rank, size;

 public:
  DisjointSet(int size)
  {
    this->size = size;
    parent = new int[size];
    rank = new int[size];
    initializeSet();
  }

  void initializeSet()
  {
    for (int i = 0; i < size; i++) {
      parent[i] = i;
      rank[i] = 0;
    }
  }

  int find(int x)
  {
    if (parent[x] != x) {
      parent[x] = find(parent[x]);
    }
    return parent[x];
  }

  void join(int x, int y)
  {
    int xRoot = find(x);
    int yRoot = find(y);

    if (xRoot == yRoot) {
      return;
    }

    if (rank[xRoot] < rank[yRoot]) {
      std::swap(xRoot, yRoot);
    }
    parent[yRoot] = xRoot;

    if (rank[xRoot] == rank[yRoot]) {
      rank[xRoot]++;
    }
  }
};

CCL_NAMESPACE_END

#endif /* __UTIL_DISJOINT_SET_H__ */
