# Triage Labels

The skills speak in terms of five canonical triage roles. This file maps those roles to the actual label strings used in this repo's issue tracker.

## Authority

This repo's Planner/Executor relay workflow is governed by the `.agent/` collaboration contract. **Contract §1.3's five status labels are the single source of truth** for issue status:

`ready` · `in-progress` · `in-review` · `blocked` · `needs-info`

- The legacy triage vocabulary (`ready-for-agent` / `ready-for-human` / `needs-triage`) is **deprecated and its labels have been deleted** from the tracker — do not use or recreate them.
- Priority labels (`P0`–`P4`) and the source label `auto-discovered` come from the same contract and remain in use.
- At most one status label per issue at any time.

## Role mapping

| Skill role (mattpocock/skills) | Label to apply in this repo                                        | Meaning                                  |
| ------------------------------ | ------------------------------------------------------------------ | ---------------------------------------- |
| `needs-triage`                 | *(no status label)* — leave unlabelled until Planner triage; apply `ready` once it passes the gate | Maintainer needs to evaluate this issue  |
| `needs-info`                   | `needs-info`                                                       | Waiting on a product decision / more information |
| `ready-for-agent`              | `ready`                                                            | Fully specified, ready for the Executor agent |
| `ready-for-human`              | *(no equivalent)* — such work does not enter the Executor queue; apply `needs-info` when a user decision is required | Requires human implementation            |
| `wontfix`                      | `wontfix`                                                          | Will not be actioned (standard GitHub label) |

When a skill mentions a role (e.g. "apply the AFK-ready triage label"), use the corresponding label string from the right-hand column above. Never apply the deprecated strings `ready-for-agent` / `ready-for-human` / `needs-triage` — those labels no longer exist.
