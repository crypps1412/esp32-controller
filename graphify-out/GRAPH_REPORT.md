# Graph Report - esp32_controller  (2026-10-20)

## Corpus Check
- 16 files · ~6,195 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 5 file(s) not represented in the graph (top: (none) 3, .cmd 1, .defaults 1)

## Summary
- 98 nodes · 146 edges · 17 communities (8 shown, 9 thin omitted)
- Extraction: 92% EXTRACTED · 8% INFERRED · 0% AMBIGUOUS · INFERRED: 12 edges (avg confidence: 0.88)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `943e0271`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- Graphify Usage Rules (CLAUDE.md)
- main.c
- esp_err_t
- joystick.c
- add_bt_scan_result
- ble_gap_event_handler
- bt_gap_event_handler
- deinit_low_level
- esp_hid_scan

## God Nodes (most connected - your core abstractions)
1. `ble_gap_event_handler()` - 7 edges
2. `find_scan_result()` - 6 edges
3. `add_bt_scan_result()` - 6 edges
4. `add_ble_scan_result()` - 5 edges
5. `handle_bt_device_result()` - 5 edges
6. `bt_gap_event_handler()` - 5 edges
7. `init_low_level()` - 5 edges
8. `esp_hid_scan()` - 5 edges
9. `Graphify Usage Rules (CLAUDE.md)` - 5 edges
10. `handle_ble_device_result()` - 4 edges

## Surprising Connections (you probably didn't know these)
- `esp32_controller CMake Project` --references--> `main Component (idf_component_register)`  [INFERRED]
  CMakeLists.txt → main/CMakeLists.txt
- `main Component (idf_component_register)` --implements--> `ESP-IDF Build System (project.cmake)`  [INFERRED]
  main/CMakeLists.txt → CMakeLists.txt
- `app_main()` --calls--> `joystick_init()`  [INFERRED]
  main/main.c → main/joystick.c
- `ble_gap_event_handler()` --calls--> `ble_hid_task_start_up()`  [INFERRED]
  main/esp_hid_gap.c → main/main.c
- `app_main()` --calls--> `esp_hid_ble_gap_adv_init()`  [INFERRED]
  main/main.c → main/esp_hid_gap.c

## Import Cycles
- None detected.

## Hyperedges (group relationships)
- **ESP-IDF Project Build Chain** — cmakelists_esp32_controller_project, cmakelists_esp_idf_build_system, main_cmakelists_main_component, main_main [INFERRED 0.85]

## Communities (17 total, 9 thin omitted)

### Community 0 - "Graphify Usage Rules (CLAUDE.md)"
Cohesion: 0.47
Nodes (4): GRAPH_REPORT.md, Graphify Usage Rules (CLAUDE.md), Graphify Knowledge Graph (graphify-out), graphify-out/wiki/index.md

### Community 1 - "main.c"
Cohesion: 0.18
Nodes (5): esp32_controller CMake Project, ESP-IDF Build System (project.cmake), main Component (idf_component_register), esp_hid_ble_gap_adv_start(), hidd_event_callback()

### Community 2 - "esp_err_t"
Cohesion: 0.48
Nodes (6): esp_hid_ble_gap_adv_init(), esp_hid_gap_init(), init_ble_gap(), init_bt_gap(), init_low_level(), app_main()

### Community 8 - "joystick.c"
Cohesion: 0.19
Nodes (6): joystick_init(), joystick_read_axes(), joystick_read_button(), read_avg(), scale_axis(), joystick_task()

### Community 10 - "add_bt_scan_result"
Cohesion: 0.27
Nodes (7): add_ble_scan_result(), add_bt_scan_result(), ble_addr_type_str(), find_scan_result(), handle_ble_device_result(), handle_bt_device_result(), print_uuid()

### Community 12 - "ble_gap_event_handler"
Cohesion: 0.25
Nodes (5): ble_gap_event_handler(), ble_gap_evt_str(), esp_ble_key_type_str(), nimble_hid_gap_event(), ble_hid_task_start_up()

### Community 14 - "deinit_low_level"
Cohesion: 0.50
Nodes (3): deinit_low_level(), esp_hid_gap_deinit(), esp_hid_scan_results_free()

### Community 15 - "esp_hid_scan"
Cohesion: 0.67
Nodes (3): esp_hid_scan(), start_ble_scan(), start_bt_scan()

## Knowledge Gaps
- **2 isolated node(s):** `GRAPH_REPORT.md`, `graphify-out/wiki/index.md`
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 41 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **9 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `app_main()` connect `esp_err_t` to `joystick.c`, `main.c`?**
  _High betweenness centrality (0.051) - this node is a cross-community bridge._
- **What connects `GRAPH_REPORT.md`, `graphify-out/wiki/index.md` to the rest of the system?**
  _2 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Why does `ble_gap_event_handler()` connect `ble_gap_event_handler` to `add_bt_scan_result`, `esp_hid_gap.c`?**
  _High betweenness centrality (0.043) - this node is a cross-community bridge._
- **Why does `joystick_init()` connect `joystick.c` to `esp_err_t`?**
  _High betweenness centrality (0.037) - this node is a cross-community bridge._