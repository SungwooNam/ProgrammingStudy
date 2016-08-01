TestCase("TestBackbone", {
  
	testJSON : function() {
		var artist = new Backbone.Model({
			firstName: "Wassily",
			lastName: "Kandinsky"
		});
		artist.set({birthday: "December 16, 1866"});
		assertEquals( "{\"firstName\":\"Wassily\",\"lastName\":\"Kandinsky\",\"birthday\":\"December 16, 1866\"}", JSON.stringify(artist) );
	},  
	
	testModel0 : function() {

		var nsw = (function(){
			function namespace( string ) {
				var object = this;
				var levels = string.split(".");
				for( var i=0, l = levels.length; i<1; i++) {
					if (typeof object[levels[i]] == "undefined") {
						object[ levels[i] ] = {};
					}
					object = object[levels[i]];
				}
				
				return object;
			}
			
			return { 
				namespace: namespace
			};
		}());
		
		nsw.graph = {
			Node : Backbone.Model.extend({
				default : {
					name : {},
				},
			}),
			Edge : Backbone.Model.extend({
				default : {
					name : {},
					source : {},
					target : {},
				}
			}),
			Graph : Backbone.Model.extend({
				default : {
					name : {}
					nodes : []
					edges : []
				}
			}),
		};		
		
		var ng = nsw.graph;
		var n0 = new ng.Node({ name : "Hello", });
		var n1 = new ng.Node({ name : "World", });
		var e0 = new ng.Edge({ 
			name : "",
			source : n0,
			target : n1 
		});
		
		var gr = new ng.Gragh({ 
			name : "",
			nodes : [n0,n1],
			edges : [e0] 
		});
	},  
	
});