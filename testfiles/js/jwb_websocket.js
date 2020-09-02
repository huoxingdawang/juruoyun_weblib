var jwb=new function(){};
jwb.info=new function(){};
jwb.info.ws=new function(){};
jwb.info.ws.url="ws://"+window.location.host;
jwb.websocket = new function()
{
	var socket=null;
	this.stop=false;
	var timer1;
	var timer2;
	var listener=[];
	var listener_type=[];
	var error=[];
	var send_buf=[];
	this.status=0;
	if(true||typeof SharedWorker=='undefined')
	{
		var start=()=>
		{
			if(this.stop/*||jry_wb_login_user==undefined||jry_wb_login_user.id<=0||jry_wb_login_user.id==''*/)
				return;
			if(socket!==null)return socket.close();
			socket=new WebSocket(jwb.info.ws.url);
			socket.onopen=(evt)=>
			{
				onstart();
				this.status=socket.readyState;
			};
			socket.onmessage=(evt)=>
			{
				var reader=new FileReader();
				reader.addEventListener("loadend",function()
				{
					console.log( "Received Message: " + reader.result+"\nlength="+reader.result.length);
					callback(reader.result);
				});
				reader.readAsText(evt.data);
			};
			socket.onclose=(evt)=>
			{
				this.stop=false;
				this.status=socket.readyState;
				socket=null;
				onclose();
				setTimeout(()=>
				{
					start();
				},1000);
			};
		};
	}
	else
	{
		var start=()=>
		{
			
		}
	}
	var onclose=()=>
	{
		console.log("close");
		document.getElementById('button').disabled="disabled";
	};
	var onstart=()=>
	{
		console.log("start");
		document.getElementById('button').disabled="";
	};	
	var callback=(data)=>
	{
		var div=document.createElement("div");document.getElementById('output').insertBefore(div,document.getElementById('output').firstElementChild)
		div.innerHTML= data;
	};
	start();
	this.start=start;
	this.send=(data,add_buf)=>
	{
		console.log('Send Message: ',data);
		if(socket!=null&&socket.readyState==1)
		{
			socket.send(data);
			return true;
		}
		return false;
	};
};
