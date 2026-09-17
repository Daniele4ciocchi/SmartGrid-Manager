# AGENTS.md - Quick Start Guide

## 🔎 Investigation Strategy
1.  **Priority Sources**: Start with `README*`, root manifests, workspace config, lockfiles, and build/test/lint configs.
2.  **Execution Source**: Trust executable sources (scripts, config files) over prose documentation when conflicts arise.
3.  **Architecture**: Inspect representative code files to find entrypoints and boundaries if configuration is unclear.

## ✨ High-Signal Extraction
*   **Commands**: Capture exact developer commands, especially non-obvious ones.
*   **Verification**: Extract how to run a single test, package, or focused verification step.
*   **Order of Operations**: Note required command sequences (e.g., `lint -> typecheck -> test`).
*   **Structure**: Identify monorepo boundaries, major directory ownership, and real app/library entrypoints.
*   **Quirks**: Document framework quirks (codegen, migrations, special env loading) and testing prerequisites.
*   **Conventions**: Record repo-specific style or workflow conventions that deviate from defaults.

## ❓ Questions Policy
*   Only ask questions if the repository cannot answer something critical.
*   Use `question` tool sparingly for undocumented team conventions or missing setup prerequisites.

## ✍️ Writing Rules (Include Only)
*   Exact commands and shortcuts the agent might guess wrong.
*   Architecture notes not obvious from filenames.
*   Conventions that differ from language/framework defaults.
*   Setup requirements, environment quirks, and operational gotchas.
*   References to existing instruction sources that are crucial.

## 🚫 Exclude
*   Generic software advice.
*   Long tutorials or exhaustive file trees.
*   Obvious language conventions.
*   Speculative claims.