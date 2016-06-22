#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string>

enum class ERRCODE{ unknown = 99 };

class NODE;
using PNODE = NODE*;

class NODE
{
private:
	enum Dimension{ Two = 2, Three, Four };
	uint32_t *ArrayofKey;
	NODE **dPointtoChild;
	uint32_t theNumberofKey;

public:
	NODE() : ArrayofKey{ new uint32_t[Four]{0, } }, dPointtoChild{ new NODE*[Four]{nullptr} },
		theNumberofKey{ 0 } {};

	~NODE()
	{
		delete[] ArrayofKey;
		delete[] dPointtoChild;
	}

	PNODE& LeftSide(uint32_t index) { return dPointtoChild[index]; }
	PNODE& RightSide(uint32_t index){ return dPointtoChild[index + 1]; }
	uint32_t& Key(uint32_t index) { return ArrayofKey[index]; }

	const PNODE& LeftSide(uint32_t index) const { return dPointtoChild[index]; }
	const PNODE& RightSide(uint32_t index) const { return dPointtoChild[index]; }
	const uint32_t& Key(uint32_t index) const { return ArrayofKey[index]; }
	const uint32_t Size() { return theNumberofKey; }

	void Clear(Dimension dim)
	{
		for (int i = 0; i < dim; i++) {
			ArrayofKey[i] = 0;
		}

		for (int i = 0; i < dim; i++) {
			dPointtoChild[i] = nullptr;
		}

		theNumberofKey = 0;
	}

	bool AddKey(uint32_t, PNODE, PNODE);
	bool DelKey(uint32_t);
	int32_t FindKey(uint32_t);
};

bool NODE::AddKey(uint32_t value, PNODE left, PNODE right)
{
	int i = (Size() - 1);

	for (; i >= 0 && Key(i) > value; i--) {
		Key(i + 1) = Key(i);
		LeftSide(i + 1) = LeftSide(i);
	}

	Key(i) = value;
	LeftSide(i) = left;
	RightSide(i) = right;

	theNumberofKey++;
}

bool NODE::DelKey(uint32_t index)
{
	for (; index < (theNumberofKey - 1); index++) {
		Key(index) = Key(index + 1);
		LeftSide(index) = LeftSide(index + 1);
	}

	Key(index) = 0;
	theNumberofKey--;

	return true;
	/*
	int i = 0;

	for (; i <= (theNumberofKey - 1) && Key(i) < value; i++);

	for (; i < (theNumberofKey - 1); i++) {
	Key(i) = Key(i + 1);
	LeftSide(i) = LeftSide(i + 1);
	}

	Key(i) = 0;
	theNumberofKey --;

	return true;
	*/
}

int32_t NODE::FindKey(uint32_t target)
{
	for (int i = 0; i < (theNumberofKey - 1); i++) {
		if (Key(i) == target)
			return i;

		else
			return -1;
	}
}

enum Dimension{ Two = 2, Three, Four };

class BTREE
{
private:
	PNODE RooT, Head;
	uint32_t theNumberofNode;
	Dimension NodeDimension = Four;


protected:
	BTREE() : RooT{ nullptr }, Head{ nullptr }, theNumberofNode{ 0 } {};
	~BTREE();

public:
	uint32_t GetCount() const { return theNumberofNode; }
	bool Find() const;
	bool Insert(const uint32_t&);
	bool Remove();
	void RemoveAll();
	
	void Merge();
	void Borrow();
	void Swap();
	PNODE Split(PNODE, const uint32_t&);
};

PNODE BTREE::Split(PNODE parents, const uint32_t &value)
{
	int i, j = 0;
	PNODE left = nullptr, right = nullptr;

	if (parents == RooT)
	{
		left = parents;
		right = new NODE;

		for (i = (NodeDimension / 2) + 1; i < left->Size(); i++, j++) {
			right->Key(j) = left->Key(i);
			right->LeftSide(j) = left->LeftSide(i);
		}

		left->Size() = NodeDimension / 2;
		right->Size() = j;

		RooT = new NODE();
		Head = RooT;
		Head->AddKey(left->Key(NodeDimension / 2), left, right);

		parents = Head;
	}

	else 
	{
		for (; i < parents->Size() && parents->Key(i) < value; i++);

		left = parents->LeftSide(i);
		right = new NODE();

		for (i = (NodeDimension / 2) + 1, j = 0; i < left->Size(); i++, j++) {
			right->Key(j) = left->Key(i);
			right->LeftSide(j) = left->LeftSide(i);
		}
		right->LeftSide(j) = left->LeftSide(i);

		left->Size() = NodeDimension / 2;
		right->Size() = j;

		parents->AddKey(left->Key(NodeDimension / 2), left, right);
	}

	return parents;
}

bool BTREE::Insert (const uint32_t &value)
{
	PNODE before = Head;
	PNODE present = Head;

	if (present == nullptr) {
		Head = new NODE;
		RooT = Head;
		present = Head;
		theNumberofNode++;
	}

	int i = 0;

	while (present != nullptr) {

		if (present->FindKey(value) >= 0 )
			return false;

		if (present->Size() == Four)
			present = Split(before, value);

		before = present;

		for ( ; i < before->Size() && before->Key(i) < value; i++);
		
		present = before->LeftSide(i);
	}

	before->AddKey(value, 0, 0);

	/* log for debug*/
	for (i = 0; i < before->Size(); i++)
		std::cout << before->Key(i);
	std::cout << std::endl;

	return true;
}


int main(void)
{

	return 0;
}