#include<iostream>
#include<fstream>
#include<string>
#include<stdexcept>
#include <algorithm>

using namespace std;
class Entry // make the word entry include word as key others as item
{
private:
	string key;
	static const int max = 5;
	int itemcount = 0;    // show as how many pos in this word
	string item[max];     // one pos save as one item
	Entry* leftChildPtr;  // pointer to left child
	Entry* rightChildPtr; // pointer to right child

public:
	Entry() :leftChildPtr(nullptr), rightChildPtr(nullptr){}
	Entry(const string& keyItem) :key(keyItem), leftChildPtr(nullptr), rightChildPtr(nullptr){}
	void setItem(const string& anItem, int count)
	{
		if (count < max)
			item[count] = anItem;
		itemcount++;
	}
	string getItem(int count) const
	{
		return item[count];
	}
	void setKey(const string& anItem)
	{
		key = anItem;
	}
	string getKey() const
	{
		return key;
	}

	Entry* getLeft() const
	{
		return leftChildPtr;
	}
	Entry* getRight() const
	{
		return rightChildPtr;
	}

	void setLeft(Entry* leftPtr)
	{
		leftChildPtr = leftPtr;
	}
	void setRight(Entry* rightPtr)
	{
		rightChildPtr = rightPtr;
	}

	void modcount() // modify itemcount 
	{
		itemcount--;
	}
	void display() // display all the word relatives as same frame when it was put in
	{
		cout << key << item[0] << endl;
		for (int i = 1; i < itemcount; i++) // use loop to show up all the item
		{
			cout << item[i] << endl;
		}
		cout << item[itemcount];
	}
	void displayPos() 
	{
		string pos[4] = { "v. ", "adj. ", "adv. ", "n." }; // save the pos as an matrix ready to compare with
		for (int i = 0; i < itemcount; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (item[i].find(pos[j]) < item[i].length()) // have find the above pos then print it out
				{
					cout << pos[j] << endl;
				}
			}
		}
	}

};

class BinaryNodeTree
{
private:
	Entry* rootPtr;
	int nodeCount = 0; // count how many node in this tree

public:
	BinaryNodeTree() :rootPtr(nullptr){}
	BinaryNodeTree(const Entry rootItem){ rootPtr = new Entry(rootItem); }
	void BinaryNodeTree::destroyTree(Entry* subTreePtr)
	{
		if (subTreePtr != nullptr)
		{
			destroyTree(subTreePtr->getLeft());
			destroyTree(subTreePtr->getRight());
			delete subTreePtr;
		} 
	} 

	~BinaryNodeTree(){ { destroyTree(rootPtr); } }
	int getNodeCount() 
	{
		return nodeCount;
	}
	Entry* getRoot(){
		return rootPtr;
	}
	bool add(Entry* newData) // use an inserInorder to add a node
	{
		Entry* newNodePtr = newData;
		rootPtr = insertInorder(rootPtr, newNodePtr);
		nodeCount++;
		return true;
	} 
	Entry* insertInorder(Entry* subTreePtr, Entry* newNodePtr)
	{
		if (subTreePtr == nullptr)
			return newNodePtr;
		else if (subTreePtr->getKey() > newNodePtr->getKey()) // use word string as key to compare if bigger set at right,
		{
			Entry* tempPtr = insertInorder(subTreePtr->getLeft(), newNodePtr);
			subTreePtr->setLeft(tempPtr);
		}
		else // otherwise set at left
		{
			Entry* tempPtr = insertInorder(subTreePtr->getRight(), newNodePtr);
			subTreePtr->setRight(tempPtr);
		}

		return subTreePtr;
	}

