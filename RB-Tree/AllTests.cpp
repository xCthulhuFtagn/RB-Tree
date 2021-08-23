#include "AllTests.h"

#include "TestingFramework.h"
#include "RB-CLASS.h"

void TestAll() {
	TestRunner tr;
	tr.RunTest(TestInsertion, "Insertion check");
	tr.RunTest(TestDeletion, "Deletion check");
	tr.RunTest(TestSearch, "Search check");
	tr.RunTest(Fail, "Fail");
}

void TestInsertion() {
	RBTree<int> tree;
	RBNode<int> tester;
	tester.data = 11;
	tree.insert(11);

}

void TestDeletion() {
	RBTree<int> tree;
	tree.insert(11);
}

void TestSearch() {
	RBTree<int> tree;
	RBNode<int> tester;
	tester.data = 11;
	tree.insert(11);
	Assert(tester.data == tester.find(11), "TestSearch 1");
}

void Fail() {
	Assert(false, "");
}