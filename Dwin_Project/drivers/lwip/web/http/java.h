const unsigned char Java_file_1[] =
"var xmlHttp;"
"var s_num=\"001\";"
"var s;"
"var s1=0;"
"function showCustomer(str)"
	"{" 
		"xmlHttp=GetXmlHttpObject();"
		"if (xmlHttp==null)"
		"{"
			"alert (\"您的浏览器不支持AJAX！\");"
			"return;"
		"}"
		//"var url=\"getcustomer.asp\";	"	 /Web/Souce/Asp/file_1.asp
		"var url=\"/Web/Souce/Asp/file_1.asp\";	"
	//		"alert (str);" 	
		" s1=str;"
	//	"FORM_Display_Bufften();"
	//	"url=Fun_select_send(url);"	
	//	"alert (s1);" 
		//"url=url+\"?q=\"+str;"
	//	"url=url+\"&sid=\"+Math.random();"
		"xmlHttp.onreadystatechange=stateChanged; "
		"xmlHttp.open(\"GET\",url,true);"
		"xmlHttp.send(null);"
	"}"
			
"function stateChanged() "
"{"
	"if(xmlHttp.readyState==4)"
	"{"
		"document.getElementById(\"txtHint\").innerHTML=xmlHttp.responseText; "
	"}"
"}"

"function GetXmlHttpObject()"
"{"
	"var xmlHttp=null;"
	"try"
		"{"
		 // Firefox, Opera 8.0+, Safari
		"xmlHttp=new XMLHttpRequest();"
		"}"
	"catch (e)"
	"{"
	  // Internet Explorer
		"try"
		"{"
		"xmlHttp=new ActiveXObject(\"Msxml2.XMLHTTP\");"
		"}"
		"catch (e)"
		"{"
		"xmlHttp=new ActiveXObject(\"Microsoft.XMLHTTP\");"
		"}"
	"}"
	"return xmlHttp;"
"}"
"function formSubmit()"
"{"
	"document.getElementById(\"myForm\").submit();"
"} "
"function formSubmit_read()"
"{"
	"formSubmit();"
"}"
"function formSubmit_set()"
"{"
   "formSubmit();"
"}"
"function FORM_ADD()"
"{" 

	"xmlHttp=GetXmlHttpObject();"
	"if (xmlHttp==null)"
	"{"
		"alert (\"您的浏览器不支持AJAX！\");"
		"return;"
	"}"
	"var url=\"/Web/Souce/Asp/file_1.asp\";	"
	"s1=s1+1;"
	"FORM_Display_Bufften();"
	"FID_Form.customers.value=s1;"
	"url=Fun_select_send(url);"
	//"url=url+\"&sid=\"+Math.random();"
	"xmlHttp.onreadystatechange=stateChanged; "
	"xmlHttp.open(\"GET\",url,true);"
	"xmlHttp.send(null);"

"}"
"function FORM_Display_Bufften()"
"{"
	"if(s1 <0 )"
	 "{"
	 	"s1=1;"
		" document.getElementById(\"next\").disabled=true;"

	 "}"
	 "else"
	 "{"
		 "if( document.getElementById(\"next\").disabled == true)"
		 "{"
			 " document.getElementById(\"next\").disabled=false;"
		 "}"

	   "if(s1 <20)"
		"{"
			"if( document.getElementById(\"Previous\").disabled ==true){"
			 " document.getElementById(\"Previous\").disabled=false;"
		   "}"
		"}"
		"if(s1>20)"
		"{"
			  "s1=20;"
			
		   	" document.getElementById(\"Previous\").disabled=true;"
		"}"
	 "}"
