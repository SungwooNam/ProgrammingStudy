#include <boost/test/unit_test.hpp>
#include <vector>
#include <numeric>
#include <limits>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <math.h>
#include <stdio.h>

using namespace std;

namespace closest_common_ancestor_01
{

class Node
{
public:
	Node()
	{
		parent = left = right = NULL;
	}

	Node *parent;
	Node *left;
	Node *right;

	int value;

	Node* SetupNode( Node* leftNode, Node* rightNode )
	{
		left = leftNode;
		right = rightNode;

		if( left != NULL )
			left->parent = this;

		if( right != NULL )
			right->parent = this;

		return this;
	}

};

Node* tree_forest[];

Node* closest_common_ancestor( Node *n1, Node *n2 )
{
	Node *temp = n2;

	for( ; n1!=NULL; n1 = n1->parent )
	{
		for( n2 = temp; n2!=NULL; n2 = n2->parent) 
		{
			if( n1 == n2 ) 
			{
				return n1;
			}
		}
	}

	return NULL;
}

void setupNodeVector( vector<Node>& nodes )
{
	for( size_t i=0; i<nodes.size(); ++i)
	{
		nodes[i].value = (int)i;
	}
}



}


BOOST_AUTO_TEST_CASE( Test_closest_common_ancestor_O1 )
{
	using namespace closest_common_ancestor_01;

	vector<Node> n(10);
	setupNodeVector( n );

	n[0].SetupNode( 
		n[1].SetupNode( &n[3], &n[4] ), 
		n[2].SetupNode( &n[5], &n[6] ) );

	n[7].SetupNode( 
		n[8].SetupNode( &n[9], NULL ), 
		NULL );

	Node* common = closest_common_ancestor( &n[3], &n[5] );
	BOOST_CHECK_EQUAL( common->value, 0 );

	common = closest_common_ancestor( &n[1], &n[4] );
	BOOST_CHECK_EQUAL( common->value, 1 );

	common = closest_common_ancestor( &n[4], &n[1] );
	BOOST_CHECK_EQUAL( common->value, 1 );

	common = closest_common_ancestor( &n[9], &n[3] );
	BOOST_CHECK( common == NULL );
}
