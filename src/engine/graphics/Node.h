#ifndef NODE_H
#define NODE_H

#include <string>

class MatrixNode;
class Geode;

class Node {
public:
	virtual MatrixNode* asMatrixNode() = 0;
	virtual Geode* asGeode() = 0;
	virtual std::string getName() = 0;
};
#endif