	void inorder(Entry* treePtr) const// display the word as inorder
	{
		if (treePtr != nullptr) {
			inorder(treePtr->getLeft());
			treePtr->display();
			inorder(treePtr->getRight());
		}
	}
	void check(Entry* treePtr, string& test) // use a string test to include all the word key to check for the find & remove function
	{
		if (treePtr != nullptr) {
			check(treePtr->getLeft(), test);
			test = test + treePtr->getKey() + " ";
			check(treePtr->getRight(), test);
		}
	}
	void range(Entry* treePtr, string& w1, string& w2)
	{
		if (treePtr != nullptr) {
			range(treePtr->getLeft(), w1, w2);
			if (treePtr->getKey() >= w1 && treePtr->getKey() <= w2) // only the word key bigger than w1 and amller than w2 will be display
				treePtr->display();
			range(treePtr->getRight(), w1, w2);
		}
	}
	Entry* removeValue(Entry* subTreePtr, string target, bool& success)
	{
		if (subTreePtr == nullptr) {
			success = false;
			return nullptr;
		}
		else if (subTreePtr->getKey() == target) {
			subTreePtr = removeNode(subTreePtr);
			success = true;
			return subTreePtr;
		}
		else if (subTreePtr->getKey() > target) {
			Entry* tempPtr = removeValue(subTreePtr->getLeft(), target, success);
			subTreePtr->setLeft(tempPtr);
			return subTreePtr;
		}
		else {
			Entry* tempPtr = removeValue(subTreePtr->getRight(), target, success);
			subTreePtr->setRight(tempPtr);
			return subTreePtr;
		}
		// use the word key as value to find the node want to remove
	}
	Entry* removeNode(Entry* nodePtr) // remove the node in three situation
	{
		if (nodePtr->getLeft() == nullptr && nodePtr->getRight() == nullptr) { //the node is a leaf
			delete nodePtr;                                                    // just remove the node
			nodePtr = nullptr;
			return nodePtr;
		}
		else if ((nodePtr->getLeft() != nullptr) | (nodePtr->getRight() != nullptr)) { // the node has one child
			Entry* nodeToConnectPtr = new Entry();                                     // use a new ptr to get its child
			if (nodePtr->getLeft() != nullptr)                                         // then remove the node return the newptr as new subtreeptr
				nodeToConnectPtr = nodePtr->getLeft();
			else
				nodeToConnectPtr = nodePtr->getRight();

			delete nodePtr;
			nodePtr = nullptr;
			return nodeToConnectPtr;
		}
		else {
			string successor;                                                          // the node has two child
			Entry* tempPtr = removeLeftmostNode(nodePtr->getRight(), successor);       // find the inorder successor to replace the node's value
			nodePtr->setRight(tempPtr);
			nodePtr->setKey(successor);
			return nodePtr;                                                            
		}

	}
	Entry* removeLeftmostNode(Entry* nodePtr, string& inorderSuccessor) // find the inorder successor
	{
		if (nodePtr->getLeft() == nullptr) {
			inorderSuccessor = nodePtr->getKey();                                     // return the node's value and remove this node
			return removeNode(nodePtr);
		}
		else {
			Entry* tempPtr = removeLeftmostNode(nodePtr->getLeft(), inorderSuccessor); // keep get the finally left node of the right subtree
			nodePtr->setLeft(tempPtr);
			return nodePtr;
		}
	}
	bool remove(string target)throw(out_of_range) // remove a word key value
	{
		int error = 0;
		string test;
		check(rootPtr, test);
		if (test.find(target) != string::npos)               // check is the word exist or not
			error = 1;
		if (error == 0)
			throw out_of_range("Word not found");
		bool success = false;
		rootPtr = removeValue(rootPtr, target, success);
		return success;
	}
	Entry* find(string target)throw(out_of_range)   // find a word key value return the nodeptr
	{
		int error = 0;
		string test;
		check(rootPtr, test);
		if (test.find(target) != string::npos)        // check is the word exist or not
			error = 1; 
		if (error == 0)
			throw out_of_range("Word not found");
		Entry* nodePtr = new Entry();
		nodePtr = findNode(rootPtr, target);
		return nodePtr;
	}
	Entry* findNode(Entry* subTreePtr, string target) // use word key value to compare and get the nodeptr
	{
		if (subTreePtr == nullptr)
			return nullptr;
		else if (subTreePtr->getKey() == target)
			return subTreePtr;
		else if (subTreePtr->getKey() > target)
			return findNode(subTreePtr->getLeft(), target);
		else
			return findNode(subTreePtr->getRight(), target);
	}
	void inorderTraverse() const // show up the whole tree
	{
		inorder(rootPtr);
	}
};

