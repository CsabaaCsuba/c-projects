
//AST: set of rules of what can happen in the language
//CST: the concrete realization of that rule based on the input code

enum NodeType {
	END_NODE,	//concrete number
	DEEP_NODE,	//an other expression
};

enum Operation {
	Add,
	Multiply,
	Subtract,
	Divide
};

struct OpNode;

struct OpNode {
	int numeric_val;

	int left_val;
	int right_val;

	Operation node_op;

	enum NodeType type;

	struct OpNode* left_node;

	struct OpNode* right_node;
};

struct OpNode Sub_Node = OpNode{};

struct OpNode RootNode = OpNode{10, Add, END_NODE, nullptr, nullptr};

void interpret_node() {

}