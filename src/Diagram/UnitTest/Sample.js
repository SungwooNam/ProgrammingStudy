TestCase("AssertionsTestCase", {
  
	testAlwaysPass : function(){
		var expected = 1, actual = 1;
		assertEquals("The vales should be the same", expected, actual);
		assertEquals(expected, actual);

		var myStr = "hello";
		assertString("The variable should contain a string", myStr);
		assertString(myStr);
	},
  
	testAlwaysFail : function(){
		assertEquals(1, 2);
	},
 
	testAlwaysFailWithMessage : function(){
		assertEquals("1<>2", 1, 2);
	}
});