# LL(k) Grammars

**LL(k) grammars** are a subset of context-free grammars that can be parsed by a top-down parsing technique called LL parsing, where:

- **L** stands for **Left-to-right** scanning of the input.

- The second **L** denotes **Leftmost derivation** of the sentence.

- **k** indicates the number of lookahead tokens that the parser uses to make parsing decisions.

- **Characteristics**:

  - LL(k) grammars are usually easier to parse compared to other types like LR(k) grammars.
  - They require no left recursion and are deterministic, meaning that for every parsing decision, the correct path can be chosen based on the lookahead tokens
