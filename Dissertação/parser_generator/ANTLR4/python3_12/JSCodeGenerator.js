import antlr4 from 'antlr4';
import PythonParser  from './PythonParser.js'; // Import the generated parser
import PythonParserVisitor from './PythonParserVisitor.js'; // Import the generated visitor base class
import PythonLexer from "./PythonLexer.js";
class JSCodeGenerator extends PythonParserVisitor {
    constructor(context = {}) {
        super();
        // An external context to store variables (like a symbol table)
        this.context = context;
        this.context.symbolTable = {};
    }
    visitFile_input(ctx)
    {
        let x = this.visitChildren(ctx)
        console.log("On file input" + x[0][0][0][0][0])
        return x;
    }

    visitAssignment(ctx)
    {
        let x = this.visitChildren(ctx)
        console.log("on assignment" + x[0][0][0][0][0])
        return x;
    }
    // Visit an assignment node (e.g., x = 5)
    visitAssignment(ctx) {
        // Get the variable name (assuming structure based on your tree)
        const variableName = this.visit(ctx.star_targets())
        console.log("Nome da variavel" + variableName)
        // Get the value from the right-hand side (expression)
        const value = this.visit(ctx.star_expressions());
        console.log("Valor da variavel" + value)

        // Return the JavaScript equivalent assignment statement
        return `let ${variableName} = ${value};`;
    }

    visitSum(ctx) {
        // Check if this is a recursive sum case
        if (ctx.sum()) {
            const left = this.visit(ctx.sum());

            // Get the operator and evaluate
            const operator = ctx.children[1].getText();
            const right = this.visit(ctx.term());

            return (left + " " + operator + " " + right)
        } else {
            // It's just a term
            return this.visit(ctx.term());
        }
    }

    visitStar_atom(ctx) {
        const variableName = ctx.getText();
        return variableName;  // Otherwise, return the variable name as is
    }

    // Handle variable lookups (e.g., using a variable that has been previously assigned)
    visitAtom(ctx) {
        const variableValue = ctx.getText();
        return variableValue;  //
    }
}

// Usage Example:

const input = `
x = 5 + 2
`; // Example Python-like input

// Create a parser
// Create a new input stream from the input
const inputStream = new antlr4.InputStream(input);

// Create a lexer that feeds off the input stream
const lexer = new PythonLexer(inputStream);

// Create a token stream from the lexer
const tokens = new antlr4.CommonTokenStream(lexer);

// Create a parser that feeds off the token stream
const parser = new PythonParser(tokens);

// Start parsing at the desired rule (assuming 'file_input' is the start rule)
const tree = parser.file_input(); // Adjust this to your start rule

// Create the JS code generator (visitor) and pass a context object
const codeGenerator = new JSCodeGenerator({});

// Visit the tree and generate JavaScript code
const outputCode = codeGenerator.visit(tree);

// Output the generated JavaScript code
console.log(outputCode);

