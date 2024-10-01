import { createToken, Lexer } from "chevrotain"; // Correctly import using ES Modules

// Create tokens
const Identifier = createToken({ name: "Identifier", pattern: /[a-zA-Z]\w*/ });
const Integer = createToken({ name: "Integer", pattern: /0|[1-9]\d*/ });
const WhiteSpace = createToken({
  name: "WhiteSpace",
  pattern: /\s+/,
  group: Lexer.SKIPPED,  // Use Lexer directly since it's imported
});

const Select = createToken({
  name: "Select",
  pattern: /SELECT/,
  longer_alt: Identifier,
});

const From = createToken({
  name: "From",
  pattern: /FROM/,
  longer_alt: Identifier,
});

const Where = createToken({
  name: "Where",
  pattern: /WHERE/,
  longer_alt: Identifier,
});

// Combine all tokens into an array
let allTokens = [
  WhiteSpace,
  Select,
  From,
  Where,
  Identifier,
  Integer,
];

let SelectLexer = new Lexer(allTokens);

let inputText = "SELECT column1 FROM table2";
let lexingResult = SelectLexer.tokenize(inputText);

console.log(lexingResult);

