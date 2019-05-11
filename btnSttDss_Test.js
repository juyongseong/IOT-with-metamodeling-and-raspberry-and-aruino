
						if(command_robot==="전진을")
						{
							console.log("start" + "\n");
							var headers = {
								'Content-Type':'text/plain'
//								'Content-Length':body.length
							};
							
					
							const request = require('request');
							var body={"1_t":"car","1_qs":"F","2_t":"car","2_qs":"ST","state":"2"};
							JSON.stringify(body);
							
							console.log(body);
							let options = {
								uri:"http://192.168.0.20:10000",
								method:'GET',
								qs:{
									id:'adoxx',
									name:'Automobile01'
								},
								form:body,
								headers:headers,
								json:true
							};
							request(options,function(err,res,body)
							{
								//console.log(body);

							});
						
						}
