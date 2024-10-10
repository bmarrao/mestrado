const antlr4 = require('antlr4');
const PythonLexer = require('./PythonLexer').PythonLexer;
const PythonParser = require('./PythonParser').PythonParser;

// Input string to parse (replace with your actual input)
const input = `# This is a comment
print("Hello, World!")`;

// Create a lexer and parser
const chars = new antlr4.InputStream(input);
const lexer = new PythonLexer(chars);
const tokens = new antlr4.CommonTokenStream(lexer);
const parser = new PythonParser(tokens);

// Start parsing from the desired rule (replace 'startRule' with your actual starting rule)
const tree = parser.startRule(); // Change 'startRule' to your actual starting rule name

// Print the parse tree
console.log(tree.toStringTree(parser.ruleNames));

