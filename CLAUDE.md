## graphify

This project has a knowledge graph at graphify-out/ with god nodes, community structure, and cross-file relationships.

Rules:
- For codebase questions, first run `graphify query "<question>"` when graphify-out/graph.json exists. Use `graphify path "<A>" "<B>"` for relationships and `graphify explain "<concept>"` for focused concepts. These return a scoped subgraph, usually much smaller than GRAPH_REPORT.md or raw grep output.
- If graphify-out/wiki/index.md exists, use it for broad navigation instead of raw source browsing.
- Read graphify-out/GRAPH_REPORT.md only for broad architecture review or when query/path/explain do not surface enough context.
- After modifying code, run `graphify update .` to keep the graph current (AST-only, no API cost).

## Multi-agent workflow

The main session (Opus, set in `.claude/settings.json`) is the **orchestrator**: it plans, delegates, and reports. It does not write code itself for non-trivial tasks.

| Agent | Model | Use for |
|---|---|---|
| `extractor` | haiku | Read-only lookups: pull facts/snippets/build errors out of files with `path:line` refs |
| `advisor` | opus | Design decisions and the implementation plan for non-trivial changes |
| `coder` | sonnet | Writing/editing code from a concrete plan |
| `validator` | opus | Reviewing the diff against the plan and running `idf.py build` |

Flow for a code change: extractor (gather context) → advisor (plan, skip for trivial edits) → coder (implement) → validator (review + build) → back to coder on FAIL. Pass each agent the full context it needs; agents start cold. Run independent agents in parallel.

## Serial port (COM) in the dev container

Docker Desktop on Windows cannot pass COM ports into Linux containers directly. The container talks to the board through an RFC2217 bridge on the host:

1. On Windows (outside the container): `pip install esptool`, then `.\tools\serial-bridge.ps1` (auto-picks the only COM port, or `-ComPort COM5`).
2. In the container, `ESPPORT` is preset to `rfc2217://host.docker.internal:4000?ign_set_control`, so plain `idf.py flash monitor` works.