"}"
"function FORM_SUB()"
"{" 
		"xmlHttp=GetXmlHttpObject();"
		"if (xmlHttp==null)"
		"{"
			"alert (\"您的浏览器不支持AJAX！\");"
			"return;"
		"}"
		"var url=\"/Web/Souce/Asp/file_1.asp\";"
		"s1=s1-1;"
		"FID_Form.customers.value=s1;"
		"FORM_Display_Bufften();"
		"url=Fun_select_send(url);"
		"xmlHttp.onreadystatechange=stateChanged; "
		"xmlHttp.open(\"GET\",url,true);"
		"xmlHttp.send(null);"
"}"

"function Fun_Display()"
"{"
	"document.write(\"第\"+s1+\"页\");"

"}"
"function Fun_select_send(url_1)"
"{"
	"switch(s1) "
	"{"
		"case 1:url_1=url_1+\"?q=001\";break;"
		"case 2:url_1=url_1+\"?q=002\";break;"
		"case 3:url_1=url_1+\"?q=003\";break;"
		"case 4:url_1=url_1+\"?q=004\";break;"
		"case 5:url_1=url_1+\"?q=005\";break;"
		"case 6:url_1=url_1+\"?q=006\";break;"
	"}"
	"return url_1;"
"}"
/*
 "function getNews(newsID)"
"{"
  //如果没有把参数newsID传进来
  "if (typeof(newsID) == 'undefined')"
  "{"
  "return false;"
  "}"
//需要进行Ajax的URL地址
  "switch(newsID)"
  "{"
  	"case 1:url = \"/show.php?id=\"+ newsID;"
  
  
  "}"
 "var url = \"/show.php?id=\"+ newsID;"	// /Web/Souce/User/User_ALARM.html\

  //获取新闻显示层的位置
 "var show = document.getElementById(\"show_news\");" 

  //实例化Ajax对象
  "var ajax = InitAjax();"

  //使用Get方式进行请求
  "ajax.open(\"GET\", url, true);" 

  //获取执行状态
 "ajax.onreadystatechange = function() {" 
	//如果执行是状态正常，那么就把返回的内容赋值给上面指定的层
	"if (ajax.readyState == 4 && ajax.status == 200) {" 
		"show.innerHTML = ajax.responseText;" 
	"}" 
  "}"
  //发送空
  "ajax.send(null);" 
"}"  */

;


//用户登录JAVA脚本
const unsigned char Java_file_2[] =

"function gb2utf8(data){"
"var glbEncode = [];"
"gb2utf8_data = data;"
"execScript(\"gb2utf8_data = MidB(gb2utf8_data, 1)\", \"VBScript\");"
"var t=escape(gb2utf8_data).replace(/%u/g,"").replace(/(.{2})(.{2})/g,\"%$2%$1\").replace(/%([A-Z].)%(.{2})/g,\"@$1$2\");"
"t=t.split(\"@\");"
"var i=0,j=t.length,k;"
"while(++i<j) {"
"k=t.substring(0,4);"
"if(!glbEncode[k]) {"
"gb2utf8_char = eval(\"0x\"+k);"
"execScript(\"gb2utf8_char = Chr(gb2utf8_char)\", \"VBScript\");"
"glbEncode[k]=escape(gb2utf8_char).substring(1,6);"
"}"
"t=glbEncode[k]+t.substring(4);"
"}"
"gb2utf8_data = gb2utf8_char = null;"
"return unescape(t.join(\"%\"));"
"}"


"function stateChanged() "
"{"
	"if(xmlHttp.readyState==4)"
	"{"


		"document.getElementById(\"main_body\").innerHTML=gb2utf8(xmlHttp.responseText); "
		
		
	    //"document.getElementById(\"main_body\").innerHTML=gb2utf8(http_request.responseBody);" 

	"}"
"}"



"function GetXmlHttpObject()"
"{"
	"var xmlHttp=null;"
	"try"
		"{"
		 // Firefox, Opera 8.0+, Safari
		"xmlHttp=new XMLHttpRequest();"
		"}"
	"catch (e)"
	"{"
	  // Internet Explorer
		"try"
		"{"
		"xmlHttp=new ActiveXObject(\"Msxml2.XMLHTTP\");"
		"}"
		"catch (e)"
		"{"
		"xmlHttp=new ActiveXObject(\"Microsoft.XMLHTTP\");"
		"}"
	"}"
	"return xmlHttp;"
