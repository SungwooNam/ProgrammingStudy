TestCase("TestJoint", {
  
	testPoint : function() {
		var a = joint.g.point( 123.456, 789.123 );
		assertEquals( "123.456@789.123", a.toString() );
		
		var b = joint.g.point( a.toString() )
		assertEquals( "123@789",  b.toString() );
		
		a = joint.g.point(  0, 0 );
		a.offset( 30, 40 );
		assertEquals( "30@40",  a.toString() );

		b = joint.g.point(  0, 0 );
		assertEquals( 50,  a.distance( b ) );

		assertEquals( 70,  a.manhattanDistance( b ) );
		
		assertEquals( 50,  a.magnitude() );
	
		assertEquals( "-30@-40", b.difference(a).toString());
		
		a = joint.g.point( 10, 0 );
		assertEquals( "2@0", a.normalize(2));
		
		a = joint.g.point( 0, 10 ).toPolar();
		assertEquals( 10, a.x );
		assertEquals( 270, joint.g.toDeg( a.y ) ); // NOT 90 ?
		
		a = joint.g.point( 3, 4 );
		a.move( joint.g.point( 0, 0 ), 5 );
		assertEquals( "6@8", a.round().toString() );
		
		// a = joint.g.point( 0, 1 );
		// assertEquals( 90, joint.g.toDeg( a.changeInAngle( -1, 1, joint.g.point(0,0)) ) );
	},  
	
	testLine : function() {
	
	},
	
});