int main(int argc, char **argv)
{
	ifstream infile(argv[1]);
	string strtmp;
	int count = 0;          // use to control the item put in
	BinaryNodeTree b;
	Entry* a = new Entry(); // whole down use a as tmp nodeptr to save data
	while (getline(infile, strtmp, '\n'))
	{
		if (strtmp[0] == '-') 
		{
			if (strtmp[1] == '-') // if the line data begin with '--' it's a pos line just save as a item
			{
				a->setItem(strtmp.substr(1, strtmp.find('\n')), count);
				count++;
			}
			else                 // if the line data begin only with one '-' it's a continuous line follow the last pos item
			{
				string tmp = a->getItem(count - 1);               // so, get back to get the last item as a string
				tmp += strtmp.substr(1, strtmp.find('\n'));        // plus this line
				a->setItem(tmp, count - 1);                       // put it back
				a->modcount();                                     // as we don't really save a new pos item modify the entry's itemcount
			}
		}
		else                    // if the line begin with other char it's a new word
		{
			if (a->getKey().length() == 0)                         // since at very first the nodeptr a has no key need to deal with that
				a->setKey(strtmp.substr(0, strtmp.find(' ')));
			else
			{
				b.add(a);                                          // following process need to add the nodeptr first into the tree
				Entry* newentry = new Entry(strtmp.substr(0, strtmp.find(' '))); // create a newnodeptr construct with new wordkey
				a = newentry;
				count = 0;                                                       // initiate the count variable since a new nodeptr
			}
			a->setItem(strtmp.substr((strtmp.substr(0, strtmp.find(' '))).length(), strtmp.find('\n')), count);
			count++;                                               // the rest line save as a new item
		}
	}
	b.add(a);  // save the last one node
	infile.close(); // close the file



	ifstream insample(argv[2]);
	string pos[4] = { "v. ", "adj. ", "adv. ", "n." };
	string s7 = "os";
	string s6 = "delete";
	string s5 = "find";
	string s4 = "new";
	string s3 = "count";
	string s2 = "quit";
	string s1 = "/0";

	while (s1.find(s2) != 0)                                           // as the receive command not a "quit" keep the loop
	{
		getline(insample, s1, '\n');

			if (!s1.find(s6))                                          // get the line command as "delete"
			{
				string target = s1.substr(7, s1.length() - 6);
				try{
					b.remove(target);
				}
				catch (out_of_range e){
					cout << e.what() << endl;
				}
			}
			if (!s1.find(s3))                                          // get the line command as "count"
				cout << b.getNodeCount() << endl;
			if (!s1.find(s5))                                          // get the line command as "find" first
			{
				string check = s1.substr(5, s1.length() - 4);          // define the command line in three condition
				if (check.substr(0, 2) == s7)                          // check the command is "findpos"
				{
					string target2 = s1.substr(8, s1.length() - 8);
					b.find(target2)->displayPos();
				}
				else if (check.find(' ') != 0 && check.find(' ')<check.length()) // a ' ' in command line to check the command is "find w1 w2"(range)
				{
					string word1 = check.substr(0, check.find(' '));
					string word2 = check.substr(check.find(' ') + 1, check.length() - check.find(' '));
					try{                                                         // check two word exist in the tree or not
						b.find(word1+" ");
						b.find(word2+" ");
						if (word1 < word2)
							b.range(b.getRoot(), word1, word2);
						else
							b.range(b.getRoot(), word2, word1);
					}
					catch (out_of_range e){
						cout << e.what() << endl;
					}
				}
				else {                                                           // check the command is only just "find"
					string want = s1.substr(5, s1.length() - 5);
					try{
						b.find(want)->display();
					}
					catch (out_of_range e){
						cout << e.what() << endl;
					}
				}
			}
			if (!s1.find(s4))                                           // the command is "new"
			{
				string item;
				int control2 = 0;
				Entry* newNode = new Entry();
				newNode->setKey(s1.substr(4, s1.find('\"') - 5));       // first save the word key
				item = " " + s1.substr(s1.find('['), s1.length());      // save the first item
				newNode->setItem(item, control2);
				control2++;
				getline(insample, s1);                                  
				while (s1.find('\"')== string::npos)                    // '\"' in the reading line as a break code
				{
					if (s1[1] == '-') // save as pos item 
					{
						newNode->setItem(s1, control2);
						control2++;
					}
					else              // save in the last pos item
					{
						item = newNode->getItem(control2 - 1);
						item = item + "\n" + s1;
						newNode->setItem(item, control2 - 1);
						newNode->modcount();
					}
					getline(insample, s1);                              // continue get the following data in this section
				}
				item = newNode->getItem(control2 - 1);                  // need to deal with the last line with '\"'
				item = item + "\n" + s1.replace(s1.length() - 1, 1, "\n");
				newNode->setItem(item, control2 - 1);
				newNode->modcount();
				b.add(newNode);                                         // finally add the node into the tree
			}
		}
	insample.close();
}