"}"

"function in_user()"
"{" 

	"xmlHttp=GetXmlHttpObject();"
	"if (xmlHttp==null)"
	"{"
		"alert (\"您的浏览器不支持AJAX！\");"
		"return;"
	"}"
	"var url=\"/Web\";"
 	"var userName = userform.user.value;"
	"var userPwd =  userform.password.value;"
    "var postStr = \"user=\"+ userName +\"&password&=\"+ userPwd+\"&END\";"
	"xmlHttp.onreadystatechange=stateChanged; "
	"xmlHttp.open(\"POST\", url, true);"
//	"xmlHttp.setRequestHeader(\"Content-Type\",\"application/x-www-form-urlencoded\");" 
	"xmlHttp.send(postStr);" 

"}"	;


/**
* 初始化一个xmlhttp对象
*/
/*

"function InitAjax()"
"{"
"var ajax=false;" 
"　try {" 
"　　ajax = new ActiveXObject(\"Msxml2.XMLHTTP\"); "
"{catch (e) { "
   "try {" 
   "ajax = new ActiveXObject(\"Microsoft.XMLHTTP\");" 
"} catch (E) {" 
"ajax = false; "
"}" 
"}"
"if (!ajax && typeof XMLHttpRequest!='undefined') { "
"ajax = new XMLHttpRequest();" 
"}" 
"return ajax;"
"}" 
"function getNews(newsID)"
"{"
  //如果没有把参数newsID传进来
  "if (typeof(newsID) == 'undefined')"
  "{"
  "return false;"
  "}"
//需要进行Ajax的URL地址
 "var url = \"/show.php?id=\"+ newsID;"

  //获取新闻显示层的位置
 "var show = document.getElementById(\"show_news\");" 

  //实例化Ajax对象
  "var ajax = InitAjax();"

  //使用Get方式进行请求
  "ajax.open(\"GET\", url, true);" 

  //获取执行状态
 "ajax.onreadystatechange = function() {" 
	//如果执行是状态正常，那么就把返回的内容赋值给上面指定的层
	"if (ajax.readyState == 4 && ajax.status == 200) {" 
		"show.innerHTML = ajax.responseText;" 
	"}" 
  "}"
  //发送空
  "ajax.send(null);" 
"}" 
 "function saveUserInfo()"
"{"
 //获取接受返回信息层
 "var msg = document.getElementById(\"msg\");"

 //获取表单对象和用户信息值
 "var f = document.user_info;"
 "var userName = f.user_name.value;"
 "var userAge = f.user_age.value;"
 "var userSex = f.user_sex.value;"

  //接收表单的URL地址
  "var url = \"/save_info.php\";"

  //需要POST的值，把每个变量都通过&来联接
  "var postStr = \"user_name=\"+ userName +\"&user_age=\"+ userAge +\"&user_sex=\"+ userSex;"

  //实例化Ajax
  "var ajax = InitAjax();"
  //通过Post方式打开连接
  "ajax.open(\"POST\", url, true);" 

  //定义传输的文件HTTP头信息
 "ajax.setRequestHeader(\"Content-Type\",\"application/x-www-form-urlencoded\");" 

  //发送POST数据
  "ajax.send(postStr);"

  //获取执行状态
  "ajax.onreadystatechange = function() {" 
	//如果执行状态成功，那么就把返回信息写到指定的层里
	"if (ajax.readyState == 4 && ajax.status == 200) { "
      "msg.innerHTML = ajax.responseText;" 
    "}" 
  "}" 
"}" ;
 */

//超级用户登录脚本
const unsigned char Java_file_3[1];








