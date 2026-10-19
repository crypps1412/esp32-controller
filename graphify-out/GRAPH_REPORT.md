# Graph Report - esp32_controller  (2026-10-19)

## Corpus Check
- 12 files · ~1,335 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 2 file(s) not represented in the graph (top: (none) 2)

## Summary
- 17 nodes · 13 edges · 8 communities (2 shown, 6 thin omitted)
- Extraction: 69% EXTRACTED · 31% INFERRED · 0% AMBIGUOUS · INFERRED: 4 edges (avg confidence: 0.95)
- Token cost: 0 input · 0 output

## Community Hubs (Navigation)
- Graphify Usage Rules (CLAUDE.md)
- main Component (idf_component_register)

## God Nodes (most connected - your core abstractions)
1. `Graphify Usage Rules (CLAUDE.md)` - 5 edges
2. `Graphify Knowledge Graph (graphify-out)` - 3 edges
3. `main Component (idf_component_register)` - 3 edges
4. `graphify query / path / explain commands` - 2 edges
5. `graphify update (post-modification refresh)` - 2 edges
6. `esp32_controller CMake Project` - 2 edges
7. `ESP-IDF Build System (project.cmake)` - 2 edges
8. `GRAPH_REPORT.md` - 1 edges
9. `graphify-out/wiki/index.md` - 1 edges

## Surprising Connections (you probably didn't know these)
- `esp32_controller CMake Project` --references--> `main Component (idf_component_register)`  [INFERRED]
  CMakeLists.txt → main/CMakeLists.txt
- `main Component (idf_component_register)` --implements--> `ESP-IDF Build System (project.cmake)`  [INFERRED]
  main/CMakeLists.txt → CMakeLists.txt

## Import Cycles
- None detected.

## Hyperedges (group relationships)
- **ESP-IDF Project Build Chain** — cmakelists_esp32_controller_project, cmakelists_esp_idf_build_system, main_cmakelists_main_component, main_main [INFERRED 0.85]

## Communities (8 total, 6 thin omitted)

### Community 0 - "Graphify Usage Rules (CLAUDE.md)"
Cohesion: 0.47
Nodes (4): GRAPH_REPORT.md, Graphify Usage Rules (CLAUDE.md), Graphify Knowledge Graph (graphify-out), graphify-out/wiki/index.md

### Community 1 - "main Component (idf_component_register)"
Cohesion: 1.00
Nodes (3): esp32_controller CMake Project, ESP-IDF Build System (project.cmake), main Component (idf_component_register)

## Knowledge Gaps
- **2 isolated node(s):** `GRAPH_REPORT.md`, `graphify-out/wiki/index.md`
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 9 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **6 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `main Component (idf_component_register)` connect `main Component (idf_component_register)` to `main.c`?**
  _High betweenness centrality (0.050) - this node is a cross-community bridge._
- **Are the 2 inferred relationships involving `Graphify Knowledge Graph (graphify-out)` (e.g. with `graphify query / path / explain commands` and `graphify update (post-modification refresh)`) actually correct?**
  _`Graphify Knowledge Graph (graphify-out)` has 2 INFERRED edges - model-reasoned connections that need verification._
- **What connects `GRAPH_REPORT.md`, `graphify-out/wiki/index.md` to the rest of the system?**
  _2 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Are the 2 inferred relationships involving `main Component (idf_component_register)` (e.g. with `esp32_controller CMake Project` and `ESP-IDF Build System (project.cmake)`) actually correct?**
  _`main Component (idf_component_register)` has 2 INFERRED edges - model-reasoned connections that need verification._